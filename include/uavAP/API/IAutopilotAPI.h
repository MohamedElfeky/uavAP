//
// Created by seedship on 7/3/20.
//

#ifndef UAVAP_IAUTOPILOTAPI_H
#define UAVAP_IAUTOPILOTAPI_H

#include <boost/signals2.hpp>

class SensorData;

class ServoData;

class PowerData;

class ControllerOutput;

class AdvancedControl;

class IAutopilotAPI
{
public:

	using OnControllerOut = boost::signals2::signal<void(const ControllerOutput&)>;
	using OnAdvancedControl = boost::signals2::signal<void(const AdvancedControl&)>;

	virtual boost::signals2::connection
	subscribeOnControllerOut(const OnControllerOut::slot_type& slot) = 0;

	virtual boost::signals2::connection
	subscribeOnAdvancedControl(const OnAdvancedControl::slot_type& slot) = 0;

	virtual void
	setSensorData(const SensorData& sd) = 0;

	virtual void
	setServoData(const ServoData& sd) = 0;

	virtual void
	setPowerData(const PowerData& pd) = 0;
};

#endif //UAVAP_IAUTOPILOTAPI_H
