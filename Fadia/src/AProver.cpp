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
#include "common.h"
#include "NeighborDiscovery_m.h"

#define DEBUG 1





// The module class needs to be registered with OMNeT++
Define_Module(AProver);


//
//AProver::AProver()
//{
//    initUID();
//    initKeyRing();
//}


void
AProver::initialize()
{
    initUID();
    initKeyRing();

    if (getParentModule()->getIndex() == 0)
    {
        cMessage* msg = new cMessage("new");
        scheduleAt(simTime() + uniform(0,1), msg);
    }
//    if (DEBUG)
//    {
//        for (KEYRNG_IT it = KeyRing.begin(); it != KeyRing.end(); ++it)
//        {
//            EV << "0x" << setfill('0') << setw(8) << hex << it->first << " " << it->second  << endl;
//        }
//    }
}

void
AProver::handleMessage(cMessage *msg)
{
    const char* msgname = msg->getName();

    if(!strncmp(msgname, "new", 4) && false)
    {
        logInfo("Performing Neighbor discovery");
        sendNDReq();
    }


    else if(!strncmp(msgname, "JN", 2))
    {
        logDebug("Received a join message");
        handleJoinMsg(msg);
    }

    else if(!strncmp(msgname, "ND", 2))
    {
        logDebug("Received a neighbor discovery message");
        handleNDMsg(msg);
    }


    else if(!strncmp(msgname, "ATT", 3))
    {
        logDebug("Received an attestation message");
        handleAttMsg(msg);
    }

    else if(!strncmp(msgname, "RVK", 3))
    {
        logDebug("Received a revocation message");
        handleRevMsg(msg);
    }

}


void
AProver::initUID()
{
    do
    {
        UId = (::getBaseID() + getParentModule()->getIndex()) % MAXUID;
    } while(!UId);
}


void
AProver::initKeyRing()
{
    KRsize = getSystemModule()->par("keyRingSize");
    KPsize = getSystemModule()->par("keyPoolSize");
    cRNG* rng = getRNG(seed);
    KeyRing = ::getKeyRing(rng, KPsize, KRsize);
}


void
AProver::handleJoinMsg(cMessage* msg)
{
    char msgtype[3];
    strncpy(msgtype, msg->getName()+2, 2);

    if(!strcmp(msgtype, "RP"))
        handleJoinResp(msg);

    else if(!strcmp(msgtype, "RQ"))
        logError("Received a join request message!!!! This should never happen");

    else if(!strcmp(msgtype, "AK"))
        logError("Received a join Ack message!!!! This should never happen");

    else
        logError("Received a join message of unknown type!!!");
}

void
AProver::handleNDMsg(cMessage* msg)
{
    char msgtype[3];
    strncpy(msgtype, msg->getName()+2, 2);

    if(!strcmp(msgtype, "RQ"))
        handleNDReq(msg);

    else if(!strcmp(msgtype, "RP"))
        handleNDResp(msg);

    else if(!strcmp(msgtype, "AK"))
        handleNDAck(msg);

    else
        logError("Received a ND message of unknown type!!!");
}

void
AProver::handleAttMsg(cMessage* msg)
{
    char msgtype[3];
    strncpy(msgtype, msg->getName()+3, 2);

    if(!strcmp(msgtype, "PR"))
        handlePullAttReq(msg);

    else if(!strcmp(msgtype, "RQ"))
        handleAttReq(msg);

    else if(!strcmp(msgtype, "RP"))
        handleAttResp(msg);

    else if(!strcmp(msgtype, "AK"))
        handleAttAck(msg);

    else
        logError("Received an attest message of unknown type!!!");
}

void
AProver::handleRevMsg(cMessage* msg)
{
    char msgtype[3];
    strncpy(msgtype, msg->getName()+3, 2);

    if(!strcmp(msgtype, "RQ"))
        handleRevReq(msg);

    else
        logError("Received a Revocation message of unknown type!!!");
}

void
AProver::sendPullAttReq()
{

}


void
AProver::handlePullAttReq(cMessage* msg)
{

}


void
AProver::sendJoinReq()
{

}

void
AProver::sendNDReq()
{
    char msgname[6];
    sprintf(msgname, "NDRQ");
    NDiscoverReq* msg = new NDiscoverReq(msgname);
    msg->setSource(UId);
    msg->setKidArraySize(KeyRing.size());
    size_t i = 0;
    for(KEYRNG_IT it = KeyRing.begin(); it != KeyRing.end(); ++it)
    {
        msg->setKid(i++, it->first);
    }
    sendProverBroadcast(msg);


}


void
AProver::sendAttReq(UID target, KEYID kid)
{

}


void
AProver::handleNDReq(cMessage* msg)
{
    NDiscoverReq* reqmsg = check_and_cast<NDiscoverReq *>(msg);
    UID senderid = (UID) reqmsg->getSource();

    if(NTable.find(senderid) != NTable.end())
        return;

    size_t kidsize = (size_t) reqmsg->getKidArraySize();
    for(int i=0; i < kidsize; ++i)
    {
        KEYRNG_IT it = KeyRing.find((KEYID)reqmsg->getKid(i));
        if( it != KeyRing.end())
        {
            sendNDResp(senderid, it->first);
            NTableTmp[senderid] = it->first;
            return;
        }
    }
    return;
}

void
AProver::handleAttReq(cMessage* msg)
{

}

void
AProver::handleRevReq(cMessage* msg)
{

}


void AProver::sendNDResp(UID target, KEYID kid)
{
    char msgname[5];
    sprintf(msgname, "NDRP");
    NDiscoverResp* msg = new NDiscoverResp(msgname);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setKid(kid);
    msg->setMac(generateMAC(msg));
    sendProver(target, msg);
}

void AProver::sendAttResp(UID target, KEYID kid)
{

}

void
AProver::handleJoinResp(cMessage* msg)
{

}

void
AProver::handleNDResp(cMessage* msg)
{
    NDiscoverResp* resmsg = check_and_cast<NDiscoverResp *>(msg);
    if(resmsg->getDestination() != UId)
        return;
    UID senderid = (UID) resmsg->getSource();
    KEYID kid = (KEYID) resmsg->getKid();
    if(checkMAC<NDiscoverResp>(resmsg))
    {
        addNeighbor(senderid);
    }
    sendNDAck(senderid);
}

void
AProver::handleAttResp(cMessage* msg)
{

}

void
AProver::sendJoinAck(UID target)
{

}

void
AProver::sendNDAck(UID target)
{
    char msgname[5];
    sprintf(msgname, "NDAK");
    NDiscoverAck* msg = new NDiscoverAck(msgname);
    msg->setSource(UId);
    msg->setDestination(target);
    msg->setMac(generateMAC(msg));
    sendProver(target, msg);
}

void
AProver::sendAttAck(UID target)
{

}

void
AProver::handleNDAck(cMessage* msg)
{
    NDiscoverAck* ackmsg = check_and_cast<NDiscoverAck *>(msg);
    if(ackmsg->getDestination() != UId)
        return;
    UID senderid = (UID) ackmsg->getSource();
    if(checkMAC<NDiscoverAck>(ackmsg))
    {
        addNeighbor(senderid);
    }
}


void
AProver::handleAttAck(cMessage* msg)
{

}

//template<class T, class U>
//T* getIds(U list)
//{
//    T val, arr[list.size()];
//    size_t i = 0;
//    for(auto it = list.begin(); it != list().end(); ++it)
//    {
//        if(!is_same<U, vector>::value)
//            val = (T) it->first;
//        else
//            val = (T) *it;
//
//        arr[i++] = val;
//    }
//    return arr;
//}


//template <typename FUNCTION>
//inline void
//AProver::forEachKey(FUNCTION f)
//{
//    for(KEYRNG_IT it = KeyRing.begin();
//            it != KeyRing.end(); ++it)
//        f(it->first, it->second);
//}
//
//void
//AProver::fillKeyIds(KID kid, KEY key)
//{
//
//}
//

void
AProver::sendProver(UID uid, cMessage* msg)
{
    size_t indx = ( uid - ::getBaseID()) % MAXUID;
    cModule* mod = getSystemModule()->getSubmodule("prover", indx);
    cMessage* msgd = msg->dup();
    sendDirect(msgd, 0.1, 0, mod, "radioIn");
}

void
AProver::sendProverBroadcast(cMessage* msg)
{
    int size = getParentModule()->getVectorSize();
    assert(size);
    for(int i = 0; i < (size_t)size; ++i)
    {
        if((int)i != getParentModule()->getIndex())
        {
            cModule* mod = getSystemModule()->getSubmodule("prover", i);
            cMessage* msgd = msg->dup();
            sendDirect(msgd, 0.1, 0, mod, "radioIn");
        }
    }
}

int
AProver::generateMAC(cMessage* msg)
{
    return 0;
}

template <class T>
bool
AProver::checkMAC(T* msg)
{
    return msg->getMac() == 0;
}

void
AProver::addNeighbor(UID uid)
{
    if(NTableTmp.find(uid) == NTableTmp.end())
        return;

    NTable[uid] = NTableTmp[uid];
    NTableTmp.erase(uid);
}


void
AProver::logDebug(string m)
{
    EV_DEBUG << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=" << hex <<  UId << "]: " << dec<< m << endl;
}

void
AProver::logInfo(string m)
{
    EV << "Prover [Index=" << getParentModule()->getIndex()
             << ", UID=" << hex <<  UId << "]: " << dec<< m << endl;
}


void
AProver::logError(string m)
{
    ostringstream oss;
    oss << "Prover [Index=" << getParentModule()->getIndex()
        << ", UID=" << hex <<  UId << "]: " << dec<< m << endl;
    EV_ERROR << oss.str();
    getSimulation()->getActiveEnvir()->alert(oss.str().c_str());

}

