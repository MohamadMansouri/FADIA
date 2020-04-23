/*
 * ACollector.h
 *
 *  Created on: Mar 19, 2020
 *      Author: mmrota
 */

#ifndef SRC_ACOLLECTOR_H_
#define SRC_ACOLLECTOR_H_



#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <utility>
#include <vector>

#include "ADevice.h"
#include "ADevice_template.h"
#include "NetworkOwner.h"

#ifndef depreciated
#define depreciated {logError("function is not implemented for a collector");}
#endif

using namespace omnetpp;

typedef map<uid_t, keyv_t> pkey_map_t;
typedef map<uid_t, vector<keyv_t>> pkid_map_t;
typedef map<uid_t, pair<bool, simtime_t>> devstat_map_t;

class ACollector : public ADevice
{

private:
    // unique id of collector
    cid_t CId;

    // active join sessions (depreciated for now) 
    vector<uid_t> jsessions;

    // keys to comunicate with provers securly
    pkey_map_t proverKeys;

    // key ids of each prover's key ring
    pkid_map_t proverKeyIds;

    // status table holding provers statuses
    devstat_map_t statusTable;

    // configuration parameters
    double deltah = 600;
    int maxdepth = INT_MAX;

    //Signals
    simsignal_t elapsedtimesig;

    // Inits
    virtual void initUID() override;
    virtual void initKeyRing() depreciated;
    // virtual void initPorverKeys();

    // Join
    virtual void handleJoinReq(cMessage* msg) override;
    virtual void handleJoinResp(cMessage* msg) depreciated;
    virtual void handleJoinAck(cMessage* msg) depreciated;
    virtual void sendJoinReq() depreciated;
    virtual void sendJoinResp(uid_t target) override;
    virtual void sendJoinAck(uid_t target) depreciated;
    virtual void handleJoinRespTimeOut(cMessage* msg) depreciated;
    virtual void handleJoinAckTimeOut(cMessage* msg) depreciated;
    virtual void handleMessageUpdateTimeOut(cMessage* msg) depreciated;


    // Make Spanning Tree
    virtual void startAttestation() depreciated;
    virtual void handleCommitReq(cMessage* msg) depreciated;
    virtual void handleCommitResp(cMessage* msg) depreciated;
    virtual void handleCommitAck(cMessage* msg) depreciated;
    virtual void sendCommitReq(treeid_t tid) depreciated;
    virtual void sendCommitResp(uid_t target, treeid_t tid) depreciated;
    virtual void sendCommitAck(uid_t target, treeid_t tid)  depreciated;
    virtual void handleCommitRespTimeOut(cMessage* msg) depreciated;
    virtual void handleCommitAckTimeOut(cMessage* msg) depreciated;

    // Update
    virtual void handleUpReq(cMessage* msg) override;
    virtual void sendUpReq(treeid_t tid) depreciated;
    virtual void handleUpdateTimeOut(cMessage* msg) depreciated;

    // Revoke
    virtual void handleRevReq(cMessage* msg) depreciated;
    virtual void sendRevReq(uid_t comdev) override;

    // Sync
    virtual void sendSyncReq() override;
    virtual void handleSyncReq(cMessage* msg) override;

    // Utility
    void logInfo(string m);
    void logDetail(string m);
    void logDebug(string m);
    void logError(string m);
    void checkSoftConfig() depreciated;
    int chooseProve(double p);
    int chooseVerify(double p);
    void updateProverKey(uid_t uid);

protected:
    virtual void initialize() override;

public:
    ACollector() {}
    ~ACollector() {}

};


#endif /* SRC_ACOLLECTOR_H_ */
