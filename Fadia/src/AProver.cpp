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

    txsig = registerSignal("dataSent");
    rxsig = registerSignal("dataRcvd");
    crashsig = registerSignal("crashTime");

    // deltah = ((unsigned int) getSystemModule()->par("htime"));
    range = ((double) getParentModule()->par("range"));
    rootidx = ((int) getSystemModule()->par("root"));
    aggsize = ((size_t) getParentModule()->par("aggsize"));
    maxdepth = ((size_t) getSystemModule()->par("maxdepth"));
#ifdef ENERGY_TEST
    statadapt = ((bool) getSystemModule()->par("statadapt"));
#endif
#if SCENARIO == PASTA
    device = NA;
#elif SCENARIO == SALADS
    device = NA;
#elif SCENARIO == FADIA
    device = ((device_t) getParentModule()->par("type").intValue());
#endif
    // maxdepth = ((unsigned int) getParentModule()->par("maxdepth"));
    deltag = deltah - (timeoutresp + timeoutack) * maxdepth - timeoutup; 

#ifdef ENERGY_TEST
    energy = check_and_cast<inet::power::SimpleEpEnergyStorage *> (getParentModule()->getSubmodule("energyStorage"));
    energy->totalPowerConsumption = inet::units::values::mW(IDLE_CONSUMPTION);
#endif
    
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
    creqmsg->setByteLength(CRQ_SIZE(KeyRing.size()));

    cresptomsg = new CommitTimeOut();
    cresptomsg->setKind(CMRPTO);

    // cackmsg = new CommitAck();
    // cackmsg->setKind(CMAK);
    // cackmsg->setSource(UId);

    cacktomsg = new CommitTimeOut();
    cacktomsg->setKind(CMAKTO);

#ifdef RUNTIME_TEST
    startmsg = new cMessage("start", ATTEST);
    scheduleAt(simTime(), startmsg);   
    status = JOINING;

#else
    status = FINISHED;
    startmsg = new cMessage("start", ATTEST);
    scheduleAt(simTime() + uniform(0,deltah), startmsg);   
#endif
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
    msg->setByteLength(JRQ_SIZE(KeyRing.size()));
    scheduleAt(simTime() + macDelay<JoinReq>(msg) + getMacDelays(), msg);

    scheduleAt(simTime() + timeoutack + macDelay<JoinReq>(msg) + getMacDelays(), jointomsg);
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
    deltag = deltah - (timeoutresp + timeoutack) * maxdepth - timeoutup;
    if(checkMAC<JoinResp>(resmsg, ckey))
        scheduleAt(simTime() +  macDelay<JoinResp>(resmsg) + getMacDelays(), attesttimer);

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
//     scheduleAt(simTime() + macDelay(msg), msg);

// }

void
AProver::startAttestation()
{

    if(status != FINISHED && status != JOINING)
    {
        logWarn("Device didn't attest last deltah!!! (status = " + to_string(status) + ")");
        // TODO: take action here
    }
    logInfo("Start attesting");

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
    aggreport.uids.clear();
    aggreport.counters.clear();
    aggreport.proofs.clear();

#ifdef ENERGY_TEST
    if(statadapt)
    {
        updateMaxChildren();
    }

    energy->updateResidualCapacity();
    energy->totalPowerConsumption = inet::units::values::mW(LISTENING_CONSUMPTION);

#endif

#ifdef TREE
    gatsz = connectedGates();
    if(gatsz == MINGSIZE)
        maxchildren = 0;
#endif

    scheduleAt(simTime() + deltah, attesttimer);
#ifndef RUNTIME_TEST
#ifdef ENERGY_TEST
    double d;
    if(statadapt)
        d = chooseTreeDelay();
    else
        d = deltag;
#else
    double d = uniform(0, deltag);
#endif
    scheduleAt(simTime() + checkFirmwareDelay() + d, mktreetimer);
#endif
    scheduleAt(simTime() + checkFirmwareDelay() , checkdelaymsg);


}

void
AProver::handleCommitReq(cMessage* msg)
{
    CommitReq* rmsg = check_and_cast<CommitReq *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();

    logDebug("Received a commitment request from (UID = " + to_string(senderid) + ")");

    if (status != READY)
    {
        switch (status)
        {
            case ATTESTING:
            case ATTESTING_CREATING:
                logDebug("Device is currently attesting. Discarding commitment request");
                break;
            case CREATING:
                logDebug("Device is currently creating a tree. Discarding commitment request");
                break;
            case COLLECTING:
                logDebug("Device is currently collecting. Discarding commitment request");
                break;
                logDebug("Device is not listening. Discarding commitment request");
                break;
            case FINISHED:
            case CHECKING:
            default:
                logDebug("Device is not listening. Discarding commitment request");
        }
        delete msg;
        return;
    }

#ifdef ENERGY_TEST
    if(energy)
    {
        cPacket* pkt = (cPacket*) msg;
        double bl = pkt->getByteLength();
        energy->updateResidualCapacity();
        energy->totalPowerConsumption = inet::units::values::mW(RECEPTION_CONSUMPTION);
        cMessage* enrxmsg = new cMessage("enrxdone", ENRXDONE);
        scheduleAt(simTime() + bl / byterate, enrxmsg);
    }
#endif  

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

    logDebug("Sending a commitment response to (UID = " + to_string(target) + ")");

    status = ATTESTING;

    CommitResp* crespmsg = new CommitResp();
    crespmsg->setKind(CMRP);
    crespmsg->setSource(UId);
    crespmsg->setDestination(target);
    crespmsg->setTreeID(tid);
    keyid_t kid = psessions.second->keyID;
    crespmsg->setKid(kid);
    crespmsg->setMac(generateMAC(crespmsg, kid));
    crespmsg->setByteLength(CRP_SIZE);

    double macd = macDelay<CommitResp>(crespmsg) + getMacDelays();
#ifdef ENERGY_TEST
    cMessage* enmacmsg = new cMessage("MAC Energy consumption");
    enmacmsg->setKind(ENMAC);
    scheduleAt(simTime() + macd , enmacmsg);
#endif    
    scheduleAt(simTime() +  macd , crespmsg);

    cacktomsg->setDevice(target);
    cacktomsg->setTreeID(tid);
    scheduleAt(simTime() + timeoutack, cacktomsg);

}


void
AProver::handleCommitAck(cMessage* msg)
{

    CommitAck* rmsg = check_and_cast<CommitAck *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();

    logDebug("Received a commitment acknowledgment from (UID = " + to_string(senderid) + ")");

#ifdef ENERGY_TEST
    if(energy)
    {
        cPacket* pkt = (cPacket*) msg;
        double bl = pkt->getByteLength();
        energy->updateResidualCapacity();
        energy->totalPowerConsumption = inet::units::values::mW(RECEPTION_CONSUMPTION);
        cMessage* enrxmsg = new cMessage("enrxdone", ENRXDONE);
        scheduleAt(simTime() + bl / byterate, enrxmsg);
    }
#endif  
    
    treeid_t tid = (treeid_t) rmsg->getTreeID();

    if(psessions.first != senderid)
    {
        logWarn("handleCommitAck: Unexpected Ack (probably from a timedout session)");
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
        logError("handleCommitAck: I dont have this key!!!");
        delete msg;
        return;
    }

    if(!checkMAC<CommitAck>(rmsg, kid))    
    {
        logError("handleCommitAck: Invalid MAC!!!");
        delete msg;
        return;
    }


    if(mktreetimer->isScheduled())
        cancelEvent(mktreetimer);

    if(cacktomsg->isScheduled())
        cancelEvent(cacktomsg);

    psessions.second->valid = true;
    if((psessions.second->depth == 1 /*&& !ignoredepth*/) || maxchildren == 0)
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
    CommitTimeOut* tomsg = check_and_cast<CommitTimeOut*>(msg);
    uid_t dev = tomsg->getDevice();
    logDebug("handleCommitAckTimeOut: Ack not received from " + to_string(dev));
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
        session_t* sess = new session_t(treeid, NOID, ckey, maxdepth, true);
        psessions.second = sess;
        depth = maxdepth;
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
    cancelEvent(cresptomsg);
    scheduleAt(simTime() + timeoutresp, cresptomsg);

}




void
AProver::handleCommitRespTimeOut(cMessage* msg)
{
    logDebug("handleCommitRespTimeOut: No more responses can be accepted. Number of children = " + to_string(csessions.size()));
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
    CommitResp* rmsg = check_and_cast<CommitResp *>(msg); 
    uid_t senderid = (uid_t) rmsg->getSource();
    
    logDebug("Received a commitment Response from (UID = " + to_string(senderid) + ")");

    if(status == COLLECTING || status == FINISHED || status == READY)
    {
        logDebug("handleCommitResp: Request has already timedout!!!");
        delete msg;
        return;
    }

    if (status != CREATING)
    {
        logError("handleCommitResp: Unexpected status. Expected " + to_string(CREATING) + " but got " + to_string(status));
        delete msg;
        return;
    }

    if (csessions.size() - 1 >= maxchildren)
    {
        logError("handleCommitResp: Unexpected status (should be waiting)");
        delete msg;
        return;
    }

#ifdef ENERGY_TEST
    if(energy)
    {
        cPacket* pkt = (cPacket*) msg;
        double bl = pkt->getByteLength();
        energy->updateResidualCapacity();
        energy->totalPowerConsumption = inet::units::values::mW(RECEPTION_CONSUMPTION);
        cMessage* enrxmsg = new cMessage("enrxdone", ENRXDONE);
        scheduleAt(simTime() + bl / byterate, enrxmsg);
    }
#endif  

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
    logDebug("Sending a commitment acknowledgment to (UID = " + to_string(target) + ")");

    CommitAck* cackmsg = new CommitAck();
    cackmsg->setKind(CMAK);
    cackmsg->setSource(UId);
    cackmsg->setDestination(target);
    cackmsg->setTreeID(tid);
    keyid_t kid = csessions[target]->keyID;
    cackmsg->setMac(generateMAC(cackmsg, kid));
    cackmsg->setByteLength(CAK_SIZE);
    double macd = macDelay<CommitAck>(cackmsg) + getMacDelays();

#ifdef ENERGY_TEST    
    cMessage* enmacmsg = new cMessage("MAC Energy consumption");
    enmacmsg->setKind(ENMAC);
    scheduleAt(simTime() + macd , enmacmsg);
#endif

    scheduleAt(simTime() + macd , cackmsg);

    
#ifdef WIRELESS
    CommitTimeOut* uptomsg = new CommitTimeOut();
    uptomsg->setKind(UPTO);
    uptomsg->setDevice(target);
    uptomsg->setTreeID(tid);
    unsigned int depth =  psessions.second->depth - 1;
    double delaytime =  (timeoutack + timeoutresp) * depth + timeoutup;
    scheduleAt(simTime() + delaytime, uptomsg);

    csessions[target]->tomsg = uptomsg;
#endif
}

void 
AProver::handleUpdateTimeOut(cMessage* msg)
{
    CommitTimeOut* tomsg = check_and_cast<CommitTimeOut *>(msg);
    treeid_t tid = tomsg->getTreeID();
    uid_t uid = tomsg->getDevice();
    logWarn("handleUpdateTimeOut: update session of prover (UID = " + to_string(uid) + ") timed out");
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
    addOwnReport();
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
    msg->setReport1ArraySize(aggreport.uids.size());
    msg->setReport2ArraySize(aggreport.uids.size());
    
    for (size_t i=0; i < aggreport.uids.size(); ++i)
    {
        msg->setReport1(i, aggreport.uids[i]);
        msg->setReport2(i, aggreport.counters[i]);
    }
    msg->setTreeID(tid);
    int j=0;
    msg->setProofArraySize(aggreport.proofs.size());
    for (auto it =  aggreport.proofs.begin(); it != aggreport.proofs.end(); ++it)
    {
        msg->setProof(j++,*it);
    }
    msg->setMac(generateMAC(msg, psessions.second->keyID));
    msg->setByteLength(URQ_SIZE(aggreport.size - aggreport.sepsize,j));

    if(!psessions.first)
    {
        logInfo("sending a report of " + to_string(aggreport.size) + " records");
    }
    else
    {
        logDebug("sending a report of " + to_string(aggreport.size) + " records to " + to_string(psessions.first));
    }

    double macd = macDelay<UpdateReq>(msg) + getMacDelays();
#ifdef ENERGY_TEST
    cMessage* enmacmsg = new cMessage("MAC Energy consumption");
    enmacmsg->setKind(ENMAC);
    scheduleAt(simTime() + macd , enmacmsg);
#endif
    scheduleAt(simTime() + macd, msg);

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

    if(msg->getKind() == UPRQF)
    {
        sendCollector<UpdateReq>(msg);
        return;
    }

    UpdateReq* upmsg = check_and_cast<UpdateReq *>(msg);

    if((uid_t)upmsg->getDestination() != UId)
    {
        logError("handleUpReq: wrong destination!!!");
        delete msg;
        return;
    }

    cPacket* pkt = (cPacket*) msg;
    double bl = pkt->getByteLength();
    if(checkRecordTime()  && device != SERV)
    {
        emit(rxsig, bl);
    }
#ifdef ENERGY_TEST
    if(energy)
    {
        energy->updateResidualCapacity();
        energy->totalPowerConsumption = inet::units::values::mW(RECEPTION_CONSUMPTION);
        cMessage* enrxmsg = new cMessage("enrxdone", ENRXDONE);
        scheduleAt(simTime() + bl / byterate, enrxmsg);
    }
#endif 

    if(status == FINISHED)
    {
        logWarn("handleUpReq: session of child (UID = " + to_string(upmsg->getSource()) + ") is already timedout!!! ");
        upmsg->setKind(UPRQF);
        upmsg->setDestination(findClosestCollector());
        sendCollector<UpdateReq>(upmsg);
        return;
    }

    if(status != COLLECTING && status != CREATING)
    {
        logError("handleUpReq: Unexpected state!!!");
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
    

    // remove unfinialized proof aggregates

    size_t a = aggreport.uids.size();
    size_t uncompleted = 0;
    while (a && aggreport.uids[a-1] != NOID)
    {
        --a;
        ++uncompleted;
    }

    vector<uid_t> tmpuids(aggreport.uids.begin() + a, aggreport.uids.end());
    vector<size_t> tmpcounters(aggreport.counters.begin() + a, aggreport.counters.end());
    mac_t tmpproof;
    if(uncompleted) 
    {
        tmpproof = aggreport.proofs.back();
        aggreport.uids.erase(aggreport.uids.begin() + a, aggreport.uids.end());
        aggreport.counters.erase(aggreport.counters.begin() + a, aggreport.counters.end());
        aggreport.size -= uncompleted;
        aggreport.proofs.pop_back();
    }


    // add finalized proof aggregates
    size_t reportsize = (size_t) upmsg->getReport1ArraySize();
    for(size_t i=0; i < reportsize; ++i)
    {
        aggreport.uids.push_back((uid_t)upmsg->getReport1(i));
        aggreport.counters.push_back((size_t)upmsg->getReport2(i));

        if((uid_t)upmsg->getReport1(i) == NOID)
            aggreport.sepsize++;
        else
            aggreport.size++;
    }
    size_t proofsize = (size_t) upmsg->getProofArraySize();

    for(size_t j=0 ; j < proofsize; ++j)
    {
        aggreport.proofs.push_back((size_t)upmsg->getProof(j));
    }

    // aggregate unfinalized proof aggregates
    if(uncompleted)
    {
        a = aggreport.uids.size();
        uncompleted = 0;
        while (a && aggreport.uids[a-1] != NOID)
        {
            --a;
            ++uncompleted;
        }

        size_t r = uncompleted % aggsize;
        if(r + tmpuids.size() <= aggsize)
        {
            aggreport.uids.insert(aggreport.uids.end(), tmpuids.begin(), tmpuids.end());
            aggreport.counters.insert(aggreport.counters.end(), tmpcounters.begin(), tmpcounters.end());
            aggreport.size += tmpuids.size();
            if(r)
                aggreport.proofs.back() ^= tmpproof;
            else
                aggreport.proofs.push_back(tmpproof);
            fixProofs();
        }
        else
        {
            aggreport.uids.push_back(NOID);
            aggreport.counters.push_back(0);
            aggreport.sepsize++;
            aggreport.uids.insert(aggreport.uids.end(), tmpuids.begin(), tmpuids.end());
            aggreport.counters.insert(aggreport.counters.end(), tmpcounters.begin(), tmpcounters.end());
            aggreport.size += tmpuids.size();
            aggreport.proofs.push_back(tmpproof);
        }
    }


    if(csessions[senderid]->tomsg && csessions[senderid]->tomsg->isScheduled())
        cancelAndDelete(csessions[senderid]->tomsg);

    if(csessions[senderid])
        delete csessions[senderid];
    csessions.erase(senderid);
#ifdef TREE
    --gatsz;
    if(csessions.size() == 1 &&  gatsz == MINGSIZE)
        sendUpReq(tid);
#else
    if(csessions.size() == 1)
        sendUpReq(tid);
#endif
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
             << ", UID=" <<  UId << "]: " << dec<< m << endl;
}

void
AProver::logInfo(string m)
{
    EV << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=" <<  UId << "]: " << dec<< m << endl;
}

void
AProver::logWarn(string m)
{
    EV_WARN << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=" <<  UId << "]: " << dec<< m << endl;
}


void
AProver::logError(string m)
{
    ostringstream oss;
    oss << "Prover [Index=" << getParentModule()->getIndex()
        << ", UID=" <<  UId << "]: " << dec<< m << endl;
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

#ifdef RUNTIME_TEST
    if(getParentModule()->getIndex() == rootidx)
    {
        startmsg->setKind(MKTREE);
        scheduleAt(simTime(), startmsg);

        if(mktreetimer->isScheduled())
            cancelEvent(mktreetimer);
    }
#endif
}


void AProver::addOwnReport()
{
    mac_t mac = 0; // compute mac of: UID || counter || TID

    
    int a = aggreport.uids.size() % aggsize ;
    if(a == 0)
    {
        aggreport.proofs.push_back(mac);
    }
    else 
    {
        aggreport.proofs.back() ^= mac;
    }
    
    aggreport.uids.push_back(UId);
    aggreport.counters.push_back(++attestcount);
    aggreport.size++;
    fixProofs();
}

void
AProver::fixProofs()
{
    int a = aggreport.uids.size();
    size_t s = 0;
    while(a && aggreport.uids[a-1] != NOID)
    {
        --a; ++s;
    }
    if(s == aggsize)
    {
        aggreport.uids.push_back(NOID);
        aggreport.counters.push_back(0);
        aggreport.sepsize++;
    }
}

void
AProver::finalizeProofs()
{
    if(aggreport.uids.size() % aggsize > (size_t)floor(aggsize / 2))
    {
        aggreport.proofs[aggreport.size-2] ^= aggreport.proofs.back();
        aggreport.proofs.pop_back();
    }
}

#ifdef ENERGY_TEST
void
AProver::updateMaxChildren()
{

    double per = inet::power::unit(energy->getResidualEnergyCapacity() / energy->getNominalEnergyCapacity()).get();
    double v = CHILDREN(per);
    v = v<1?0:v;
    maxchildren = (int)ceil(v);
}


double
AProver::chooseTreeDelay()
{

    double per = inet::power::unit(energy->getResidualEnergyCapacity() / energy->getNominalEnergyCapacity()).get();
    double v = DELTAG(per,deltag);
    v = v<0?0:v;
    return v;
}
#endif


void 
AProver::finish()
{
        cancelAndDelete(startmsg);
        cancelAndDelete(jointomsg);
        cancelAndDelete(mktreetimer);
        cancelAndDelete(attesttimer);
        cancelAndDelete(checkdelaymsg);
        cancelAndDelete(cresptomsg);
        cancelAndDelete(cacktomsg);
        cancelAndDelete(creqmsg);
        cancelAndDelete(txmsg);
        cancelAndDelete(entxmsg);
        cancelAndDelete(cbusymsg);

}
