/*
 * DataHandling.h
 *
 *  Created on: Feb 13, 2019
 *      Author: mirco
 */

#ifndef UAVAP_CORE_DATAHANDLING_H_
#define UAVAP_CORE_DATAHANDLING_H_

#include <map>

#include <cpsCore/cps_object>
#include <cpsCore/Utilities/DataPresentation/DataPresentation.h>
#include <cpsCore/Utilities/IPC/IPC.h>

#include "uavAP/Core/DataHandling/DataHandlingParams.h"
#include "uavAP/Core/DataHandling/Content.hpp"

class IScheduler;

class DataHandling: public AggregatableObject<DataPresentation, IScheduler, IPC>,
		public ConfigurableObject<DataHandlingParams>,
		public IRunnableObject
{
public:

	static constexpr TypeId typeId = "data_handling";

	DataHandling() = default;

	template<typename Type>
	void
	subscribeOnCommand(Content content, std::function<void
	(const Type&)> commandFunc);

	template<typename Type>
	void
	addStatusFunction(std::function<Type
	()> statusFunc, Content content);

	template<typename Type, typename TriggerType>
	void
	addTriggeredStatusFunction(std::function<Optional<Type>
	(const TriggerType&)> statusFunc, Content statusContent, Content TriggerCommand);

	template<typename Type>
	void
	sendData(const Type& data, Content content, Target target = Target::BROADCAST);

	bool
	run(RunStage stage) override;

private:

	void
	onPacket(const Packet& packet);

	void
	sendStatus();

	template<typename Type>
	Packet
	createPacket(std::function<Type
	()> statusFunc, Content content);

	template<typename Type>
	void
	forwardCommand(const Packet& packet, std::function<void
	(const Type&)> callback);

	template<typename Type, typename TriggerType>
	void
	evaluateTrigger(const TriggerType& any, std::function<Optional<Type>
	(const TriggerType&)> callback, Content statusContent);

	std::vector<std::function<Packet
	()>> statusPackaging_;

	std::map<Content, std::vector<std::function<void
	(const Packet&)>>> subscribers_;

	Publisher<Packet> publisher_;
};

template<typename Type>
inline void
DataHandling::addStatusFunction(std::function<Type
()> statusFunc, Content content)
{
	auto func = std::bind(&DataHandling::createPacket<Type>, this, statusFunc, content);
	statusPackaging_.push_back(func);
}

template<typename Type>
inline Packet
DataHandling::createPacket(std::function<Type
()> statusFunc, Content content)
{
	auto status = statusFunc();
	auto dp = get<DataPresentation>();
	if (!dp)
	{
		CPSLOG_ERROR << "Data Presentation Missing, cannot create packet.";
		return Packet();
	}
	auto packet = dp->serialize(status);
	dp->addHeader(packet, content);
	return packet;

}

template<typename Type>
inline void
DataHandling::subscribeOnCommand(Content content, std::function<void
(const Type&)> commandFunc)
{
	auto func = std::bind(&DataHandling::forwardCommand<Type>, this, std::placeholders::_1,
			commandFunc);
	auto it = subscribers_.find(content);
	if (it != subscribers_.end())
	{
		it->second.push_back(func);
		return;
	}

	std::vector<std::function<void
	(const Packet&)>> vec;
	vec.push_back(func);
	subscribers_.insert(std::make_pair(content, vec));
}

template<typename Type>
inline void
DataHandling::forwardCommand(const Packet& packet, std::function<void
(const Type&)> callback)
{
	auto dp = get<DataPresentation>();
	callback(dp->deserialize<Type>(packet));
}

template<typename Type, typename TriggerType>
inline void
DataHandling::addTriggeredStatusFunction(std::function<Optional<Type>
(const TriggerType&)> statusFunc, Content statusContent, Content triggerCommand)
{
	std::function<void
	(const TriggerType&)> func = std::bind(&DataHandling::evaluateTrigger<Type, TriggerType>, this,
			std::placeholders::_1, statusFunc, statusContent);
	subscribeOnCommand(triggerCommand, func);
}

template<typename Type, typename TriggerType>
inline void
DataHandling::evaluateTrigger(const TriggerType& trigger, std::function<Optional<Type>
(const TriggerType&)> callback, Content statusContent)
{
	auto status = callback(trigger);
	if (status)
	{
		auto dp = get<DataPresentation>();
		if (!dp)
		{
			CPSLOG_ERROR << "Data Presentation Missing, cannot create packet.";
			return;
		}
		auto packet = dp->serialize(*status);
		dp->addHeader(packet, statusContent);
		publisher_.publish(packet);
	}
}

template<typename Type>
inline void
DataHandling::sendData(const Type& data, Content content, Target target)
{
	auto dp = get<DataPresentation>();
	if (!dp)
	{
		CPSLOG_ERROR << "Data Presentation Missing, cannot create packet.";
		return;
	}

	Packet packet = dp->serialize(data);
	dp->addHeader(packet, content);
	dp->addHeader(packet, target);
	publisher_.publish(packet);

}

#endif /* UAVAP_CORE_DATAHANDLING_H_ */
