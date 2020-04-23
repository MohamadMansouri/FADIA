/*
 * Collector.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: mmrota
 */




#include <string.h>
#include <omnetpp.h>
#include <algorithm>

#include "NetworkOwner.h"
#include "ACollector.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"

using namespace omnetpp;



Define_Module(ACollector);


void ACollector::initialize()
{
    initUID();
    elapsedtimesig = registerSignal("elapsedTime");
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
    scheduleAt(simTime() + macdelay*2, msg);
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
    UpdateReq* umsg = check_and_cast<UpdateReq *> (msg);

    size_t s = umsg->getReport1ArraySize();
    logInfo("handleUpReq: received a report of size " + to_string(s));
    cout << "Collector: received a report of size " + to_string(s) << endl;
    // ostringstream oss;
    // while(s)
    //     oss << umsg->getReport1(--s) << ", ";
    // logDetail("handleUpReq: GOOD devices= " + oss.str());
    // cout << "handleUpReq: GOOD devices= " << oss.str() << endl;
    delete msg;
    emit(elapsedtimesig, simTime().dbl());
    endSimulation();
}

void
ACollector::sendRevReq(uid_t comdev)
{

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
