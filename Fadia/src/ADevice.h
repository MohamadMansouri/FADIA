/*
 * common.h
 *
 *  Created on: Mar 19, 2020
 *      Author: mmrota
 */

#ifndef ADEVICE_H_
#define ADEVICE_H_

#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <utility>
#include <vector>

#include "NetworkOwner.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"


using namespace omnetpp;

typedef int CHLNG;
typedef int SOLV;
typedef map<UID, KEYID> NTBL;
typedef map<UID, pair<KEYID, CHLNG>> SEST;



KEYRNG getKeyRing(cRNG* rng, size_t KPsize, size_t KRsize);
const UID getBaseID();


class ADevice : public cSimpleModule
{

protected:
    size_t KRsize;
    size_t KPsize;
    static NetworkOwner NO;
    static const int baseID;

    // Inits
    virtual void initUID() {};
    virtual void initKeyRing() {};

    // Core
    void Join() {};
    void NeighborDiscover()  {};
    void Prove() {};
    void Verify() {};
    void Revoke() {};

    // Join
    virtual void handleJoinMsg(cMessage* msg) {};
    virtual void sendJoinReq() {};
    virtual void handleJoinReq(cMessage* msg) {};
    virtual void sendJoinResp(UID target, double batttery) {};
    virtual void handleJoinResp(cMessage* msg) {};
    virtual void sendJoinAck(UID target) {};
    virtual void handleJoinAck(cMessage* msg) {};


    // ND ( DEPRECIATED )
    virtual void handleNDMsg(cMessage* msg) {};
    virtual void sendNDReq() {};
    virtual void handleNDReq(cMessage* msg) {};
    virtual void sendNDResp(UID target, KEYID kid) {};
    virtual void handleNDResp(cMessage* msg) {};
    virtual void sendNDAck(UID target, KEYID kid) {};
    virtual void handleNDAck(cMessage* msg) {};
    virtual void addNeighbor(UID uid) {};

    // Attest
    virtual void handleAttMsg(cMessage* msg) {};
    virtual void sendPullAttReq() {};
    virtual void handlePullAttReq(cMessage* msg) {};
    virtual void sendAttReq(UID target, KEYID kid) {};
    virtual void handleAttReq(cMessage* msg) {};
    virtual void sendAttResp(UID target) {};
    virtual void handleAttResp(cMessage* msg) {};
    virtual void sendAttAck(UID target) {};
    virtual void handleAttAck(cMessage* msg) {};
    virtual void handlePTimeOut(cMessage* msg) {};
    virtual void handleVTimeOut(cMessage* msg) {};
    // Update
    virtual void handleUpMsg(cMessage* msg) {};
    virtual void sendUpReq(CID target) {};
    virtual void handleUpReq(cMessage* msg) {};

    // Revoke
    virtual void handleRevMsg(cMessage* msg) {};
    virtual void sendRevReq(UID comdev) {};
    virtual void handleRevReq(cMessage* msg) {};

    // Revoke
    virtual void handleSyncMsg(cMessage* msg) {};
    virtual void sendSyncReq() {};
    virtual void handleSyncReq(cMessage* msg) {};

    // utilities
    template<class T, class U> T* getIds(U list);
    void sendProver(UID target, cMessage* msg);                     // This method does not consume the message ;)
    void sendProverBroadcast(cMessage* msg);                        // This method does not consume the message ;)
    void sendCollector(CID target, cMessage* msg);                  // This method does not consume the message ;)
    void sendCollectorBroadcast(CID target, cMessage* msg);         // This method does not consume the message ;)
    int generateMAC(cMessage* msg,KEYID kid);
    template <class T> bool checkMAC(T* msg);
    template <typename T> const T getBaseID();

public:
    ADevice() {}
    ~ADevice() {}
};

#endif /* ADEVICE_H_ */
