/*
 * Collector.cpp
 *
 *  Created on: Mar 6, 2020
 *      Author: mmrota
 */


#include <string.h>
#include <omnetpp.h>
#include <map>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <assert.h>

#include <fstream>

#include "AProver.h"
#include "NeighborDiscovery_m.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"
#include "Commit_m.h"

#include "../inet/src/inet/power/contract/IEpEnergyStorage.h"

#define DEBUG 1

#define Bubble(_text_) getParentModule()->bubble(_text_)


// session_t::session_t()
// {
//     session_t(NOID, NOID, NOID, 0);

// }

// session_t::session_t(treeid_t t, uid_t uid, keyid_t k, unsigned int d, bool v)
// {
//     treeID = t; 
//     deviceID = uid;
//     keyID = k;
//     depth = d;
//     valid = v;
// }


// The module class needs to be registered with OMNeT++
Define_Module(AProver);


void
AProver::initialize()
{
    initNO();
    initUID();
    initKeyRing();
    deltah = ((unsigned int) getSystemModule()->par("htime"));
    range = ((double) getParentModule()->par("range"));
    maxdepth = ((unsigned int) getParentModule()->par("maxdepth"));

    startmsg = new cMessage("Join", JOIN);
    scheduleAt(simTime() + uniform(0,23), startmsg);

    jointomsg = new cMessage();
    jointomsg->setKind(JNRPTO);

    attesttimer = new cMessage();
    attesttimer->setKind(ATTEST);

    mktreetimer = new cMessage(); 
    mktreetimer->setKind(MKTREE);

    checkdelaymsg = new cMessage();
    checkdelaymsg->setKind(CHECK);

    creqmsg = new CommitReq();
    creqmsg->setKind(CMRQ);
    creqmsg->setSource(UId);
    creqmsg->setKidArraySize(KeyRing.size());
    size_t i = 0;
    for(auto it = KeyRing.begin(); it != KeyRing.end(); ++it)
    {
        creqmsg->setKid(i++, it->first);
    }

    cresptomsg = new CommitTimeOut();
    cresptomsg->setKind(CMRPTO);

    // cackmsg = new CommitAck();
    // cackmsg->setKind(CMAK);
    // cackmsg->setSource(UId);

    cacktomsg = new CommitTimeOut();
    cacktomsg->setKind(CMAKTO);


}

void
AProver::initNO()
{
    if(!NO.initialized)
    {
        KRsize = getSystemModule()->par("keyRingSize");
        KPsize = getSystemModule()->par("keyPoolSize");
        cRNG* rng = getRNG(seed);
        NO = NetworkOwner(rng, KPsize, KRsize);
        NO.initialized = true;
    }
}
void
AProver::initUID()
{
    do
    {
        UId = (getBaseID<uid_t>() + getParentModule()->getIndex()) % MAXUID;
    } while(!UId);
}

void
AProver::initKeyRing()
{
    KeyRing = NO.getKeyRing();
}



void
AProver::sendJoinReq()
{
    status = JOINING;
    cid = findClosestCollector();
    if (cid == NOID)
        return;

    JoinReq* msg = new JoinReq();
    msg->setKind(JNRQ);
    msg->setSource(UId);
    msg->setDestination(cid);
    msg->setKidArraySize(KeyRing.size());
    size_t i = 0;
    for(auto it = KeyRing.begin(); it != KeyRing.end(); ++it)
    {
        msg->setKid(i++, it->first);
    }
    // msg->setBattery(getBatteryLevel());
    msg->setMac(generateMAC(msg, ckey));
    scheduleAt(simTime() + macdelay, msg);

    scheduleAt(simTime() + timeout + macdelay, jointomsg);
}

void
AProver::handleJoinResp(cMessage* msg)
{

    JoinResp* resmsg = check_and_cast<JoinResp *>(msg);
    if((uid_t)resmsg->getDestination() != UId
        || (cid_t)resmsg->getSource() != cid)
    {
        logError("handleJoinResp: received message with wrong src/dest");
        delete msg;
        return;
    }

    if(jointomsg->isScheduled())
        cancelEvent(jointomsg);

    // uid_t senderid = (uid_t) resmsg->getSource();
    maxdepth = (unsigned int) resmsg->getDepth();
    deltah = (double) resmsg->getDeltah();
    deltag = deltah * 0.2;
    if(checkMAC<JoinResp>(resmsg, ckey))
        scheduleAt(simTime() +  macdelay, attesttimer);

    // sendJoinAck(senderid);
    delete msg;
}

void
AProver::handleJoinRespTimeOut(cMessage* msg)
{
    sendJoinReq();
    delete msg;
}

// void
// AProver::handleJoinAckTimeOut(cMessage* msg)
// {
// }

// void
// AProver::sendJoinAck(uid_t target)
// {
//     JoinAck* msg = new JoinAck();
//     msg->setKind(JNAK);
//     msg->setSource(UId);
//     msg->setDestination(target);
//     msg->setMac(generateMAC(msg, ckey));
//     scheduleAt(simTime() + macdelay, msg);

// }

void
AProver::startAttestation()
{

    if(status != FINISHED && status != JOINING)
    {
        logWarn("Device didn't attest last deltah!!!");
        // TODO: take action here
    }

    status = CHECKING;
    
    cDisplayString& displayString = getParentModule()->getDisplayString();
    displayString.setTagArg("i", 0, "misc/sensor3");

    for (auto it = csessions.begin(); it != csessions.end(); ++it)
    {
        if(it->second)
            delete it->second;
    }
    csessions.clear();

    if (psessions.second)
    {
        delete psessions.second;
        psessions.second = nullptr;
    }
    psessions.first = NOID;

    scheduleAt(simTime() + deltah, attesttimer);
    scheduleAt(simTime() + deltag, mktreetimer);
    scheduleAt(simTime() + checkDelay, checkdelaymsg);

}

void
AProver::handleCommitReq(cMessage* msg)
{
    if (status != READY)
    {
        delete msg;
        return;
    }


    CommitReq* rmsg = check_and_cast<CommitReq *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();
    treeid_t tid = (treeid_t) rmsg->getTreeID();
    
    // // chek if this you are already
    // if(csessions_bydev.find(senderid) != csessions_bydev.end())
    //     return;

    // // check if this guy is already a parent
    // if(psessions_bytree.find(tid) != psessions_bytree.end()
    //     || psessions_bydev.find(senderid) != psessions_bydev.end())
    //     return;


    size_t kidsize = (size_t) rmsg->getKidArraySize();
    unsigned int depth = (unsigned int) rmsg->getDepth();

    for(size_t i=0; i < kidsize; ++i)
    {
        auto it = KeyRing.find((keyid_t)rmsg->getKid(i));
        if( it != KeyRing.end())
        {
            session_t* sess = new session_t(tid, senderid, it->first, depth);
            psessions.first = senderid;
            psessions.second = sess;
            sendCommitResp(senderid, tid);
            delete msg;
            return;
        }
    }
    delete msg;
    return;
}



void
AProver::sendCommitResp(uid_t target, treeid_t tid)
{
    status = ATTESTING;

    CommitResp* crespmsg = new CommitResp();
    crespmsg->setKind(CMRP);
    crespmsg->setSource(UId);
    crespmsg->setDestination(target);
    crespmsg->setTreeID(tid);
    keyid_t kid = psessions.second->keyID;
    crespmsg->setKid(kid);
    crespmsg->setMac(generateMAC(crespmsg, kid));
    scheduleAt(simTime() + macdelay, crespmsg);

    cacktomsg->setDevice(target);
    cacktomsg->setTreeID(tid);
    scheduleAt(simTime() + macdelay + timeout, cacktomsg);

}


void
AProver::handleCommitAck(cMessage* msg)
{
    CommitAck* rmsg = check_and_cast<CommitAck *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();
    treeid_t tid = (treeid_t) rmsg->getTreeID();

    if(psessions.first != senderid)
    {
        logDebug("handleCommitAck: Unexpected Ack (probably from a timedout session)");
        delete msg;
        return;
    }

    if((uid_t)rmsg->getDestination() != UId)
    {
        logError("handleCommitAck: wrong destination!!!");
        delete msg;
        return;
    }

    keyid_t kid = psessions.second->keyID;

    if(KeyRing.find(kid) == KeyRing.end())
    {
        logDebug("handleCommitAck: I dont have this key!!!");
        delete msg;
        return;
    }

    if(!checkMAC<CommitAck>(rmsg, kid))    
    {
        logDebug("handleCommitAck: Invalid MAC!!!");
        delete msg;
        return;
    }


    if(mktreetimer->isScheduled())
        cancelEvent(mktreetimer);

    if(cacktomsg->isScheduled())
        cancelEvent(cacktomsg);

    psessions.second->valid = true;
    if(psessions.second->depth == 1 || maxchildren == 0)
    {
        status = FINISHED;
        sendUpReq(tid);
    }
    
    else    
    {
        status = CREATING;
        sendCommitReq(tid);
    }
    delete msg;
}


void
AProver::handleCommitAckTimeOut(cMessage* msg)
{
    logWarn("handleCommitAckTimeOut: Ack not received");
    CommitTimeOut* tomsg = check_and_cast<CommitTimeOut*>(msg);
    uid_t dev = tomsg->getDevice();
    if (dev != psessions.first)
    {
        logError("handleCommitAckTimeOut: timeout with wrong data received!!!");
        return;
    }
    // treeid_t tid = tomsg->getTreeID();
    delete psessions.second;
    psessions.second = nullptr;
    psessions.first = NOID;
    if (status == ATTESTING_CREATING)
    {
        status = CREATING;
        sendCommitReq(NOID);
    }
    else
        status = READY;
}

void
AProver::sendCommitReq(treeid_t tid)
{    
    unsigned int depth;
    treeid_t treeid;

    if (status == ATTESTING)
    {
        logDebug("sendCommitReq: deltag finished while in attesting phase");
        status = ATTESTING_CREATING;
        return;
    }

    treeid = uniform(0, MAXTREEID);

    if(maxchildren == 0)
    {
        sendUpReq(treeid);
        return;
    }

    if (psessions.first == NOID) // This is the root prover
    {
        session_t* sess = new session_t(treeid, NOID, ckey, maxdepth, true);
        psessions.second = sess;
        depth = maxdepth;
    }
    else
    {
        depth = psessions.second->depth - 1;
        treeid =  tid;
    }
    
    status = CREATING;


    creqmsg->setDepth(depth);
    creqmsg->setTreeID(treeid);
    scheduleAt(simTime(), creqmsg);

    session_t* sess = new session_t();
    sess->treeID = treeid;
    csessions[NOID] = sess;


    cresptomsg->setKind(CMRPTO);
    cresptomsg->setTreeID(treeid);
    //TODO: check the time out here
    scheduleAt(simTime() + timeout*maxchildren, cresptomsg);

}




void
AProver::handleCommitRespTimeOut(cMessage* msg)
{
    logDebug("handleCommitRespTimeOut: No more responses can be accepted");
    status = COLLECTING;
    // delete csessions[NOID];
    if (csessions.size() == 1)
    {
        logDebug("handleCommitRespTimeOut: No childrens left... sending report");
        CommitTimeOut* tomsg = check_and_cast<CommitTimeOut *>(msg);
        treeid_t tid = tomsg->getTreeID();
        sendUpReq(tid);
    }
    // auto range = csessions.;
    // for (auto r = range.first; r != range.second; ++r)
    // {
    //     if(r->second->tomsg)
    //     {
    //         delete r->second;
    //         break;
    //     } 
    // } 
}

void
AProver::handleCommitResp(cMessage* msg)
{
    if(status == COLLECTING)
    {
        logDebug("handleCommitResp: Request has already timedout!!!");
        delete msg;
        return;
    }

    if (status != CREATING)
    {
        logError("handleCommitResp: Unexpected status");
        delete msg;
        return;
    }

    if (csessions.size() - 1 >= maxchildren)
    {
        logError("handleCommitResp: Unexpected status (should be waiting)");
        delete msg;
        return;
    }

    CommitResp* rmsg = check_and_cast<CommitResp *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();
    treeid_t tid = (treeid_t) rmsg->getTreeID();


    if((uid_t)rmsg->getDestination() != UId)
    {
        logError("handleCommitResp: wrong destination!!!");
        delete msg;
        return;
    }
    
    if(csessions.find(senderid) != csessions.end())
    {
        logDebug("handleCommitResp: Session already exist!!!");
        delete msg;
        return;
    }

    keyid_t kid = (keyid_t) rmsg->getKid();

    if(KeyRing.find(kid) == KeyRing.end())
    {
        logDebug("handleCommitResp: I dont have this key!!!");
        delete msg;
        return;
    }

    if(!checkMAC<CommitResp>(rmsg,kid))    
    {
        logDebug("handleCommitResp: Invalid MAC!!!");
        delete msg;
        return;
    }

    // unsigned int depth = (unsigned int) rmsg->getDepth();

    session_t* sess = new session_t(tid, senderid, kid, 0, true);
    csessions[senderid] = sess;
    sendCommitAck(senderid, tid);

    if(csessions.size() == maxchildren + 1)
    {
        status = COLLECTING;
        cancelEvent(cresptomsg);
    }
    delete msg;

}


void
AProver::sendCommitAck(uid_t target, treeid_t tid)
{

    CommitAck* cackmsg = new CommitAck();
    cackmsg->setKind(CMAK);
    cackmsg->setSource(UId);
    cackmsg->setDestination(target);
    cackmsg->setTreeID(tid);
    keyid_t kid = csessions[target]->keyID;
    cackmsg->setMac(generateMAC(cackmsg, kid));
    scheduleAt(simTime() + macdelay, cackmsg);

    
    // TODO: Continue here


    CommitTimeOut* uptomsg = new CommitTimeOut();
    uptomsg->setKind(UPTO);
    uptomsg->setDevice(target);
    uptomsg->setTreeID(tid);
    unsigned int depth =  psessions.second->depth;
    scheduleAt(simTime() + macdelay + timeout * depth *2, uptomsg);

    csessions[target]->tomsg = uptomsg;
}

void 
AProver::handleUpdateTimeOut(cMessage* msg)
{
    CommitTimeOut* tomsg = check_and_cast<CommitTimeOut *>(msg);
    treeid_t tid = tomsg->getTreeID();
    uid_t uid = tomsg->getDevice();
    logDebug("handleUpdateTimeOut: update session of prover (id = " + to_string(uid) + ") timed out");
    if(csessions[uid])
        delete csessions[uid];
    csessions.erase(uid);
    if (csessions.size() == 1)
    {
        status = FINISHED;
        sendUpReq(tid);
    }
    delete msg;
}

void
AProver::sendUpReq(treeid_t tid)
{
    aggreport.tid = tid;
    aggreport.devices[UId] = ++attestcount;
    aggregateProof();
    // Bubble("sending report to parent");
    UpdateReq* msg = new UpdateReq();
    if(psessions.first)
    {
        msg->setDestination(psessions.first);
        msg->setKind(UPRQ);
    }
    else
    {
        msg->setDestination(findClosestCollector());
        msg->setKind(UPRQC);
    }
    msg->setSource(UId);
    msg->setReport1ArraySize(aggreport.devices.size());
    msg->setReport2ArraySize(aggreport.devices.size());
    int i=0;
    for (auto it =  aggreport.devices.begin(); it != aggreport.devices.end(); ++it)
    {
        msg->setReport1(i, it->first);
        msg->setReport2(i++, it->second);
    }
    msg->setTreeID(tid);
    msg->setProof(aggreport.mac);
    msg->setMac(generateMAC(msg, psessions.second->keyID));

    // TODO: update macdelay 
    if(!psessions.first)
    {
        logInfo("sending a report of " + to_string(aggreport.devices.size()) + " records");
        ofstream ofs("/tmp/output", ios::app);
        ofs << "sending a report of " << aggreport.devices.size() << " records" << endl;
    }

    scheduleAt(simTime() + macdelay*2, msg);

    if(csessions[NOID])
        delete csessions[NOID];
    csessions.erase(NOID);

    if(psessions.second)
    {
        delete psessions.second;
        psessions.second = nullptr;
    }
    psessions.first = NOID;

    status = FINISHED;
}

void
AProver::handleUpReq(cMessage* msg)
{

    if(status == FINISHED)
    {
        logWarn("handleUpReq: session is timedout!!!");
        delete msg;
        return;
    }

    if(status != COLLECTING && status != CREATING)
    {
        logError("handleUpReq: Unexpected state!!!");
        delete msg;
        return;
    }

    UpdateReq* upmsg = check_and_cast<UpdateReq *>(msg);

    if((uid_t)upmsg->getDestination() != UId)
    {
        logError("handleUpReq: wrong destination!!!");
        delete msg;
        return;
    }

    uid_t senderid = (uid_t) upmsg->getSource();

    if(csessions.find(senderid) == csessions.end())
    {
        logDebug("handleUpReq: sender is not a child!!!");
        delete msg;
        return;
    }

    treeid_t tid = upmsg->getTreeID();
    if(tid != psessions.second->treeID)
    {
        logError("handleUpReq: wrong tid!!!");
        delete msg;
        return;   
    }

    keyid_t kid = csessions[senderid]->keyID;

    if(!checkMAC<UpdateReq>(upmsg,kid))    
    {
        logDebug("handleUpReq: Invalid MAC!!!");
        delete msg;
        return;
    }
    size_t reportsize = (size_t) upmsg->getReport1ArraySize();
    for(size_t i=0; i < reportsize; ++i)
    {
        aggreport.devices[(uid_t)upmsg->getReport1(i)] = (unsigned int)upmsg->getReport2(i);
    }

    if(csessions[senderid]->tomsg->isScheduled())
        cancelAndDelete(csessions[senderid]->tomsg);

    aggreport.mac ^= upmsg->getProof();
    if(csessions[senderid])
        delete csessions[senderid];
    csessions.erase(senderid);
    if(csessions.size() == 1)
        sendUpReq(tid);

    delete msg;
}

void
AProver::handleRevReq(cMessage* msg)
{

}

// CHLNG
// AProver::generateChallenge()
// {
//     return intrand(INT32_MAX);
// }

// SOLV
// AProver::solveChallenge(uid_t target)
// {
// //    KEY key = KeyRing[sessions[target].first];
// //    CHLNG c = sessions[target].second;
//     return 0;
// }


void
AProver::logDebug(string m)
{
    EV_DEBUG << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=0x" << hex <<  UId << "]: " << dec<< m << endl;
}

void
AProver::logInfo(string m)
{
    EV << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=0x" << hex <<  UId << "]: " << dec<< m << endl;
}

void
AProver::logWarn(string m)
{
    EV_WARN << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=0x" << hex <<  UId << "]: " << dec<< m << endl;
}


void
AProver::logError(string m)
{
    ostringstream oss;
    oss << "Prover [Index=" << getParentModule()->getIndex()
        << ", UID=0x" << hex <<  UId << "]: " << dec<< m << endl;
    EV_ERROR << oss.str();
    getSimulation()->getActiveEnvir()->alert(oss.str().c_str());

}

const cid_t
AProver::findClosestCollector()
{
    return getBaseID<cid_t>();

}

const double
AProver::getBatteryLevel()
{
    using namespace inet;
    using namespace power;
    IEpEnergyStorage *es = check_and_cast<IEpEnergyStorage *>(getParentModule()->getSubmodule("energyStorage"));
    J res = es->getResidualEnergyCapacity();
    J nom = es->getNominalEnergyCapacity();
    double percent =  res.get() / nom.get() ;
    logInfo("Battery level = " + to_string(percent));
    return percent;
}


// void AProver::refreshDisplay() const
// {
//     char buf[60];
//     sprintf(buf, "prover: %ld/%ld verify: %ld/%ld", pcounter, prove, vcounter, verify);
//     getParentModule()->getDisplayString().setTagArg("t", 0, buf);
// }


void AProver::postponeMsg(cMessage* msg)
{
    scheduleAt(simTime() + postponetime, msg);
}


void AProver::checkSoftConfig()
{
    status = READY;
}


void AProver::aggregateProof()
{
    // compute mac of: UID || counter || TID
    aggreport.mac ^=  0;
}

void AProver::finish()
{
        cancelAndDelete(startmsg);
        cancelAndDelete(jointomsg);
        cancelAndDelete(mktreetimer);
        cancelAndDelete(attesttimer);
        cancelAndDelete(checkdelaymsg);
        cancelAndDelete(cresptomsg);
        cancelAndDelete(cacktomsg);
        cancelAndDelete(creqmsg);

}