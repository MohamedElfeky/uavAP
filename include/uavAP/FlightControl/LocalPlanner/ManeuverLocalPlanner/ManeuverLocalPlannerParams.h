/*
 * ManeuverLocalPlannerParams.h
 *
 *  Created on: Jun 28, 2019
 *      Author: mirco
 */

#ifndef UAVAP_FLIGHTCONTROL_LOCALPLANNER_MANEUVERLOCALPLANNER_MANEUVERLOCALPLANNERPARAMS_H_
#define UAVAP_FLIGHTCONTROL_LOCALPLANNER_MANEUVERLOCALPLANNER_MANEUVERLOCALPLANNERPARAMS_H_
#include "uavAP/Core/LinearAlgebra.h"
#include "uavAP/Core/PropertyMapper/Parameter.h"


struct ManeuverLocalPlannerParams
{
	Parameter<FloatingType> kAltitude = {1.0, "k_altitude", true};
	Parameter<FloatingType> kConvergence = {1.0, "k_convergence", true};
	Parameter<FloatingType> kYawRate = {1.0, "k_yaw_rate", true};
	Parameter<FloatingType> safetyVelocity = {25.0, "safety_velocity", true};
	Parameter<FloatingType> safetyYawRate = {0.0, "safety_yaw_rate", false};
	Parameter<int> period = {0, "period", false};

	template <class Configurator>
	inline void
	configure(Configurator& c)
	{
		c & kAltitude;
		c & kConvergence;
		c & kYawRate;
		c & safetyVelocity;
		c & safetyYawRate;
		c & period;
	}
};




#endif /* UAVAP_FLIGHTCONTROL_LOCALPLANNER_MANEUVERLOCALPLANNER_MANEUVERLOCALPLANNERPARAMS_H_ */
