//
// Copyright (C) 2013 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#include "inet/physicallayer/wireless/common/base/packetlevel/ErrorModelBase.h"

#include "inet/common/ProtocolTag_m.h"

namespace inet {

namespace physicallayer {

void ErrorModelBase::initialize(int stage)
{
    if (stage == INITSTAGE_LOCAL) {
        const char *corruptionModeString = par("corruptionMode");
        if (!strcmp("packet", corruptionModeString))
            corruptionMode = CorruptionMode::CM_PACKET;
        else if (!strcmp("chunk", corruptionModeString))
            corruptionMode = CorruptionMode::CM_CHUNK;
        else if (!strcmp("byte", corruptionModeString))
            corruptionMode = CorruptionMode::CM_BYTE;
        else if (!strcmp("bit", corruptionModeString))
            corruptionMode = CorruptionMode::CM_BIT;
        else
            throw cRuntimeError("Unknown corruption mode");
        const char *snirModeString = par("snirMode");
        if (!strcmp("min", snirModeString))
            snirMode = SnirMode::SM_MIN;
        else if (!strcmp("mean", snirModeString))
            snirMode = SnirMode::SM_MEAN;
        else
            throw cRuntimeError("Unknown SNIR mode: '%s'", snirModeString);
        snirOffset = inet::math::dB2fraction(par("snirOffset"));
    }
}

double ErrorModelBase::getScalarSnir(const ISnir *snir) const
{
    double scalarSnir;
    if (snirMode == SnirMode::SM_MIN)
        scalarSnir = snir->getMin();
    else if (snirMode == SnirMode::SM_MEAN)
        scalarSnir = snir->getMean();
    else
        throw cRuntimeError("Unknown SNIR mode");
    return scalarSnir * snirOffset;
}

bool ErrorModelBase::hasProbabilisticError(b length, double ber) const
{
    ASSERT(0.0 < ber && ber <= 1.0);
    return dblrand() < 1 - std::pow((1 - ber), length.get());
}

Packet *ErrorModelBase::corruptBits(const Packet *packet, double ber, bool& isCorrupted) const
{
    std::vector<bool> corruptedBits;
    const auto& all = packet->peekAllAsBits();
    for (bool bit : all->getBits()) {
        if (hasProbabilisticError(b(1), ber)) {
            isCorrupted = true;
            bit = !bit;
        }
        corruptedBits.push_back(bit);
    }
    return new Packet(packet->getName(), makeShared<BitsChunk>(corruptedBits));
}

Packet *ErrorModelBase::corruptBytes(const Packet *packet, double ber, bool& isCorrupted) const
{
    std::vector<uint8_t> corruptedBytes;
    const auto& all = packet->peekAllAsBytes();
    for (uint8_t byte : all->getBytes()) {
        if (hasProbabilisticError(B(1), ber)) {
            isCorrupted = true;
            byte = ~byte;
        }
        corruptedBytes.push_back(byte);
    }
    return new Packet(packet->getName(), makeShared<BytesChunk>(corruptedBytes));
}

Packet *ErrorModelBase::corruptChunks(const Packet *packet, double ber, bool& isCorrupted) const
{
    b offset = b(0);
    auto corruptedPacket = new Packet(packet->getName());
    while (auto chunk = packet->peekAt(offset, b(-1), Chunk::PF_ALLOW_NULLPTR)) {
        if (hasProbabilisticError(chunk->getChunkLength(), ber)) {
            isCorrupted = true;
            auto corruptedChunk = chunk->dupShared();
            corruptedChunk->markIncorrect();
            corruptedPacket->insertAtBack(corruptedChunk);
        }
        else
            corruptedPacket->insertAtBack(chunk);
        offset += chunk->getChunkLength();
    }
    return corruptedPacket;
}

Packet *ErrorModelBase::corruptPacket(const Packet *packet, bool& isCorrupted) const
{
    isCorrupted = true;
    auto corruptedPacket = packet->dup();
    corruptedPacket->setBitError(true);
    return corruptedPacket;
}

Packet *ErrorModelBase::computeCorruptedPacket(const Packet *packet, double ber) const
{
    bool isCorrupted = false;
    Packet *corruptedPacket = nullptr;
    // TODO this while loop looks bad, but we don't have any other chance now, because the decision whether the reception is successful or not has been already made
    while (!isCorrupted) {
        switch (corruptionMode) {
            case CorruptionMode::CM_PACKET:
                corruptedPacket = corruptPacket(packet, isCorrupted);
                break;
            case CorruptionMode::CM_CHUNK:
                corruptedPacket = corruptChunks(packet, ber, isCorrupted);
                break;
            case CorruptionMode::CM_BYTE:
                corruptedPacket = corruptBytes(packet, ber, isCorrupted);
                break;
            case CorruptionMode::CM_BIT:
                corruptedPacket = corruptBits(packet, ber, isCorrupted);
                break;
            default:
                throw cRuntimeError("Unknown corruption mode");
        }
        if (!isCorrupted)
            delete corruptedPacket;
    }
    return corruptedPacket;
}

Packet *ErrorModelBase::computeCorruptedPacket(const ISnir *snir) const
{
    auto transmittedPacket = snir->getReception()->getTransmission()->getPacket();
    auto ber = computeBitErrorRate(snir, IRadioSignal::SIGNAL_PART_WHOLE);
    auto receivedPacket = computeCorruptedPacket(transmittedPacket, ber);
    receivedPacket->clearTags();
    receivedPacket->addTag<PacketProtocolTag>()->setProtocol(transmittedPacket->getTag<PacketProtocolTag>()->getProtocol());
    return receivedPacket;
}

} // namespace physicallayer

} // namespace inet

