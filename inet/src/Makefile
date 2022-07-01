#
# OMNeT++/OMNEST Makefile for $(LIB_PREFIX)INET
#
# This file was generated with the command:
#  opp_makemake --make-so -f --deep -o INET -O out -pINET -I. -Xinet/applications/clock -Xinet/applications/dhcp -Xinet/applications/ethernet -Xinet/applications/ieee8022 -Xinet/applications/rtpapp -Xinet/applications/sctpapp -Xinet/applications/tcpapp -Xinet/applications/tunapp -Xinet/applications/udpapp -Xinet/applications/voip -Xinet/applications/voipstream -Xinet/clock/base -Xinet/clock/common -Xinet/clock/model -Xinet/clock/oscillator -Xinet/common/selfdoc -Xinet/emulation -Xinet/environment/common -Xinet/environment/ground -Xinet/environment/objectcache -Xinet/linklayer/acking -Xinet/linklayer/bmac -Xinet/linklayer/configurator/gatescheduling/base -Xinet/linklayer/configurator/gatescheduling/common -Xinet/linklayer/configurator/gatescheduling/z3 -Xinet/linklayer/csmaca -Xinet/linklayer/ethernet -Xinet/linklayer/ieee80211 -Xinet/linklayer/ieee802154 -Xinet/linklayer/ieee8021ae -Xinet/linklayer/ieee8021as -Xinet/linklayer/ieee8021d -Xinet/linklayer/ieee8021q -Xinet/linklayer/ieee8021r -Xinet/linklayer/ieee8022 -Xinet/linklayer/lmac -Xinet/linklayer/ppp -Xinet/linklayer/shortcut -Xinet/linklayer/tun -Xinet/linklayer/virtual -Xinet/linklayer/vlan -Xinet/linklayer/xmac -Xinet/networklayer/arp/ipv4 -Xinet/networklayer/configurator/ipv4 -Xinet/networklayer/configurator/ipv6 -Xinet/networklayer/configurator/nexthop -Xinet/networklayer/diffserv -Xinet/networklayer/flooding -Xinet/networklayer/icmpv6 -Xinet/networklayer/internetcloud -Xinet/networklayer/ipv4 -Xinet/networklayer/ipv6 -Xinet/networklayer/ipv6tunneling -Xinet/networklayer/ldp -Xinet/networklayer/mpls -Xinet/networklayer/nexthop -Xinet/networklayer/probabilistic -Xinet/networklayer/rsvpte -Xinet/networklayer/ted -Xinet/networklayer/wiseroute -Xinet/networklayer/xmipv6 -Xinet/networks/tsn -Xinet/node/aodv -Xinet/node/bgp -Xinet/node/dsdv -Xinet/node/dymo -Xinet/node/ethernet -Xinet/node/gpsr -Xinet/node/internetcloud -Xinet/node/ipv6 -Xinet/node/mpls -Xinet/node/ospfv2 -Xinet/node/ospfv3 -Xinet/node/rip -Xinet/node/rtp -Xinet/node/tsn -Xinet/node/wireless -Xinet/node/xmipv6 -Xinet/physicallayer/wired/ethernet -Xinet/physicallayer/wireless/apsk -Xinet/physicallayer/wireless/common/analogmodel -Xinet/physicallayer/wireless/common/antenna -Xinet/physicallayer/wireless/common/backgroundnoise -Xinet/physicallayer/wireless/common/base -Xinet/physicallayer/wireless/common/communicationcache -Xinet/physicallayer/wireless/common/energyconsumer -Xinet/physicallayer/wireless/common/errormodel -Xinet/physicallayer/wireless/common/medium -Xinet/physicallayer/wireless/common/modulation -Xinet/physicallayer/wireless/common/neighborcache -Xinet/physicallayer/wireless/common/obstacleloss -Xinet/physicallayer/wireless/common/pathloss -Xinet/physicallayer/wireless/common/propagation -Xinet/physicallayer/wireless/common/radio -Xinet/physicallayer/wireless/common/signal -Xinet/physicallayer/wireless/ieee80211 -Xinet/physicallayer/wireless/ieee802154 -Xinet/physicallayer/wireless/noise -Xinet/physicallayer/wireless/shortcut -Xinet/physicallayer/wireless/unitdisk -Xinet/protocolelement/acknowledgement -Xinet/protocolelement/aggregation -Xinet/protocolelement/checksum -Xinet/protocolelement/common -Xinet/protocolelement/dispatching -Xinet/protocolelement/forwarding -Xinet/protocolelement/fragmentation -Xinet/protocolelement/lifetime -Xinet/protocolelement/measurement -Xinet/protocolelement/ordering -Xinet/protocolelement/processing -Xinet/protocolelement/redundancy -Xinet/protocolelement/selectivity -Xinet/protocolelement/service -Xinet/protocolelement/shaper -Xinet/protocolelement/socket -Xinet/protocolelement/trafficconditioner -Xinet/protocolelement/transceiver -Xinet/queueing/buffer -Xinet/queueing/classifier -Xinet/queueing/filter -Xinet/queueing/flow -Xinet/queueing/gate -Xinet/queueing/marker -Xinet/queueing/meter -Xinet/queueing/policing -Xinet/queueing/queue -Xinet/queueing/scheduler -Xinet/queueing/server -Xinet/queueing/shaper -Xinet/queueing/sink -Xinet/queueing/source -Xinet/queueing/tokengenerator -Xinet/routing/aodv -Xinet/routing/bgpv4 -Xinet/routing/dsdv -Xinet/routing/dymo -Xinet/routing/eigrp -Xinet/routing/gpsr -Xinet/routing/ospfv2 -Xinet/routing/ospfv3 -Xinet/routing/pim -Xinet/routing/rip -Xinet/transportlayer/rtp -Xinet/transportlayer/sctp -Xinet/transportlayer/tcp -Xinet/transportlayer/tcp_common -Xinet/transportlayer/tcp_lwip -Xinet/transportlayer/udp -Xinet/visualizer/base -Xinet/visualizer/canvas -Xinet/visualizer/common -Xinet/visualizer/contract -Xinet/visualizer/osg -Xinet/visualizer/util
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = $(LIB_PREFIX)INET$(D)
TARGET = $(TARGET_NAME)$(SHARED_LIB_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# C++ include paths (with -I)
INCLUDE_PATH = -I.

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/inet/applications/base/ApplicationBase.o \
    $O/inet/applications/base/ApplicationPacketSerializer.o \
    $O/inet/applications/generic/IpvxTrafGen.o \
    $O/inet/applications/generic/IpvxTrafSink.o \
    $O/inet/applications/netperfmeter/NetPerfMeter.o \
    $O/inet/applications/pingapp/PingApp.o \
    $O/inet/clock/contract/IOscillator.o \
    $O/inet/common/BitVector.o \
    $O/inet/common/Compat.o \
    $O/inet/common/FingerprintCalculator.o \
    $O/inet/common/FlowTag.o \
    $O/inet/common/IInterfaceRegistrationListener.o \
    $O/inet/common/INETDefs.o \
    $O/inet/common/INETUtils.o \
    $O/inet/common/InitStageRegistry.o \
    $O/inet/common/InitStages.o \
    $O/inet/common/Int128.o \
    $O/inet/common/IntervalTree.o \
    $O/inet/common/IProtocolRegistrationListener.o \
    $O/inet/common/LayeredProtocolBase.o \
    $O/inet/common/Macho.o \
    $O/inet/common/MatchableField.o \
    $O/inet/common/MatchableObject.o \
    $O/inet/common/MemoryInputStream.o \
    $O/inet/common/MemoryOutputStream.o \
    $O/inet/common/MessageDispatcher.o \
    $O/inet/common/ModuleAccess.o \
    $O/inet/common/NedFunctions.o \
    $O/inet/common/NetworkNamespaceContext.o \
    $O/inet/common/ObjectPrinter.o \
    $O/inet/common/OmittedModule.o \
    $O/inet/common/PacketEventTag.o \
    $O/inet/common/PatternMatcher.o \
    $O/inet/common/Protocol.o \
    $O/inet/common/ProtocolGroup.o \
    $O/inet/common/ProtocolTools.o \
    $O/inet/common/ResidenceTimeMeasurer.o \
    $O/inet/common/ResultFilters.o \
    $O/inet/common/ResultRecorders.o \
    $O/inet/common/ShortBitVector.o \
    $O/inet/common/Simsignals.o \
    $O/inet/common/StringFormat.o \
    $O/inet/common/SubmoduleLayout.o \
    $O/inet/common/TagBase.o \
    $O/inet/common/TlvOptions.o \
    $O/inet/common/Topology.o \
    $O/inet/common/XMLUtils.o \
    $O/inet/common/checksum/EthernetCRC.o \
    $O/inet/common/checksum/TcpIpChecksum.o \
    $O/inet/common/clock/ClockUserModuleMixinImpl.o \
    $O/inet/common/figures/BarFigure.o \
    $O/inet/common/figures/BoxedLabelFigure.o \
    $O/inet/common/figures/CounterFigure.o \
    $O/inet/common/figures/FigureRecorder.o \
    $O/inet/common/figures/GateFigure.o \
    $O/inet/common/figures/GaugeFigure.o \
    $O/inet/common/figures/HeatMapFigure.o \
    $O/inet/common/figures/HeatMapPlotFigure.o \
    $O/inet/common/figures/IndexedImageFigure.o \
    $O/inet/common/figures/IndicatorLabelFigure.o \
    $O/inet/common/figures/IndicatorTextFigure.o \
    $O/inet/common/figures/InstrumentUtil.o \
    $O/inet/common/figures/LabeledIconFigure.o \
    $O/inet/common/figures/LabeledLineFigure.o \
    $O/inet/common/figures/LabeledPolylineFigure.o \
    $O/inet/common/figures/LinearGaugeFigure.o \
    $O/inet/common/figures/PlotFigure.o \
    $O/inet/common/figures/ProgressMeterFigure.o \
    $O/inet/common/figures/QueueFigure.o \
    $O/inet/common/figures/SignalFigure.o \
    $O/inet/common/figures/SimTimeTextFigure.o \
    $O/inet/common/figures/ThermometerFigure.o \
    $O/inet/common/figures/TrailFigure.o \
    $O/inet/common/geometry/base/GeometricObjectBase.o \
    $O/inet/common/geometry/base/ShapeBase.o \
    $O/inet/common/geometry/common/CanvasProjection.o \
    $O/inet/common/geometry/common/Coord.o \
    $O/inet/common/geometry/common/EulerAngles.o \
    $O/inet/common/geometry/common/GeographicCoordinateSystem.o \
    $O/inet/common/geometry/common/Quaternion.o \
    $O/inet/common/geometry/common/RotationMatrix.o \
    $O/inet/common/geometry/container/BvhTree.o \
    $O/inet/common/geometry/container/QuadTree.o \
    $O/inet/common/geometry/container/SpatialGrid.o \
    $O/inet/common/geometry/object/Box.o \
    $O/inet/common/geometry/object/LineSegment.o \
    $O/inet/common/geometry/object/Plane.o \
    $O/inet/common/geometry/object/Polygon.o \
    $O/inet/common/geometry/shape/Cuboid.o \
    $O/inet/common/geometry/shape/Prism.o \
    $O/inet/common/geometry/shape/Sphere.o \
    $O/inet/common/geometry/shape/polyhedron/Polyhedron.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronEdge.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronFace.o \
    $O/inet/common/geometry/shape/polyhedron/PolyhedronPoint.o \
    $O/inet/common/lifecycle/ApplicationOperations.o \
    $O/inet/common/lifecycle/InterfaceOperations.o \
    $O/inet/common/lifecycle/LifecycleController.o \
    $O/inet/common/lifecycle/ModuleOperations.o \
    $O/inet/common/lifecycle/NodeStatus.o \
    $O/inet/common/lifecycle/OperationalMixinImpl.o \
    $O/inet/common/misc/MessageChecker.o \
    $O/inet/common/misc/SignalSource.o \
    $O/inet/common/misc/ThruputMeter.o \
    $O/inet/common/misc/ThruputMeteringChannel.o \
    $O/inet/common/packet/ChunkBuffer.o \
    $O/inet/common/packet/ChunkQueue.o \
    $O/inet/common/packet/Message.o \
    $O/inet/common/packet/Packet.o \
    $O/inet/common/packet/PacketFilter.o \
    $O/inet/common/packet/ReassemblyBuffer.o \
    $O/inet/common/packet/ReorderBuffer.o \
    $O/inet/common/packet/chunk/BitCountChunk.o \
    $O/inet/common/packet/chunk/BitsChunk.o \
    $O/inet/common/packet/chunk/ByteCountChunk.o \
    $O/inet/common/packet/chunk/BytesChunk.o \
    $O/inet/common/packet/chunk/Chunk.o \
    $O/inet/common/packet/chunk/cPacketChunk.o \
    $O/inet/common/packet/chunk/EmptyChunk.o \
    $O/inet/common/packet/chunk/EncryptedChunk.o \
    $O/inet/common/packet/chunk/FieldsChunk.o \
    $O/inet/common/packet/chunk/SequenceChunk.o \
    $O/inet/common/packet/chunk/SliceChunk.o \
    $O/inet/common/packet/dissector/PacketDissector.o \
    $O/inet/common/packet/dissector/ProtocolDissector.o \
    $O/inet/common/packet/dissector/ProtocolDissectorRegistry.o \
    $O/inet/common/packet/printer/PacketPrinter.o \
    $O/inet/common/packet/printer/ProtocolPrinter.o \
    $O/inet/common/packet/printer/ProtocolPrinterRegistry.o \
    $O/inet/common/packet/recorder/PcapngWriter.o \
    $O/inet/common/packet/recorder/PcapReader.o \
    $O/inet/common/packet/recorder/PcapRecorder.o \
    $O/inet/common/packet/recorder/PcapWriter.o \
    $O/inet/common/packet/serializer/BitCountChunkSerializer.o \
    $O/inet/common/packet/serializer/BitsChunkSerializer.o \
    $O/inet/common/packet/serializer/ByteCountChunkSerializer.o \
    $O/inet/common/packet/serializer/BytesChunkSerializer.o \
    $O/inet/common/packet/serializer/ChunkSerializer.o \
    $O/inet/common/packet/serializer/ChunkSerializerRegistry.o \
    $O/inet/common/packet/serializer/EmptyChunkSerializer.o \
    $O/inet/common/packet/serializer/FieldsChunkSerializer.o \
    $O/inet/common/packet/serializer/SequenceChunkSerializer.o \
    $O/inet/common/packet/serializer/SliceChunkSerializer.o \
    $O/inet/common/packet/tag/SharingRegionTagSet.o \
    $O/inet/common/packet/tag/SharingTagSet.o \
    $O/inet/common/packet/tag/TagSet.o \
    $O/inet/common/scenario/ScenarioManager.o \
    $O/inet/common/scheduler/RealTimeScheduler.o \
    $O/inet/common/socket/SocketBase.o \
    $O/inet/common/socket/SocketMap.o \
    $O/inet/common/streetmap/OpenStreetMap.o \
    $O/inet/linklayer/base/MacProtocolBase.o \
    $O/inet/linklayer/base/MacRelayUnitBase.o \
    $O/inet/linklayer/common/ExampleQosClassifier.o \
    $O/inet/linklayer/common/FcsMode.o \
    $O/inet/linklayer/common/MacAddress.o \
    $O/inet/linklayer/common/MacAddressType.o \
    $O/inet/linklayer/common/QosClassifier.o \
    $O/inet/linklayer/common/RandomQosClassifier.o \
    $O/inet/linklayer/common/UserPriorityPacketComparatorFunction.o \
    $O/inet/linklayer/configurator/FailureProtectionConfigurator.o \
    $O/inet/linklayer/configurator/Ieee8021dInterfaceData.o \
    $O/inet/linklayer/configurator/L2NetworkConfigurator.o \
    $O/inet/linklayer/configurator/L2NodeConfigurator.o \
    $O/inet/linklayer/configurator/MacForwardingTableConfigurator.o \
    $O/inet/linklayer/configurator/StreamRedundancyConfigurator.o \
    $O/inet/linklayer/ieee802/Ieee802EpdHeaderSerializer.o \
    $O/inet/linklayer/ieee802/Ieee802EpdProtocolDissector.o \
    $O/inet/linklayer/ieee802/Ieee802EpdProtocolPrinter.o \
    $O/inet/linklayer/loopback/Loopback.o \
    $O/inet/mobility/base/LinearRotatingMobilityBase.o \
    $O/inet/mobility/base/LineSegmentsMobilityBase.o \
    $O/inet/mobility/base/MobilityBase.o \
    $O/inet/mobility/base/MovingMobilityBase.o \
    $O/inet/mobility/base/RotatingMobilityBase.o \
    $O/inet/mobility/contract/IMobility.o \
    $O/inet/mobility/group/MoBanCoordinator.o \
    $O/inet/mobility/group/MoBanLocal.o \
    $O/inet/mobility/group/Posture.o \
    $O/inet/mobility/group/PostureTransition.o \
    $O/inet/mobility/single/AnsimMobility.o \
    $O/inet/mobility/single/AttachedMobility.o \
    $O/inet/mobility/single/BonnMotionFileCache.o \
    $O/inet/mobility/single/BonnMotionMobility.o \
    $O/inet/mobility/single/ChiangMobility.o \
    $O/inet/mobility/single/CircleMobility.o \
    $O/inet/mobility/single/FacingMobility.o \
    $O/inet/mobility/single/GaussMarkovMobility.o \
    $O/inet/mobility/single/LinearMobility.o \
    $O/inet/mobility/single/MassMobility.o \
    $O/inet/mobility/single/Ns2MotionMobility.o \
    $O/inet/mobility/single/RandomWaypointMobility.o \
    $O/inet/mobility/single/RectangleMobility.o \
    $O/inet/mobility/single/SuperpositioningMobility.o \
    $O/inet/mobility/single/TractorMobility.o \
    $O/inet/mobility/single/TurtleMobility.o \
    $O/inet/mobility/single/VehicleMobility.o \
    $O/inet/mobility/static/StaticConcentricMobility.o \
    $O/inet/mobility/static/StaticGridMobility.o \
    $O/inet/mobility/static/StaticLinearMobility.o \
    $O/inet/mobility/static/StationaryMobility.o \
    $O/inet/networklayer/base/NetworkProtocolBase.o \
    $O/inet/networklayer/common/EchoPacketSerializer.o \
    $O/inet/networklayer/common/EchoProtocol.o \
    $O/inet/networklayer/common/EchoProtocolDissector.o \
    $O/inet/networklayer/common/EchoProtocolPrinter.o \
    $O/inet/networklayer/common/InterfaceMatcher.o \
    $O/inet/networklayer/common/InterfaceTable.o \
    $O/inet/networklayer/common/L3Address.o \
    $O/inet/networklayer/common/L3AddressResolver.o \
    $O/inet/networklayer/common/L3Tools.o \
    $O/inet/networklayer/common/ModuleIdAddress.o \
    $O/inet/networklayer/common/ModuleIdAddressType.o \
    $O/inet/networklayer/common/ModulePathAddress.o \
    $O/inet/networklayer/common/ModulePathAddressType.o \
    $O/inet/networklayer/common/NetfilterInfoHook.o \
    $O/inet/networklayer/common/NetworkInterface.o \
    $O/inet/networklayer/configurator/base/L3NetworkConfiguratorBase.o \
    $O/inet/networklayer/configurator/base/NetworkConfiguratorBase.o \
    $O/inet/networklayer/contract/IArp.o \
    $O/inet/networklayer/contract/IRoute.o \
    $O/inet/networklayer/contract/L3Socket.o \
    $O/inet/networklayer/contract/NetworkHeaderBase.o \
    $O/inet/networklayer/contract/clns/ClnsAddress.o \
    $O/inet/networklayer/contract/clns/ClnsAddressType.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Address.o \
    $O/inet/networklayer/contract/ipv4/Ipv4AddressType.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Socket.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Address.o \
    $O/inet/networklayer/contract/ipv6/Ipv6AddressType.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Socket.o \
    $O/inet/networks/base/NetworkBase.o \
    $O/inet/node/base/NodeBase.o \
    $O/inet/physicallayer/common/Signal.o \
    $O/inet/physicallayer/wired/common/WireJunction.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/INew.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/IRadio.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/IRadioMedium.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/IRadioSignal.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/ITracingObstacleLoss.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/ITransmission.o \
    $O/inet/power/base/CcEnergySinkBase.o \
    $O/inet/power/base/CcEnergySourceBase.o \
    $O/inet/power/base/CcEnergyStorageBase.o \
    $O/inet/power/base/EnergySinkBase.o \
    $O/inet/power/base/EnergySourceBase.o \
    $O/inet/power/base/EnergyStorageBase.o \
    $O/inet/power/base/EpEnergySinkBase.o \
    $O/inet/power/base/EpEnergySourceBase.o \
    $O/inet/power/base/EpEnergyStorageBase.o \
    $O/inet/power/consumer/AlternatingEpEnergyConsumer.o \
    $O/inet/power/contract/ICcEnergyConsumer.o \
    $O/inet/power/contract/ICcEnergyGenerator.o \
    $O/inet/power/contract/ICcEnergySink.o \
    $O/inet/power/contract/ICcEnergySource.o \
    $O/inet/power/contract/ICcEnergyStorage.o \
    $O/inet/power/contract/IEpEnergyConsumer.o \
    $O/inet/power/contract/IEpEnergyGenerator.o \
    $O/inet/power/contract/IEpEnergySink.o \
    $O/inet/power/contract/IEpEnergySource.o \
    $O/inet/power/contract/IEpEnergyStorage.o \
    $O/inet/power/generator/AlternatingEpEnergyGenerator.o \
    $O/inet/power/management/SimpleEpEnergyManagement.o \
    $O/inet/power/storage/IdealEpEnergyStorage.o \
    $O/inet/power/storage/SimpleCcBattery.o \
    $O/inet/power/storage/SimpleEpEnergyStorage.o \
    $O/inet/queueing/base/ActivePacketSinkBase.o \
    $O/inet/queueing/base/ActivePacketSourceBase.o \
    $O/inet/queueing/base/PacketBufferBase.o \
    $O/inet/queueing/base/PacketClassifierBase.o \
    $O/inet/queueing/base/PacketDuplicatorBase.o \
    $O/inet/queueing/base/PacketFilterBase.o \
    $O/inet/queueing/base/PacketFlowBase.o \
    $O/inet/queueing/base/PacketGateBase.o \
    $O/inet/queueing/base/PacketLabelerBase.o \
    $O/inet/queueing/base/PacketMarkerBase.o \
    $O/inet/queueing/base/PacketMeterBase.o \
    $O/inet/queueing/base/PacketProcessorBase.o \
    $O/inet/queueing/base/PacketPullerBase.o \
    $O/inet/queueing/base/PacketPusherBase.o \
    $O/inet/queueing/base/PacketQueueBase.o \
    $O/inet/queueing/base/PacketSchedulerBase.o \
    $O/inet/queueing/base/PacketServerBase.o \
    $O/inet/queueing/base/PacketSinkBase.o \
    $O/inet/queueing/base/PacketSourceBase.o \
    $O/inet/queueing/base/PacketTaggerBase.o \
    $O/inet/queueing/base/PassivePacketSinkBase.o \
    $O/inet/queueing/base/PassivePacketSourceBase.o \
    $O/inet/queueing/base/TokenGeneratorBase.o \
    $O/inet/queueing/common/OrdinalBasedDuplicator.o \
    $O/inet/queueing/common/PacketCloner.o \
    $O/inet/queueing/common/PacketDelayer.o \
    $O/inet/queueing/common/PacketDemultiplexer.o \
    $O/inet/queueing/common/PacketDuplicator.o \
    $O/inet/queueing/common/PacketMultiplexer.o \
    $O/inet/queueing/common/PacketPushToSend.o \
    $O/inet/queueing/common/PacketSendToPush.o \
    $O/inet/queueing/common/TokenBucket.o \
    $O/inet/queueing/function/PacketClassifierFunction.o \
    $O/inet/queueing/function/PacketComparatorFunction.o \
    $O/inet/queueing/function/PacketDropperFunction.o \
    $O/inet/queueing/function/PacketFilterFunction.o \
    $O/inet/queueing/function/PacketSchedulerFunction.o \
    $O/inet/routing/common/ManetProtocolDissector.o \
    $O/inet/routing/ospf_common/OspfPacketSerializer.o \
    $O/inet/routing/ospf_common/OspfProtocolDissector.o \
    $O/inet/transportlayer/base/TransportProtocolBase.o \
    $O/inet/transportlayer/common/CrcMode.o \
    $O/inet/transportlayer/common/L4Tools.o \
    $O/inet/transportlayer/common/TransportPseudoHeaderSerializer.o \
    $O/inet/transportlayer/contract/sctp/SctpSocket.o \
    $O/inet/transportlayer/contract/tcp/TcpSocket.o \
    $O/inet/transportlayer/contract/udp/UdpSocket.o \
    $O/inet/applications/base/ApplicationPacket_m.o \
    $O/inet/applications/netperfmeter/NetPerfMeter_m.o \
    $O/inet/applications/pingapp/PingApp_m.o \
    $O/inet/clock/contract/ClockTime_m.o \
    $O/inet/clock/contract/IClock_m.o \
    $O/inet/common/DirectionTag_m.o \
    $O/inet/common/FlowTag_m.o \
    $O/inet/common/IdentityTag_m.o \
    $O/inet/common/INETDefs_m.o \
    $O/inet/common/PacketEventTag_m.o \
    $O/inet/common/Protocol_m.o \
    $O/inet/common/ProtocolTag_m.o \
    $O/inet/common/SequenceNumberTag_m.o \
    $O/inet/common/Simsignals_m.o \
    $O/inet/common/TagBase_m.o \
    $O/inet/common/TimeTag_m.o \
    $O/inet/common/TlvOptions_m.o \
    $O/inet/common/Units_m.o \
    $O/inet/common/geometry/Geometry_m.o \
    $O/inet/common/packet/ChunkBuffer_m.o \
    $O/inet/common/packet/ChunkQueue_m.o \
    $O/inet/common/packet/Message_m.o \
    $O/inet/common/packet/Packet_m.o \
    $O/inet/common/packet/chunk/Chunk_m.o \
    $O/inet/common/scenario/ScenarioTimer_m.o \
    $O/inet/common/socket/SocketCommand_m.o \
    $O/inet/common/socket/SocketTag_m.o \
    $O/inet/linklayer/common/DropEligibleTag_m.o \
    $O/inet/linklayer/common/EtherType_m.o \
    $O/inet/linklayer/common/FcsMode_m.o \
    $O/inet/linklayer/common/Ieee802Ctrl_m.o \
    $O/inet/linklayer/common/Ieee802SapTag_m.o \
    $O/inet/linklayer/common/InterfaceTag_m.o \
    $O/inet/linklayer/common/MacAddress_m.o \
    $O/inet/linklayer/common/MacAddressTag_m.o \
    $O/inet/linklayer/common/PcpTag_m.o \
    $O/inet/linklayer/common/UserPriorityTag_m.o \
    $O/inet/linklayer/common/VlanTag_m.o \
    $O/inet/linklayer/ieee802/Ieee802EpdHeader_m.o \
    $O/inet/networklayer/common/DscpTag_m.o \
    $O/inet/networklayer/common/EchoPacket_m.o \
    $O/inet/networklayer/common/EcnTag_m.o \
    $O/inet/networklayer/common/FragmentationTag_m.o \
    $O/inet/networklayer/common/HopLimitTag_m.o \
    $O/inet/networklayer/common/IpProtocolId_m.o \
    $O/inet/networklayer/common/L3Address_m.o \
    $O/inet/networklayer/common/L3AddressTag_m.o \
    $O/inet/networklayer/common/MulticastTag_m.o \
    $O/inet/networklayer/common/NetworkInterface_m.o \
    $O/inet/networklayer/common/NextHopAddressTag_m.o \
    $O/inet/networklayer/common/TosTag_m.o \
    $O/inet/networklayer/contract/L3SocketCommand_m.o \
    $O/inet/networklayer/contract/NetworkHeaderBase_m.o \
    $O/inet/networklayer/contract/clns/ClnsAddress_m.o \
    $O/inet/networklayer/contract/ipv4/Ipv4Address_m.o \
    $O/inet/networklayer/contract/ipv4/Ipv4SocketCommand_m.o \
    $O/inet/networklayer/contract/ipv6/Ipv6Address_m.o \
    $O/inet/networklayer/contract/ipv6/Ipv6SocketCommand_m.o \
    $O/inet/physicallayer/common/Signal_m.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/IModulation_m.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/RadioControlInfo_m.o \
    $O/inet/physicallayer/wireless/common/contract/packetlevel/SignalTag_m.o \
    $O/inet/queueing/common/LabelsTag_m.o \
    $O/inet/queueing/common/ProgressTag_m.o \
    $O/inet/queueing/common/RateTag_m.o \
    $O/inet/routing/ospf_common/OspfPacketBase_m.o \
    $O/inet/transportlayer/common/CrcMode_m.o \
    $O/inet/transportlayer/common/L4PortTag_m.o \
    $O/inet/transportlayer/common/TransportPseudoHeader_m.o \
    $O/inet/transportlayer/contract/TransportHeaderBase_m.o \
    $O/inet/transportlayer/contract/sctp/SctpCommand_m.o \
    $O/inet/transportlayer/contract/tcp/TcpCommand_m.o \
    $O/inet/transportlayer/contract/udp/UdpControlInfo_m.o

# Message files
MSGFILES = \
    inet/applications/base/ApplicationPacket.msg \
    inet/applications/netperfmeter/NetPerfMeter.msg \
    inet/applications/pingapp/PingApp.msg \
    inet/clock/contract/ClockTime.msg \
    inet/clock/contract/IClock.msg \
    inet/common/DirectionTag.msg \
    inet/common/FlowTag.msg \
    inet/common/IdentityTag.msg \
    inet/common/INETDefs.msg \
    inet/common/PacketEventTag.msg \
    inet/common/Protocol.msg \
    inet/common/ProtocolTag.msg \
    inet/common/SequenceNumberTag.msg \
    inet/common/Simsignals.msg \
    inet/common/TagBase.msg \
    inet/common/TimeTag.msg \
    inet/common/TlvOptions.msg \
    inet/common/Units.msg \
    inet/common/geometry/Geometry.msg \
    inet/common/packet/ChunkBuffer.msg \
    inet/common/packet/ChunkQueue.msg \
    inet/common/packet/Message.msg \
    inet/common/packet/Packet.msg \
    inet/common/packet/chunk/Chunk.msg \
    inet/common/scenario/ScenarioTimer.msg \
    inet/common/socket/SocketCommand.msg \
    inet/common/socket/SocketTag.msg \
    inet/linklayer/common/DropEligibleTag.msg \
    inet/linklayer/common/EtherType.msg \
    inet/linklayer/common/FcsMode.msg \
    inet/linklayer/common/Ieee802Ctrl.msg \
    inet/linklayer/common/Ieee802SapTag.msg \
    inet/linklayer/common/InterfaceTag.msg \
    inet/linklayer/common/MacAddress.msg \
    inet/linklayer/common/MacAddressTag.msg \
    inet/linklayer/common/PcpTag.msg \
    inet/linklayer/common/UserPriorityTag.msg \
    inet/linklayer/common/VlanTag.msg \
    inet/linklayer/ieee802/Ieee802EpdHeader.msg \
    inet/networklayer/common/DscpTag.msg \
    inet/networklayer/common/EchoPacket.msg \
    inet/networklayer/common/EcnTag.msg \
    inet/networklayer/common/FragmentationTag.msg \
    inet/networklayer/common/HopLimitTag.msg \
    inet/networklayer/common/IpProtocolId.msg \
    inet/networklayer/common/L3Address.msg \
    inet/networklayer/common/L3AddressTag.msg \
    inet/networklayer/common/MulticastTag.msg \
    inet/networklayer/common/NetworkInterface.msg \
    inet/networklayer/common/NextHopAddressTag.msg \
    inet/networklayer/common/TosTag.msg \
    inet/networklayer/contract/L3SocketCommand.msg \
    inet/networklayer/contract/NetworkHeaderBase.msg \
    inet/networklayer/contract/clns/ClnsAddress.msg \
    inet/networklayer/contract/ipv4/Ipv4Address.msg \
    inet/networklayer/contract/ipv4/Ipv4SocketCommand.msg \
    inet/networklayer/contract/ipv6/Ipv6Address.msg \
    inet/networklayer/contract/ipv6/Ipv6SocketCommand.msg \
    inet/physicallayer/common/Signal.msg \
    inet/physicallayer/wireless/common/contract/packetlevel/IModulation.msg \
    inet/physicallayer/wireless/common/contract/packetlevel/RadioControlInfo.msg \
    inet/physicallayer/wireless/common/contract/packetlevel/SignalTag.msg \
    inet/queueing/common/LabelsTag.msg \
    inet/queueing/common/ProgressTag.msg \
    inet/queueing/common/RateTag.msg \
    inet/routing/ospf_common/OspfPacketBase.msg \
    inet/transportlayer/common/CrcMode.msg \
    inet/transportlayer/common/L4PortTag.msg \
    inet/transportlayer/common/TransportPseudoHeader.msg \
    inet/transportlayer/contract/TransportHeaderBase.msg \
    inet/transportlayer/contract/sctp/SctpCommand.msg \
    inet/transportlayer/contract/tcp/TcpCommand.msg \
    inet/transportlayer/contract/udp/UdpControlInfo.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
CONFIGFILE = $(shell opp_configfilepath)
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DINET_EXPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH) -PINET_API
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

# On Windows, the target has additional file(s). An import lib and an optional debug symbol file is created too.
ifeq ($(PLATFORM),win32)
  TARGET_FILES+= $(TARGET_DIR)/$(TARGET_IMPLIB)
  LDFLAGS+= $(LDFLAG_IMPLIB)$O/$(TARGET_IMPLIB)
  ifeq ($(TOOLCHAIN_NAME),clang-msabi)
    ifeq ($(MODE),debug)
      TARGET_FILES+=$(TARGET_DIR)/$(TARGET_NAME).pdb
    endif
  else
    TARGET_FILES+= $(TARGET_DIR)/$(TARGET_IMPDEF)
    LDFLAGS+= $(LDFLAG_IMPDEF)$O/$(TARGET_IMPDEF)
  endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
-include makefrag

#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null

$O/$(TARGET_NAME).pdb: $O/$(TARGET)
endif

$O/$(TARGET) $O/$(TARGET_IMPDEF) $O/$(TARGET_IMPLIB) &: $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating shared library: $@
	$(Q)$(SHLIB_LD) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS) $(LDFLAGS)
	$(Q)$(SHLIB_POSTPROCESS) $O/$(TARGET)
ifeq ($(PLATFORM),win32)
	$(Q)llvm-ar d $O/$(TARGET_IMPLIB) $(TARGET) # WORKAROUND: throw away the first file from the archive to make the LLD generated import lib valid
endif

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
