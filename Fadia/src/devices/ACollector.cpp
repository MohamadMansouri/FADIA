/*
 * Collector.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: mmrota
 */




#include "../devices/ACollector.h"

#include <string.h>
#include <omnetpp.h>
#include <algorithm>

#include "../devices/AProver.h"
#include "../devices/NetworkOwner.h"
#include "../messages/Join_m.h"
#include "../messages/Attest_m.h"
#include "../messages/Update_m.h"

using namespace omnetpp;



Define_Module(ACollector);


void ACollector::initialize()
{
    initUID();
    device = SERV;
    elapsedtimesig = registerSignal("elapsedTime");
    reportsizesig = registerSignal("reportSize");
    revokesig = registerSignal("keyRevoked");
    // initPorverKeys();
}

void ACollector::initUID()
{
    do
    {
        CId = (getBaseID<cid_t>() + getParentModule()->getIndex()) % MAXCID;
    } while(!CId);
}


void
ACollector::handleJoinReq(cMessage* msg)
{
    JoinReq* reqmsg = check_and_cast<JoinReq *>(msg);
    if((cid_t)reqmsg->getDestination() != CId)
    {
        logError("handleJoinReq: received message with wrong dest");
        delete msg;
        return;
    }

    uid_t senderid = (uid_t) reqmsg->getSource();

    if(!checkMAC<JoinReq>(reqmsg, proverKeys[senderid]))
    {
        logError("handleJoinReq: Invalid MAC");
        delete msg;
        return;
    }
    
    size_t kidsize = (size_t) reqmsg->getKidArraySize();
    for(size_t i=0; i < kidsize; ++i)
    {
        proverKeyIds[senderid].push_back((keyid_t)reqmsg->getKid(i));
    }
    updateProverKey(senderid);

    jsessions.push_back(senderid);
    statusTable[senderid] = make_pair(true, SimTime());
    sendJoinResp(senderid);
    delete msg;
    
}

void
ACollector::sendJoinResp(uid_t target)
{
    JoinResp* msg = new JoinResp();
    msg->setKind(JNRP);
    msg->setSource(CId);
    msg->setDestination(target);
    msg->setDepth(maxdepth);
    msg->setDeltah(deltah);
    msg->setMac(generateMAC(msg, proverKeys[target]));
    msg->setByteLength(JRP_SIZE);
    // TODO: fix mac delay
    scheduleAt(simTime(), msg);
}

// void
// ACollector::handleJoinAck(cMessage* msg)
// {
//     JoinAck* ackmsg = check_and_cast<JoinAck *>(msg);
//     if((cid_t)ackmsg->getDestination() != CId)
//     {
//         logError("handleJoinReq: received message with wrong dest");
//         return;
//     }
//     if(find(jsessions.begin(),jsessions.end(),(uid_t)ackmsg->getSource()) == jsessions.end())
//     {
//         logError("handleJoinReq: session is not open");
//         return;
//     }

//     uid_t senderid = (uid_t) ackmsg->getSource();

//     if(!checkMAC<JoinAck>(ackmsg))
//     {
//         logError("handleJoinReq: Invalid MAC");
//     }
//     statusTable[senderid] = make_pair(true, SimTime());
// }

void
ACollector::handleUpReq(cMessage* msg)
{
    // if(procstat != PBUSY)
    // {
    //     logWarn("handleUpReq: report received but collector not yet set to busy");
    //     procstat = PBUSY;
    // }

    scheduleAt(simTime() + processuptime, procendmsg);

    UpdateReq* umsg = check_and_cast<UpdateReq *> (msg);
    // TODO: check if the update is a fix
    size_t size = umsg->getReport1ArraySize();
    size_t s = 0;
    while(size--)
    {
        if(umsg->getReport1(size) != NOID)
            s++;
    }
    logInfo("handleUpReq: received a report of size " + to_string(s));
    sum += s;
    logInfo("handleUpReq: Total attested devices = " + to_string(sum));
    // cout << "Collector: received a report of size " + to_string(s) << endl;
    // ostringstream oss;
    // while(s)
    //     oss << umsg->getReport1(--s) << ", ";
    // logDetail("handleUpReq: GOOD devices= " + oss.str());
    // cout << "handleUpReq: GOOD devices= " << oss.str() << endl;
    delete msg;
    if(checkRecordTime())
    {
        emit(reportsizesig, s);
    }
#ifdef RUNTIME_TEST
    emit(elapsedtimesig, simTime().dbl());
    endSimulation();
#endif
}

void
ACollector::sendRevReq(uid_t target, vector<keyid_t> kids)
{
    RevokeReq* msg = new RevokeReq();
    msg->setKind(RVKRQ);
    msg->setDestination(target);
    msg->setKidArraySize(kids.size());
    for(int i = 0; i < kids.size(); ++i)
    {
        msg->setKid(i, kids[i]);
    }
    msg->setMac(generateMAC(msg, 0));
    msg->setByteLength(RVK_SIZE(kids.size()));
    double macd = macDelay<RevokeReq>(msg);
    scheduleAt(simTime() + macd, msg);
}

void
ACollector::handleRevReq(cMessage* msg)
{
    emit(revokesig,-1);

    RevokeReq* rreq = check_and_cast<RevokeReq*>(msg);
    size_t size = rreq->getKidArraySize();
    vector<keyid_t> keyids;
    while(size--)
    {
        keyids.push_back(rreq->getKid(size));
    }


    int s = getSystemModule()->getSubmodule("prover", 0)->getVectorSize();
    assert(s);


    for(int i = 0; i < s; ++i)
    {
        AProver* mod =  check_and_cast<AProver*>(getSystemModule()->getSubmodule("prover", i)->getSubmodule("proverapp"));
        vector<keyid_t> revkids = mod->sharedKeys(keyids);
        if(!revkids.empty())
        {
            sendRevReq(mod->getUID(), revkids);
        }
    }    
    delete msg; 
}


void
ACollector::sendSyncReq()
{

}

void
ACollector::handleSyncReq(cMessage* msg)
{

}



void
ACollector::logDebug(string m)
{
    EV_DEBUG << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
}

void
ACollector::logInfo(string m)
{
    EV << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
}

void
ACollector::logDetail(string m)
{
    EV_DETAIL << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
}

void
ACollector::logError(string m)
{
    ostringstream oss;
    oss << "Collector [Index=" << getParentModule()->getIndex()
        << ", CID=" << hex <<  CId << "]: " << dec<< m << endl;
    EV_ERROR << oss.str();
    getSimulation()->getActiveEnvir()->alert(oss.str().c_str());

}

void
ACollector::logWarn(string m)
{
    EV_WARN << "Collector [Index=" << getParentModule()->getIndex()
             << ", CID=" <<  CId << "]: " << dec<< m << endl;
}

int
ACollector::chooseProve(double p)
{
    return 1;
}

int
ACollector::chooseVerify(double p)
{
    return 2;
}

void ACollector::updateProverKey(uid_t uid)
{
    // TODO: compute ckey from proverKeyIds
    proverKeys[uid] = 0xCAFED00D;
}

bool  ACollector::isChannelBusyServ(int far, int gid, uid_t target)
{
        int i = target - getBaseID<uid_t>();
        AProver* mod =  check_and_cast<AProver*>(getSystemModule()->getSubmodule("prover", i)->getSubmodule("proverapp"));
        return mod->isChannelBusy(far,gid);
}

void ACollector::finish()
{
        cancelAndDelete(txmsg);
        cancelAndDelete(entxmsg);
        cancelAndDelete(procendmsg);
        cancelAndDelete(cbusymsg);
        
}
