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
#include <stack>
#include <queue>

#include "NetworkOwner.h"
#include "Join_m.h"
#include "Attest_m.h"
#include "Update_m.h"
#include "Revoke_m.h"
#include "constants.h"

#include "../inet/src/inet/power/contract/IEpEnergyStorage.h"
#include "../inet/src/inet/power/storage/SimpleEpEnergyStorage.h"

using namespace omnetpp;

// typedef int CHLNG;
// typedef int SOLV;
// typedef map<uid_t, KEYID> NTBL;
// typedef map<uid_t, pair<KEYID, CHLNG>> SEST;


enum MSG : short  
{ 
    UNKOWN,
    JOIN,
    CHECK,
    ATTEST,
    MKTREE, 
    REVOKE, 
    SYNC,
    UPDATE, 
    JNRQ, 
    JNRP, 
    JNAK, 
    CMRQ, 
    CMRP, 
    CMAK, 
    UPRQ, 
    UPRQC, 
    UPRQF,
    UPTO, 
    RVKRQ,
    CMRPTO,
    CMAKTO,
    JNRPTO,
    JNAKTO,
    TXDONE,
    CBUSYMSG,
    ENTXDONE, 
    ENRXDONE, 
    ENMAC,
};

enum txrx_e : short
{
    TRANSMITING, 
    DIDLE
};


enum cstat_e : short
{
    BUSY, 
    CIDLE
};


enum device_t : short
{
    NA,
    SERV, 
    SKY, 
    PI2
};

enum status_e
{
    OFFLINE,
    JOINING,
    CHECKING,
    READY, 
    ATTESTING,
    ATTESTING_CREATING,
    CREATING,
    COLLECTING,
    FINISHED
};


class ADevice : public cSimpleModule
{

protected:
    // configuration parameters
    const int seed = 0; 
    const double ndelay = NDELAY;  
    const double postponetime = PDELAY;
    const double timeoutresp = TIMEOUT_RSP;
    const double timeoutack = TIMEOUT_ACK;
    const double timeoutup = TIMEOUT_UP;
    const double byterate = BYTERATE;
    device_t device = NA;
    size_t maxdepth; 
    size_t maxchildren = MAXCHILDREN; 
    double deltah = DELTAH; 
    double deltag;
    size_t aggsize;
    bool crashed = false;
#ifdef RUNTIME_TEST
    size_t gatsz = 0;
#endif    
#ifdef ENERGY_TEST    
    bool statadapt;
#endif
    double range;

    // device status
    status_e status = OFFLINE;

#ifdef ENERGY_TEST
    inet::power::SimpleEpEnergyStorage* energy;
#endif    

#ifdef WIRELESS 
    int drange = -1;
#else
    int drange = NEIGHBOR_DISTANCE;
#endif
    int rootidx = 0;
    // bool ignoredepth = (SCENARIO == PASTA);
    

    static size_t countcrash;

    size_t KRsize;
    size_t KPsize;
    static NetworkOwner NO;
    static const int baseID;

    queue<double> macdelays;

    map<uid_t, int> deviceg;

    simsignal_t txsig;
    simsignal_t rxsig;
    simsignal_t crashsig;
    simsignal_t revokesig;

    txrx_e txrxstat = DIDLE;
    cstat_e chanstat = CIDLE;
    cMessage* txmsg = new cMessage("txdone", TXDONE);
    cMessage* cbusymsg = new cMessage("channelBusy", CBUSYMSG);
    cMessage* entxmsg = new cMessage("EnergyTransmitionDone", ENTXDONE);

    queue<cMessage*> msgqueue;


    // Inits
    virtual void initUID() = 0;
    virtual void initKeyRing() = 0;

    // Join
    virtual void handleJoinMsg(cMessage* msg);
    virtual void handleJoinReq(cMessage* msg) = 0;
    virtual void handleJoinResp(cMessage* msg) = 0;
    virtual void handleJoinAck(cMessage* msg) = 0;
    virtual void sendJoinReq() = 0;
    virtual void sendJoinResp(uid_t target) = 0;
    virtual void sendJoinAck(uid_t target) = 0;
    virtual void handleJoinRespTimeOut(cMessage* msg) = 0;
    virtual void handleJoinAckTimeOut(cMessage* msg) = 0;

    // Make Spanning Tree
    virtual void startAttestation() = 0;
    virtual void handleCommitMsg(cMessage* msg);
    virtual void handleCommitReq(cMessage* msg) = 0;
    virtual void handleCommitResp(cMessage* msg) = 0;
    virtual void handleCommitAck(cMessage* msg) = 0;
    virtual void sendCommitReq(treeid_t tid) = 0;
    virtual void sendCommitResp(uid_t target, treeid_t tid) = 0;
    virtual void sendCommitAck(uid_t target, treeid_t tid) = 0;
    virtual void handleCommitRespTimeOut(cMessage* msg) = 0;
    virtual void handleCommitAckTimeOut(cMessage* msg) = 0;

    // // Attest
    // virtual void handleAttMsg(cMessage* msg) = 0;
    // virtual void sendPullAttReq() = 0;
    // virtual void handlePullAttReq(cMessage* msg) = 0;
    // virtual void sendAttReq(uid_t target, KEYID kid) = 0;
    // virtual void handleAttReq(cMessage* msg) = 0;
    // virtual void sendAttResp(uid_t target) = 0;
    // virtual void handleAttResp(cMessage* msg) = 0;
    // virtual void sendAttAck(uid_t target) = 0;
    // virtual void handleAttAck(cMessage* msg) = 0;
    // virtual void handlePTimeOut(cMessage* msg) = 0;
    // virtual void handleVTimeOut(cMessage* msg) = 0;

    // Update
    virtual void handleUpMsg(cMessage* msg);
    virtual void handleUpReq(cMessage* msg) = 0;
    virtual void sendUpReq(treeid_t tid) = 0;
    virtual void handleUpdateTimeOut(cMessage* msg) = 0;

    // Revoke
    virtual void handleRevMsg(cMessage* msg);
    virtual void handleRevReq(cMessage* msg) = 0;
    virtual void sendRevReq(uid_t target, vector<keyid_t> kids) = 0;

    // Sync
    virtual void handleSyncMsg(cMessage* msg);
    virtual void sendSyncReq() = 0;
    virtual void handleSyncReq(cMessage* msg) = 0;

    virtual void handleTxDoneMsg(cMessage *msg);     
    virtual void handlePostponeDoneMsg(cMessage *msg);     

    // utilities
    virtual void logInfo(string m) = 0;
    virtual void logDebug(string m) = 0;
    virtual  void logError(string m) = 0;
    virtual  void logWarn(string m) = 0;
    virtual void checkSoftConfig() = 0;
    template <typename T> void sendProver(cMessage* msg);               // This method consume the message ;)
    void sendProverBroadcast(cMessage* msg);                            // This method does not consume the message ;)
    template <typename T> void sendCollector(cMessage* msg);            // This method consume the message ;)
    void sendCollectorBroadcast(cMessage* msg);                         // This method does not consume the message ;)
    int generateMAC(cMessage* msg, keyid_t kid);
    double getMacDelays();
    template <class T> bool checkMAC(T* MSGg, keyid_t kid);
    template <typename T> const T getBaseID();
    template <typename T> void updateGates(cMessage* msg);
    template <typename T> double macDelay(T* msg);
    template <typename T> bool isWithinRange(cMessage* msg);
    double checkFirmwareDelay();
    bool isChannelBusy();
    bool checkRecordTime();
    void handleDoneEnMsg(cMessage* msg);
    int connectedGates();

protected:
    virtual void handleMessage(cMessage *msg) override;


public:
    ADevice() {}
    ~ADevice() {}
    bool isTransmiting() {return txrxstat == TRANSMITING;};
    bool isChannelBusy(int far, int gid);
    virtual bool isChannelBusyServ(int far, int gid, uid_t target) = 0;
};

#endif /* ADEVICE_H_ */
