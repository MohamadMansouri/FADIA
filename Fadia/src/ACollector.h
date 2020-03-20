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

using namespace omnetpp;


class ACollector : public cSimpleModule {
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

};


#endif /* SRC_ACOLLECTOR_H_ */
