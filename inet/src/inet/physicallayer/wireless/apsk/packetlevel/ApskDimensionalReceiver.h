//
// Copyright (C) 2013 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//


#ifndef __INET_APSKDIMENSIONALRECEIVER_H
#define __INET_APSKDIMENSIONALRECEIVER_H

#include "inet/physicallayer/wireless/common/base/packetlevel/FlatReceiverBase.h"
#include "inet/physicallayer/wireless/common/contract/packetlevel/IModulation.h"

namespace inet {

namespace physicallayer {

class INET_API ApskDimensionalReceiver : public FlatReceiverBase
{
  protected:
    virtual bool computeIsReceptionPossible(const IListening *listening, const ITransmission *transmission) const override;
    virtual bool computeIsReceptionPossible(const IListening *listening, const IReception *reception, IRadioSignal::SignalPart part) const override;

  public:
    ApskDimensionalReceiver();

    virtual std::ostream& printToStream(std::ostream& stream, int level, int evFlags = 0) const override;
};

} // namespace physicallayer

} // namespace inet

#endif

