#ifndef AProver_H_
#define AProver_H_


#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <utility>
#include <vector>

#include "ADevice.h"
#include "ADevice_template.h"
#include "Commit_m.h"

using namespace std;
using namespace omnetpp;

#ifndef depreciated
#define depreciated {logError("function is not implemented for the prover");}
#endif

struct session_t
{
    TREEID treeID;
    UID deviceID;
    KEYID keyID; 
    unsigned int depth;
    bool valid;
    CommitTimeOut* tomsg = nullptr;

    session_t(TREEID t, UID uid, KEYID k, unsigned int d, bool v = false);
};


typedef struct session_t session_t; 
typedef map<UID, session_t> SESS_DEV;
typedef map<UID, session_t>::iterator SESS_DEV_IT;
typedef multi_map<TREEID, session_t> SESS_TREE;
typedef multi_map<TREEID, session_t>::iterator SESS_TREE_IT;


class AProver : public ADevice
{

private:
    UID UId;
    const int seed = 0;
    const double postponetime = 0.1;
    const double timeout = 1;
    KEYRNG KeyRing;
    NTBL NTable;
    NTBL NTableTmp;
    SEST vsessions;
    SEST psessions;
    SESS_DEV psessions_bydev;
    SESS_TREE psessions_bytree;
    SESS_DEV csessions_bydev;
    SESS_TREE csessions_bytree;
    CID cid;
    KEY ckey = 0xCAFED00D;
    unsigned int pcounter = 0;
    unsigned int prove = 3;
    unsigned int vcounter = 0;
    unsigned int verify = 1;
    unsigned int ptime;
    unsigned int vtime;
    vector<UID> goodDevices;
    vector<UID> badDevices;

    // Inits
    virtual void initUID() override;
    virtual void initKeyRing() override;
    void initNO();

    // Join
    virtual void handleJoinReq(cMessage* msg) depreciated;
    virtual void handleJoinResp(cMessage* msg) override;
    virtual void handleJoinAck(cMessage* msg) depreciated;
    virtual void sendJoinReq() override;
    virtual void sendJoinResp(UID target, double battery) depreciated;
    virtual void sendJoinAck(UID target) override;


    // Make Spanning Tree
    virtual void handleCommitReq(cMessage* msg) override;
    virtual void handleCommitResp(cMessage* msg) override;
    virtual void handleCommitAck(cMessage* msg) override;
    virtual void sendCommitReq(TREEID tid) override;
    virtual void sendCommitResp(UID target, TREEID tid) override;
    virtual void sendCommitAck(UID target, TREEID tid) override;

    // Update
    virtual void handleUpReq(cMessage* msg) depreciated;
    virtual void sendUpReq() override;

    // Revoke
    virtual void handleRevReq(cMessage* msg) override;
    virtual void sendRevReq(UID comdev) depreciated;

    // Sync
    virtual void sendSyncReq() depreciated;
    virtual void handleSyncReq(cMessage* msg) depreciated;

    // Attest
    virtual void handleAttMsg(cMessage* msg) depreciated;
    virtual void sendPullAttReq() ;
    virtual void handlePullAttReq(cMessage* msg) ;
    virtual void sendAttReq(UID target, KEYID kid) ;
    virtual void handleAttReq(cMessage* msg) ;
    virtual void sendAttResp(UID target) ;
    virtual void handleAttResp(cMessage* msg) ;
    virtual void sendAttAck(UID target) ;
    virtual void handleAttAck(cMessage* msg) ;
    virtual void handlePTimeOut(cMessage* msg) ;
    virtual void handleVTimeOut(cMessage* msg) ;


    // utilities
    const CID findClosestCollector();
    const double getBatteryLevel();
    CHLNG generateChallenge();
    SOLV solveChallenge(UID target);
    virtual void logInfo(string m) override;
    virtual void logDebug(string m) override;
    virtual void logError(string m) override;
    void postponeMsg(cMessage* msg);

    //public:
//    AProver();

protected:
    virtual void initialize() override;
    virtual void refreshDisplay() const override;

public:
    AProver() {}
    ~AProver() {}
};


#endif
