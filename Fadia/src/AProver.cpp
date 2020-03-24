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
    EV << "Initializing prover: " << getParentModule()->getIndex() << endl;
    if (getParentModule()->getIndex() == 0)
    {
        cMessage* msg = new cMessage("new");
        scheduleAt(simTime() + uniform(0,1), msg);
    }
    if (DEBUG)
    {
        for (KEYRNG_IT it = KeyRing.begin(); it != KeyRing.end(); ++it)
        {
            EV << "0x" << setfill('0') << setw(8) << hex << it->first << " " << it->second  << endl;
        }
    }
}

void
AProver::handleMessage(cMessage *msg)
{
    const char* msgname = msg->getName();

    if(!strncmp(msgname, "new", 4))
    {
        EV << "Prover " << getParentModule()->getIndex() << " performing Neighbor discovery" << endl;
        sendNDReq();
    }

    if(!strncmp(msgname,"ND", 2))
        handleNDMsg(msg);

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
AProver::handleNDMsg(cMessage* msg)
{
    char msgtype[3];
    strncpy(msgtype, msg->getName()+2, 2);

    static KEYID kid = NOID;

    if(!strcmp(msgtype, "RQ"))
        handleNDReq(msg);
    else if(!strcmp(msgtype, "RP"))
        handleNDResp(msg);
    else if(!strcmp(msgtype, "AK"))
    {
        handleNDAck(msg);
        kid = NOID;
    }
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
