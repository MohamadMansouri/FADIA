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

using namespace std;
using namespace omnetpp;



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
    void initUID();
    void initNO();
    void initKeyRing();

    // Core
    void Join();
    void NeighborDiscover() ;
    void Prove();
    void Verify();
    void Revoke();

    // Join
    virtual void handleJoinMsg(cMessage* msg) override;
    virtual void sendJoinReq() override;
    virtual void handleJoinResp(cMessage* msg) override;
    virtual void sendJoinAck(UID target) override;

//    // ND ( DEPRECIATED )
//    void handleNDMsg(cMessage* msg);
//    void sendNDReq();
//    void handleNDReq(cMessage* msg);
//    void sendNDResp(UID target, KEYID kid);
//    void handleNDResp(cMessage* msg);
//    void sendNDAck(UID target, KEYID kid);
//    void handleNDAck(cMessage* msg);
//    void addNeighbor(UID uid);

    // Attest
    virtual void handleAttMsg(cMessage* msg) override;
    virtual void sendPullAttReq() override;
    virtual void handlePullAttReq(cMessage* msg) override;
    virtual void sendAttReq(UID target, KEYID kid) override;
    virtual void handleAttReq(cMessage* msg) override;
    virtual void sendAttResp(UID target) override;
    virtual void handleAttResp(cMessage* msg) override;
    virtual void sendAttAck(UID target) override;
    virtual void handleAttAck(cMessage* msg) override;
    virtual void handlePTimeOut(cMessage* msg) override;
    virtual void handleVTimeOut(cMessage* msg) override;

    // Update
    virtual void sendUpReq(CID target) override;

    // Revoke
    virtual void handleRevMsg(cMessage* msg) override;
    virtual void handleRevReq(cMessage* msg) override;

    // utilities
    const CID findClosestCollector();
    const double getBatteryLevel();
    CHLNG generateChallenge();
    SOLV solveChallenge(UID target);
    void logInfo(string m);
    void logDebug(string m);
    void logError(string m);
    void postponeMsg(cMessage* msg);

    //public:
//    AProver();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;

public:
    AProver() {}
    ~AProver() {}
};


#endif
