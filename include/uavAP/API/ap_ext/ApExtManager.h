/*
 * ApExtManager.h
 *
 *  Created on: Aug 31, 2017
 *      Author: mircot
 */

#ifndef UAVAP_API_APEXTMANAGER_H_
#define UAVAP_API_APEXTMANAGER_H_
#include <uavAP/FlightControl/Controller/ControlElements/Filter/LowPassFilter.h>
#include "uavAP/API/AutopilotAPI.hpp"
#include "uavAP/API/ap_ext/ap_ext.h"
#include "uavAP/API/ChannelMixing.h"
#include "uavAP/FlightControl/Controller/AdvancedControl.h"
#include "uavAP/FlightControl/Controller/ControllerOutput.h"
#include <mutex>
#include <functional>

class ApExtManager
{
public:

	ApExtManager();

	bool
	configure(const Configuration& config);

	int
	ap_sense(const data_sample_t* sample);

	int
	ap_actuate(unsigned long* pwm, unsigned int num_channels);

	struct OutPWM
	{
		unsigned long ch[7];
	};

	struct PWMFeedback
	{
		unsigned long ch[PWM_CHS];
	};

private:

	void
	onControllerOutput(const ControllerOutput& output);

	void
	onAdvancedControl(const AdvancedControl& control);

	AutopilotAPI uavapAPI_;

	ChannelMixing channelMixing_;

	std::mutex advancedControlMutex_;
	AdvancedControl lastAdvancedControl_;

	std::mutex outputMutex_;
	std::vector<unsigned long> outputChannels_;

	boost::optional<Eigen::Quaterniond> rotationOffset_;
	bool internalImu_;
	bool externalGps_;
	bool useAirspeed_;
	bool useEuler_;
	bool traceSeqNr_;
	bool courseAsHeading_;
	Duration gpsTimeout_;
	Duration airspeedTimeout_;
	unsigned int downsample_;

	Control::LowPassFilter airspeedFilter_;

	TimePoint gpsSampleTimestamp_;
	PUBX_POS_fields lastGPSSample_;

	TimePoint airspeedTimestamp_;
	airs_sample_t lastAirspeedSample_;

	unsigned int sampleNr_;

	Vector3 lastPosition_;
	Vector3 lastVelocity_;
};

#endif /* UAVAP_API_APEXTMANAGER_H_ */
