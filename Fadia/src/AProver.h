#ifndef AProver_H_
#define AProver_H_


#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>
#include <utility>
#include <vector>

#include "common.h"

using namespace std;
using namespace omnetpp;



//typedef pair<UID, KEYID> IDPAIR;
typedef map<UID, KEYID> NTBL;

class AProver : public cSimpleModule
{

private:
    UID UId;
    size_t KRsize;
    size_t KPsize;
    const int seed = 0;
    KEYRNG KeyRing;
    NTBL NTable;
    NTBL NTableTmp;

    // Inits
    void initUID();
    void initKeyRing();

    // utilities
    template<class T, class U> T* getIds(U list);
    void sendProver(UID target, cMessage* msg);            // This message does not consume the message ;)
    void sendProverBroadcast(cMessage* msg);                // This message does not consume the message ;)
    int generateMAC(cMessage* msg);
    template <class T> bool checkMAC(T* msg);

    // Core
    void Join();
    void NeighborDiscover() ;
    void Prove();
    void Verify();
    void Revoke();

    // Join
    void sendJoinReq();
    void sendJoinResp();
    void sendJoinAck();

    // ND
    void handleNDMsg(cMessage* msg);
    void sendNDReq();
    void handleNDReq(cMessage* msg);
    void sendNDResp(UID target, KEYID kid);
    void handleNDResp(cMessage* msg);
    void sendNDAck(UID target);
    void handleNDAck(cMessage* msg);
    void addNeighbor(UID uid);
    // Prove

    // Verify

    // Revoke

//public:
//    AProver();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};


#endif
