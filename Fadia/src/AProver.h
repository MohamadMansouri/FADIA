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

struct session_t
{
    treeid_t treeID;
    uid_t deviceID;
    keyid_t keyID; 
    unsigned int depth;
    bool valid;
    CommitTimeOut* tomsg = nullptr;
    
    session_t() :
    treeID(NOID),
    deviceID(NOID),
    keyID(NOID),
    depth(0),
    valid(false)
    {};

    session_t(treeid_t t, uid_t uid, keyid_t k, unsigned int d, bool v = false) :
    treeID(t),
    deviceID(uid),
    keyID(k),
    depth(d),
    valid(v)
    {};
};

struct report_t 
{
    map<uid_t, unsigned int> devices;
    treeid_t tid;
    mac_t mac;
    
    report_t() : tid(NOID), mac(0)
    {};
};

typedef struct session_t session_t;
typedef pair<uid_t, session_t *> session_dev_pair_t; 
// typedef pair<treeid_t, session_t *> session_tree_pair_t; 
typedef map<uid_t, session_t *> session_dev_map_t;
// typedef multimap<treeid_t, session_t *> session_tree_map_t;

class AProver : public ADevice
{

private:
    // Unique ID of prover
    uid_t UId;

    // prover's key ring
    keyrng_t KeyRing;
  
    // parent and children open sessions
    session_dev_pair_t psessions = {NOID, nullptr};
    // session_tree_pair_t* psessions_bytree;
    session_dev_map_t csessions;
    // session_tree_map_t csessions_bytree;
  
    // active collector id and key
    cid_t cid;
    keyv_t ckey = 0xCAFED00D;

    // attestion counter for each prover
    int attestcount = 0;

    // device status
    status_e status = OFFLINE;

    // max number of children
    unsigned int maxchildren = 10;

    // max depth of the tree
    unsigned int maxdepth = 5;

    // delta time for a prover to attest within
    double deltah;

    // delta time for a prover to attest within
    double deltag;

    // aggregated report at each delta h
    report_t aggreport;

    // messages
    cMessage* startmsg = nullptr;
    
    cMessage* jointomsg = nullptr;

    cMessage* mktreetimer = nullptr;
    cMessage* attesttimer = nullptr;
    cMessage* checkdelaymsg = nullptr;

    CommitReq* creqmsg = nullptr;

    CommitTimeOut* cresptomsg = nullptr;
    CommitTimeOut* cacktomsg = nullptr;

    // Inits
    virtual void initUID() override;
    virtual void initKeyRing() override;
    void initNO();

    // Join
    virtual void handleJoinReq(cMessage* msg) depreciated;
    virtual void handleJoinResp(cMessage* msg) override;
    virtual void handleJoinAck(cMessage* msg) depreciated;
    virtual void sendJoinReq() override;
    virtual void sendJoinResp(uid_t target) depreciated;
    virtual void sendJoinAck(uid_t target) depreciated;
    virtual void handleJoinRespTimeOut(cMessage* msg) override;
    virtual void handleJoinAckTimeOut(cMessage* msg) depreciated;


    // Make Spanning Tree
    virtual void startAttestation() override;
    virtual void handleCommitReq(cMessage* msg) override;
    virtual void handleCommitResp(cMessage* msg) override;
    virtual void handleCommitAck(cMessage* msg) override;
    virtual void sendCommitReq(treeid_t tid) override;
    virtual void sendCommitResp(uid_t target, treeid_t tid) override;
    virtual void sendCommitAck(uid_t target, treeid_t tid) override;
    virtual void handleCommitRespTimeOut(cMessage* msg) override;
    virtual void handleCommitAckTimeOut(cMessage* msg) override;

    // Update
    virtual void handleUpReq(cMessage* msg) override;
    virtual void sendUpReq(treeid_t tid) override;

    // Revoke
    virtual void handleRevReq(cMessage* msg) override;
    virtual void sendRevReq(uid_t comdev) depreciated;

    // Sync
    virtual void sendSyncReq() depreciated;
    virtual void handleSyncReq(cMessage* msg) depreciated;  
    virtual void handleUpdateTimeOut(cMessage* msg) override;

    // utilities
    const cid_t findClosestCollector();
    const double getBatteryLevel();
    virtual void logInfo(string m) override;
    virtual void logDebug(string m) override;
    virtual void logWarn(string m);

    virtual void logError(string m) override;
    virtual void checkSoftConfig() override;
    void postponeMsg(cMessage* msg);
    void createProof();
    void aggregateProof();
    
    //public:
//    AProver();

protected:
    virtual void initialize() override;
    virtual void finish() override;
    // virtual void refreshDisplay() const override;

public:
    AProver() {}
    ~AProver() {}
};


#endif
