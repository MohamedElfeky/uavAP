////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 University of Illinois Board of Trustees
//
// This file is part of uavAP.
//
// uavAP is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// uavAP is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
/*
 * RosComm.h
 *
 *  Created on: Nov 15, 2017
 *      Author: mircot
 */

#ifndef UAVAP_COMMUNICATION_COMM_MESSAGEQUEUECOMM_MESSAGEQUEUECOMM_H_
#define UAVAP_COMMUNICATION_COMM_MESSAGEQUEUECOMM_MESSAGEQUEUECOMM_H_
#include <boost/property_tree/ptree.hpp>

#include "uavAP/Communication/Comm/IComm.h"
#include "uavAP/Core/DataPresentation/Packet.h"
#include "uavAP/Core/IPC/IPC.h"
#include "uavAP/Core/Object/IAggregatableObject.h"
#include "uavAP/Core/Object/ObjectHandle.h"
#include "uavAP/Core/Runner/IRunnableObject.h"

#include <memory>

class IScheduler;
class DataPresentation;

class MessageQueueComm: public IComm, public IAggregatableObject, public IRunnableObject
{
public:

	static constexpr TypeId typeId = "message_queue";

	MessageQueueComm();

	static std::shared_ptr<MessageQueueComm>
	create(const Configuration& configuration);

	bool
	configure(const Configuration& configuration);

	bool
	run(RunStage stage) override;

	void
	notifyAggregationOnUpdate(const Aggregator& agg) override;

private:

	void
	sendPacket(const Packet& packet);

	void
	receivePacket(const Packet& packet);

	void
	tryConnectChannelMixing();

	void
	tryConnectGroundStation();

	ObjectHandle<IPC> ipc_;
	ObjectHandle<IScheduler> scheduler_;
	ObjectHandle<DataPresentation> dataPresentation_;

	Subscription flightAnalysisSubscription_;
	Subscription flightControlSubscription_;
	Subscription missionControlSubscription_;
	Subscription channelMixingSubscription_;
	Publisher<Packet> flightAnalysisPublisher_;
	Publisher<Packet> flightControlPublisher_;
	Publisher<Packet> missionControlPublisher_;
	Publisher<Packet> apiPublisher_;

	Subscription groundStationSubscription_;
	Publisher<Packet> groundStationPublisher_;

	bool groundStationConnected_;

	std::mutex senderMutex_;

	std::string serialPort_;
};

#endif /* UAVAP_COMMUNICATION_COMM_MESSAGEQUEUECOMM_MESSAGEQUEUECOMM_H_ */
