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


#include "AProver.h"

#include "AProver.h"
#include "NeighborDiscovery_m.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"
#include "Commit_m.h"

#include "../inet/src/inet/power/contract/IEpEnergyStorage.h"

#define DEBUG 1

#define Bubble(_text_) getParentModule()->bubble(_text_)


session_t::session_t(TREEID t, UID uid, KEYID k, unsigned int d, bool v = false)
{
    treeID = t; 
    deviceID = uid;
    kid = k;
    depth = d;
    v = false;
}


// The module class needs to be registered with OMNeT++
Define_Module(AProver);


void
AProver::initialize()
{
    initNO();
    initUID();
    initKeyRing();
    cMessage* msg = new cMessage("Join", JOIN);
    scheduleAt(simTime() + uniform(0,1), msg);
    ptime = ((unsigned int) getSystemModule()->par("htime")) / 2;
    vtime = ((unsigned int) getSystemModule()->par("htime")) / 2;
    range = ((double) getParentModule()->par("range"));
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
        UId = (getBaseID<UID>() + getParentModule()->getIndex()) % MAXUID;
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
    // char msgname[6];
    cid = findClosestCollector();
    if (cid == NOID)
        return;


    // sprintf(msgname, "JNRQ");
    JoinReq* msg = new JoinReq();
    msg->setKind(JNRQ);
    msg->setSource(UId);
    msg->setDestination(cid);
    msg->setBattery(getBatteryLevel());
    msg->setMac(generateMAC(msg, ckey));
    sendCollector(cid, msg);
}

void
AProver::handleJoinResp(cMessage* msg)
{
    JoinResp* resmsg = check_and_cast<JoinResp *>(msg);
    if((UID)resmsg->getDestination() != UId
        || (CID)resmsg->getSource() != cid)
    {
        logError("handleJoinResp: received message with wrong src/dest");
        return;
    }

    UID senderid = (UID) resmsg->getSource();

    if(checkMAC<JoinResp>(resmsg))
    {
        prove = (unsigned int) resmsg->getProve() ;
        verify = (unsigned int) resmsg->getVerify() * ((unsigned int)getParentModule()->getIndex() % 2);
    }
    sendJoinAck(senderid);

    // cMessage* pmsg = new cMessage("prove");
    // cMessage* vmsg = new cMessage("verify");

    // scheduleAt(simTime(), vmsg);
    // scheduleAt(simTime(), pmsg);
}

void
AProver::sendJoinAck(UID target)
{
    // char msgname[5];
    // sprintf(msgname, "JNAK");
    JoinAck* msg = new JoinAck();
    msg->setKind(JNAK);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setMac(generateMAC(msg, ckey));
    sendCollector(target, msg);

}

void
AProver::handleCommitReq(cMessage* msg)
{
    if (pcounter >= prove)
        return;

    CommitReq* rmsg = check_and_cast<CommitReq *>(msg); 
    UID senderid = (UID) rms->getSource();
    TREEID tid = (TREEID) rms->getTreeID();
    
    if(psessions_bytree.find(tid) != psessions_bytree.end()
        && psessions_bydev.find(senderid) != psessions_bydev.end())
        return;



    size_t kidsize = (size_t) reqmsg->getKidArraySize();

    for(size_t i=0; i < kidsize; ++i)
    {
        KEYRNG_IT it = KeyRing.find((KEYID)reqmsg->getKid(i));
        if( it != KeyRing.end())
        {
            session_t pses(tid, senderid, it->first, depth);
            CommitTimeout* tomsg = new CommitTimeout();
            tomsg->setKind(CMRPTO);
            scheduleAt(simTime() + timeout, tomsg);
            pses.tomsg = tomsg;
            psessions_bydev[senderid] = pses;
            psessions_bytree[tid] = pses;
            sendCommitResp(senderid, tid);
            return;
        }
    }
    return;
}
void
AProver::sendCommitReq(TREEID tid)
{
    CommitReq* msg = new CommitReq();
    msg->setKind(CMRQ);
    msg->setSource(UId);
    msg->setKidArraySize(KeyRing.size());
    size_t i = 0;
    for(KEYRNG_IT it = KeyRing.begin(); it != KeyRing.end(); ++it)
    {
        msg->setKid(i++, it->first);
    }
    msg->setDepth(getDepth());
    msg->setTreeID(tid);
    sendProverBroadcast(msg);
}
void
AProver::handleCommitResp()
{
 
}
void
AProver::sendCommitResp(UID target, TREEID tid)
{

}

void
AProver::handleCommitAck()
{

}

void
AProver::sendCommitAck(UID target, KEYID kid, TREEID tid)
{

}

void
AProver::sendPullAttReq()
{
    char msgname[6];
    sprintf(msgname, "ATTPR");
    PullAttestReq* msg = new PullAttestReq(msgname);
    msg->setSource(UId);

}

void
AProver::handlePullAttReq(cMessage* msg)
{
    if(vcounter >= verify)
        return;
    if(vcounter + vsessions.size() >= verify)
    {
        postponeMsg(msg);
        return;
    }

    PullAttestReq* reqmsg = check_and_cast<PullAttestReq *>(msg);
    UID senderid = (UID) reqmsg->getSource();

    if(vsessions.find(senderid) != vsessions.end())
        return;

    size_t kidsize = (size_t) reqmsg->getKidArraySize();

    for(size_t i=0; i < kidsize; ++i)
    {
        KEYRNG_IT it = KeyRing.find((KEYID)reqmsg->getKid(i));
        if( it != KeyRing.end())
        {
            sendAttReq(senderid, it->first);
            return;
        }
    }
    return;

}

void
AProver::sendAttReq(UID target, KEYID kid)
{
    char msgname[6];
    sprintf(msgname, "ATTRQ");
    AttestReq* msg = new AttestReq(msgname);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setKid(kid);
    CHLNG c = generateChallenge();
    vsessions[target] = make_pair(kid, c);
    msg->setChallenge(c);
    msg->setMac(generateMAC(msg,kid));
    sendProver(target, msg);
    AttestTimeOut* tmsg = new AttestTimeOut("ATTVTO");
    tmsg->setCorrespondent(target);
    scheduleAt(simTime() + timeout, tmsg);
}

void
AProver::handleAttReq(cMessage* msg)
{
    if(pcounter >= prove)
        return;
    else if (pcounter + psessions.size() >= prove)
    {
        postponeMsg(msg);
        return;
    }

    AttestReq* reqmsg = check_and_cast<AttestReq *>(msg);
    if((UID)reqmsg->getDestination() != UId)
    {
            logError("handleAttReq: received message with wrong destination");
            return;
    }

    UID senderid = (UID) reqmsg->getSource();
    if(psessions.find(senderid) != psessions.end())
        return;


    KEYID kid = (KEYID) reqmsg->getKid();
    if(KeyRing.find(kid) == KeyRing.end())
    {
        logError("handleAttReq: received a bad keyid");
        return;
    }

    if(!checkMAC<AttestReq>(reqmsg))
    {
        logDebug("received bad MAC");
        return;
    }


    if(pcounter <  prove)
    {
        CHLNG c = reqmsg->getChallenge();
        psessions[senderid] = make_pair(kid, c);
        sendAttResp(senderid);
    }

}

void
AProver::sendAttResp(UID target)
{
    char msgname[6];
    sprintf(msgname, "ATTRP");
    AttestResp* msg = new AttestResp(msgname);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setResponse(solveChallenge(target));
    msg->setMac(generateMAC(msg,psessions[target].first));
    sendProver(target, msg);
    AttestTimeOut* tmsg = new AttestTimeOut("ATTPTO");
    tmsg->setCorrespondent(target);
    scheduleAt(simTime() + timeout, tmsg);
}

void
AProver::handleAttResp(cMessage* msg)
{
    if(vcounter >= verify)
        return;

    AttestResp* resmsg = check_and_cast<AttestResp *>(msg);
    if((UID)resmsg->getDestination() != UId)
    {
            logError("handleNDResp: received message with wrong destination");
            return;
    }

    UID senderid = (UID) resmsg->getSource();
    if (vsessions.find(senderid) == vsessions.end())
    {
        logInfo("handleAttResp: session is already closed with this target");
    }

    if(!checkMAC<AttestResp>(resmsg))
    {
        logError("handleAttResp: Invalid MAC");
        return;
    }

    if (solveChallenge(senderid) == resmsg->getResponse())
    {
        logInfo("Prover " + to_string(senderid) + " is GOOD");
        goodDevices.push_back(senderid);
    }
    else
    {
        logInfo("Prover " + to_string(senderid) + " is BAD");
        badDevices.push_back(senderid);
    }

    sendAttAck(senderid);
    if(++vcounter >= verify)
    {
        sendUpReq();
    }

}

void
AProver::sendAttAck(UID target)
{
    char msgname[6];
    sprintf(msgname, "ATTAK");
    AttestAck* msg = new AttestAck(msgname);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setMac(generateMAC(msg,vsessions[target].first));
    sendProver(target, msg);
    vsessions.erase(target);
}

void
AProver::handleAttAck(cMessage* msg)
{
    AttestAck* ackmsg = check_and_cast<AttestAck *>(msg);
    if((UID)ackmsg->getDestination() != UId)
    {
            logError("handleNDAck: received message with wrong destination");
            return;
    }

    UID senderid = (UID) ackmsg->getSource();
    if(!checkMAC<AttestAck>(ackmsg))
    {
        logDebug("received bad MAC");
        return;
    }
    psessions.erase(senderid);
    if(++pcounter >= prove)
        Bubble("finished attesting requirements");
}

void AProver::handlePTimeOut(cMessage* msg)
{
    AttestTimeOut* tmsg = check_and_cast<AttestTimeOut *>(msg);
    UID cor = tmsg->getCorrespondent();
    if(psessions.find(cor) != psessions.end())
        psessions.erase(cor);
}

void AProver::handleVTimeOut(cMessage* msg)
{
    AttestTimeOut* tmsg = check_and_cast<AttestTimeOut *>(msg);
    UID cor = tmsg->getCorrespondent();
    if(vsessions.find(cor) != vsessions.end())
        vsessions.erase(cor);
}

void
AProver::sendUpReq()
{
    Bubble("sending my report to the collector");
    char msgname[5];
    sprintf(msgname, "UPRQ");
    UpdateReq* msg = new UpdateReq(msgname);
    msg->setSource(UId);
    // msg->setDestination(cid);
    msg->setMac(generateMAC(msg,ckey));
    sendCollector(cid, msg);
}

void
AProver::handleRevReq(cMessage* msg)
{

}

CHLNG
AProver::generateChallenge()
{
    return intrand(INT32_MAX);
}

SOLV
AProver::solveChallenge(UID target)
{
//    KEY key = KeyRing[sessions[target].first];
//    CHLNG c = sessions[target].second;
    return 0;
}


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
AProver::logError(string m)
{
    ostringstream oss;
    oss << "Prover [Index=" << getParentModule()->getIndex()
        << ", UID=0x" << hex <<  UId << "]: " << dec<< m << endl;
    EV_ERROR << oss.str();
    getSimulation()->getActiveEnvir()->alert(oss.str().c_str());

}

const CID
AProver::findClosestCollector()
{
    return getBaseID<CID>();

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


void AProver::refreshDisplay() const
{
    char buf[60];
    sprintf(buf, "prover: %ld/%ld verify: %ld/%ld", pcounter, prove, vcounter, verify);
    getParentModule()->getDisplayString().setTagArg("t", 0, buf);
}


void AProver::postponeMsg(cMessage* msg)
{
    scheduleAt(simTime() + postponetime, msg);
}
