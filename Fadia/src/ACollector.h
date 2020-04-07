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
#define depreciated {logError("function is not implemented for the prover");}
#endif

using namespace omnetpp;

typedef map<UID, KEY> KEY_MAP;
typedef map<UID, pair<bool, simtime_t>> STAT_MAP;

class ACollector : public ADevice
{

private:
    CID CId;
    vector<UID> jsessions;
    KEY_MAP channelKeys;
    STAT_MAP statusTable;

    // Inits
    virtual void initUID() override;
    virtual void initKeyRing() depreciated;

    // Join
    // virtual void handleJoinMsg(cMessage* msg) override;
    virtual void handleJoinReq(cMessage* msg) override;
    virtual void handleJoinResp(cMessage* msg) depreciated;
    virtual void handleJoinAck(cMessage* msg) override;
    virtual void sendJoinReq() depreciated;
    virtual void sendJoinResp(UID target, double battery) override;
    virtual void sendJoinAck(UID target) depreciated;


    // Make Spanning Tree
    // virtual void handleCommitMsg(cMessage* msg) depreciated;
    virtual void handleCommitReq(cMessage* msg) depreciated;
    virtual void handleCommitResp(cMessage* msg) depreciated;
    virtual void handleCommitAck(cMessage* msg) depreciated;
    virtual void sendCommitReq(TREEID tid) depreciated;
    virtual void sendCommitResp(UID target, KEYID kid, TREEID tid) depreciated;
    virtual void sendCommitAck(UID target, KEYID kid, TREEID tid)  depreciated;
    
    // Update
    // virtual void handleUpMsg(cMessage* msg) override;
    virtual void handleUpReq(cMessage* msg) override;
    virtual void sendUpReq() depreciated;

    // Revoke
    // virtual void handleRevMsg(cMessage* msg) depreciated;
    virtual void handleRevReq(cMessage* msg) depreciated;
    virtual void sendRevReq(UID comdev) override;

    // Sync
    // virtual void handleSyncMsg(cMessage* msg) override;
    virtual void sendSyncReq() override;
    virtual void handleSyncReq(cMessage* msg) override;

    // Utility
    void logInfo(string m);
    void logDebug(string m);
    void logError(string m);
    int chooseProve(double p);
    int chooseVerify(double p);

protected:
    virtual void initialize() override;

public:
    ACollector() {}
    ~ACollector() {}

};


#endif /* SRC_ACOLLECTOR_H_ */
