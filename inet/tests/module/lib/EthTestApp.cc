//
// Copyright (C) 2013 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/common/INETDefs.h"

#include "inet/common/packet/chunk/ByteCountChunk.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/ethernet/common/EthernetMacHeader_m.h"
#include "inet/linklayer/common/MacAddress.h"

namespace inet {

class INET_API EthTestApp : public cSimpleModule
{
  protected:
    MacAddress destAddr;

  public:
    EthTestApp() {}

  protected:
    void parseScript(const char *script);
    void processIncomingPacket(cMessage *msg);
    void createCommand(simtime_t t, int bytes);
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};


Define_Module(EthTestApp);

void EthTestApp::initialize()
{
    const char *addr = par("destAddr");
    destAddr = MacAddress(addr);
    const char *script = par("script");
    parseScript(script);
}

void EthTestApp::createCommand(simtime_t t, int bytes)
{
    char name[100];
    sprintf(name, "PK at %s: %i Bytes", SIMTIME_STR(t), bytes);
    Packet *packet = new Packet(name);
    const auto& hdr = makeShared<EthernetMacHeader>();
    hdr->setDest(destAddr);
    hdr->setChunkLength(B(14));
    packet->insertAtFront(hdr);
    const auto& payload = makeShared<ByteCountChunk>(B(bytes-14-4));
    packet->insertAtBack(payload);
    const auto& fcs = makeShared<EthernetFcs>();
    fcs->setFcsMode(FCS_DECLARED_CORRECT);
    packet->insertAtBack(fcs);
    ASSERT(packet->getByteLength() == bytes);
    //TODO set packet->destAddr
    scheduleAt(t, packet);
}

void EthTestApp::parseScript(const char *script)
{
    const char *s = script;

    while (isspace(*s)) s++;

    while (*s)
    {
        // simtime in seconds
        char *os;
        double sendingTime = strtod(s,&os);
        if ((s == os) | (*os != ':'))
            throw cRuntimeError("syntax error in script: missing ':' after time");
        s = os;
        s++;
        while (isspace(*s)) s++;

        // length in bytes
        if (!isdigit(*s))
            throw cRuntimeError("syntax error in script: wrong bytelength spec");
        int bytes = atoi(s);
        while (isdigit(*s)) s++;

        // add command
        createCommand(sendingTime, bytes);

        // skip delimiter
        while (isspace(*s)) s++;
    }
}

void EthTestApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        send(msg, "out");
    else
        processIncomingPacket(msg);
}

void EthTestApp::processIncomingPacket(cMessage *msg)
{
    delete msg;
}

void EthTestApp::finish()
{
}

} // namespace inet

