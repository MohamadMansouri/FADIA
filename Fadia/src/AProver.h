#ifndef AProver_H_
#define AProver_H_


#include <string.h>
#include <omnetpp.h>
#include <stdint.h>
#include <stddef.h>
#include <map>

#include "common.h"

using namespace omnetpp;





class AProver : public cSimpleModule
{

private:
    const size_t KRsize = par("keyRingSize");
    const size_t KPsize = par("keyPoolSize");
    const int kidseed = par("keyIdSeed") ;
    const int keyseed = par("keySeed") ;


protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};


#endif
