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

    //initialize
    void initCID();

    // Join
    virtual void handleJoinMsg(cMessage* msg) override;
    virtual void handleJoinReq(cMessage* msg) override;
    virtual void sendJoinResp(UID target, double battery) override;
    virtual void handleJoinAck(cMessage* msg) override;

    // Update
    virtual void handleUpMsg(cMessage* msg) override;
    virtual void handleUpReq(cMessage* msg) override;

    // Revoke
    virtual void handleRevMsg(cMessage* msg) override;
    virtual void sendRevReq(UID comdev) override;

    // Synchronization
    virtual void handleSyncMsg(cMessage* msg) override;
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
    virtual void handleMessage(cMessage *msg) override;

public:
    ACollector() {}
    ~ACollector() {}

};


#endif /* SRC_ACOLLECTOR_H_ */
