//
// Copyright (C) 2001 Matthias Oppitz <Matthias.Oppitz@gmx.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef __INET_RTPPARTICIPANTINFO_H
#define __INET_RTPPARTICIPANTINFO_H

#include "inet/common/packet/Packet.h"
#include "inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "inet/transportlayer/rtp/RtpPacket_m.h"
#include "inet/transportlayer/rtp/RtpParticipantInfo_m.h"
#include "inet/transportlayer/rtp/Sdes.h"

namespace inet {
namespace rtp {

// Forward declarations:
class ReceptionReport;
class SenderReport;

/**
 * This class is a super class for classes intended for storing information
 * about RTP end systems.
 * It has two subclasses: RTPReceiverInformation which is used for storing
 * information about other system participating in an RTP session.
 * RTPSenderInformation is used by an RTP endsystem for storing information
 * about itself.
 * \sa RTPReceiverInformation
 * \sa RTPSenderInformation
 */
class INET_API RtpParticipantInfo : public RtpParticipantInfo_Base
{
  public:
    /**
     * Default constructor.
     */
    RtpParticipantInfo(uint32_t ssrc = 0);

    /**
     * Copy constructor.
     */
    RtpParticipantInfo(const RtpParticipantInfo& participantInfo);

    /**
     * Destructor.
     */
    virtual ~RtpParticipantInfo();

    /**
     * Assignment operator.
     */
    RtpParticipantInfo& operator=(const RtpParticipantInfo& participantInfo);

    /**
     * Duplicates this RtpParticipantInfo by calling the copy constructor.
     */
    virtual RtpParticipantInfo *dup() const override;

    /**
     * This method should be extended by a subclass for
     * extracting information about the originating
     * endsystem of an RTP packet.
     * This method sets _silentInterval to 0 so that
     * the sender of this RTP packet is regarded as
     * an active sender.
     */
    virtual void processRTPPacket(Packet *packet, int id, simtime_t arrivalTime);

    /**
     * This method extracts information about an RTP endsystem
     * as provided by the given SenderReport.
     */
    virtual void processSenderReport(const SenderReport& report, simtime_t arrivalTime);

    /**
     * This method extracts information of the given ReceptionReport.
     */
    virtual void processReceptionReport(const ReceptionReport& report, simtime_t arrivalTime);

    /**
     * This method extracts sdes information of the given sdes chunk.and stores it.
     */
    virtual void processSDESChunk(const SdesChunk *sdesChunk, simtime_t arrivalTime);

    /**
     * Returns a copy of the sdes chunk used for storing source
     * description items about this system.
     */
    virtual SdesChunk *getSDESChunk() const;

    /**
     * Adds this sdes item to the sdes chunk of this participant.
     */
    virtual void addSDESItem(SdesItem *sdesItem);

    /**
     * This method is intended to be overwritten by subclasses. It
     * should return a receiver report if there have been received
     * RTP packets from that endsystem and nullptr otherwise.
     */
    virtual ReceptionReport *receptionReport(simtime_t now);

    /**
     * This method is intended to be overwritten by subclasses which
     * are used for storing information about itself.
     * It should return a sender report if there have been sent RTP
     * packets recently or nullptr otherwise.
     * The implementation for this class always returns nullptr.
     */
    virtual SenderReport *senderReport(simtime_t now);

    /**
     * This method should be called by the rtcp module which uses this class
     * for storing information every time an rtcp packet is sent.
     * Some behaviour of RTP and rtcp (and this class) depend on how
     * many rtcp intervals have passed, for example an RTP end system
     * is marked as inactive if there haven't been received packets from
     * it for a certain number of rtpc intervals.
     * Call getSenderReport() and createReceptionReport() before calling this method.
     * \sa getSenderReport()
     * \sa createReceptionReport()
     */
    virtual void nextInterval(simtime_t now);

    /**
     * Returns true if the end system does no longer participate
     * in the RTP session.
     * The implementation in this class always returns false.
     */
    virtual bool toBeDeleted(simtime_t now);

    /**
     * Returns true if this endsystem has sent at least one RTP
     * data packet during the last two rtcp intervals (including
     * the current one).
     */
    virtual bool isSender() const;

    /**
     * Returns the ssrc identifier of the RTP endsystem.
     */
    virtual uint32_t getSsrc() const override;

    /**
     * Sets the ssrc identifier.
     */
    virtual void setSsrc(uint32_t ssrc) override;

    /**
     * Creates a new SdesItem and adds it to the SdesChunk stored in
     * this RtpParticipantInfo.
     */
    virtual void addSDESItem(SdesItem::SdesItemType type, const char *content);

    virtual void parsimPack(cCommBuffer *b) const override { throw cRuntimeError(this, "parsimPack() not implemented"); }
    virtual void parsimUnpack(cCommBuffer *b) override { throw cRuntimeError("The parsimUnpack() not implemented."); }

    /**
     * This method returns the given 32 bit ssrc identifier as
     * an 8 character hexadecimal number which is used as name
     * of an RtpParticipantInfo object.
     */
    static std::string ssrcToName(uint32_t ssrc);

  private:
    void copy(const RtpParticipantInfo& other);

  protected:
    /**
     * Used for storing sdes information about this RTP endsystem.
     * The ssrc identifier is also stored here.
     */
    SdesChunk _sdesChunk;

    /**
     * Stores the number of rtcp intervals (including the current one)
     * during which this RTP endsystem hasn't sent any RTP data packets.
     * When an RTP data packet is received it is reset to 0.
     */
    int _silentIntervals;
};

} // namespace rtp
} // namespace inet

#endif

