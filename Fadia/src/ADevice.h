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


typedef unsigned int TREEID;
typedef int CHLNG;
typedef int SOLV;
typedef map<UID, KEYID> NTBL;
typedef map<UID, pair<KEYID, CHLNG>> SEST;



KEYRNG getKeyRing(cRNG* rng, size_t KPsize, size_t KRsize);
const UID getBaseID();

enum MSG : short  
{ 
    UNKOWN,
    JOIN,
    MKTREE, 
    REVOKE, 
    SYNC, 
    JNRQ, 
    JNRP, 
    JNAK, 
    CMRQ, 
    CMRP, 
    CMAK, 
    UP, 
    RVK,
    CMRPTO,
    CMAKTO,
    CMR
};

class ADevice : public cSimpleModule
{

protected:
    size_t KRsize;
    size_t KPsize;
    static NetworkOwner NO;
    static const int baseID;
    double range;

    // Inits
    virtual void initUID() = 0;
    virtual void initKeyRing() = 0;

    // Join
    virtual void handleJoinMsg(cMessage* msg);
    virtual void handleJoinReq(cMessage* msg) = 0;
    virtual void handleJoinResp(cMessage* msg) = 0;
    virtual void handleJoinAck(cMessage* msg) = 0;
    virtual void sendJoinReq() = 0;
    virtual void sendJoinResp(UID target, double battery) = 0;
    virtual void sendJoinAck(UID target) = 0;


    // Make Spanning Tree
    virtual void handleCommitMsg(cMessage* msg);
    virtual void handleCommitReq(cMessage* msg) = 0;
    virtual void handleCommitResp(cMessage* msg) = 0;
    virtual void handleCommitAck(cMessage* msg) = 0;
    virtual void sendCommitReq(TREEID tid) = 0;
    virtual void sendCommitResp(UID target, KEYID kid, TREEID tid) = 0;
    virtual void sendCommitAck(UID target, KEYID kid, TREEID tid) = 0;

    // // Attest
    // virtual void handleAttMsg(cMessage* msg) = 0;
    // virtual void sendPullAttReq() = 0;
    // virtual void handlePullAttReq(cMessage* msg) = 0;
    // virtual void sendAttReq(UID target, KEYID kid) = 0;
    // virtual void handleAttReq(cMessage* msg) = 0;
    // virtual void sendAttResp(UID target) = 0;
    // virtual void handleAttResp(cMessage* msg) = 0;
    // virtual void sendAttAck(UID target) = 0;
    // virtual void handleAttAck(cMessage* msg) = 0;
    // virtual void handlePTimeOut(cMessage* msg) = 0;
    // virtual void handleVTimeOut(cMessage* msg) = 0;

    // Update
    virtual void handleUpMsg(cMessage* msg);
    virtual void handleUpReq(cMessage* msg) = 0;
    virtual void sendUpReq() = 0;

    // Revoke
    virtual void handleRevMsg(cMessage* msg);
    virtual void handleRevReq(cMessage* msg) = 0;
    virtual void sendRevReq(UID comdev) = 0;

    // Sync
    virtual void handleSyncMsg(cMessage* msg);
    virtual void sendSyncReq() = 0;
    virtual void handleSyncReq(cMessage* msg) = 0;

    // utilities
    virtual void logInfo(string m) = 0;
    virtual void logDebug(string m) = 0;
    virtual  void logError(string m) = 0;
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
    virtual void handleMessage(cMessage *msg) override;

};

#endif /* ADEVICE_H_ */
