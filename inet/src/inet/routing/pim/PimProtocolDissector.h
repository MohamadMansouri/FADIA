//
// Copyright (C) 2018 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_PIMPROTOCOLDISSECTOR_H
#define __INET_PIMPROTOCOLDISSECTOR_H

#include "inet/common/packet/dissector/ProtocolDissector.h"

namespace inet {

class INET_API PimProtocolDissector : public ProtocolDissector
{
  public:
    virtual void dissect(Packet *packet, const Protocol *protocol, ICallback& callback) const override;
};

} // namespace inet

#endif

