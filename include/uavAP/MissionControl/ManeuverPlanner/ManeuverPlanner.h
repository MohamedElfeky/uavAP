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
 * ManeuverPlanner.h
 *
 *  Created on: Aug 12, 2018
 *      Author: simonyu
 */

#ifndef UAVAP_MISSIONCONTROL_MANEUVERPLANNER_MANEUVERPLANNER_H_
#define UAVAP_MISSIONCONTROL_MANEUVERPLANNER_MANEUVERPLANNER_H_

#include <string>
#include <unordered_map>

#include <cpsCore/cps_object>

#include <uavAP/MissionControl/Geofencing/Rectanguloid.h>
#include <uavAP/MissionControl/ManeuverPlanner/ManeuverPlannerParams.h>
#include "uavAP/FlightAnalysis/ManeuverAnalysis/ManeuverAnalysisStatus.h"
#include "uavAP/FlightControl/Controller/AdvancedControl.h"
#include "uavAP/MissionControl/ManeuverPlanner/ManeuverSet.h"
#include "uavAP/MissionControl/ManeuverPlanner/Override.h"

class IPC;
class ConditionManager;
class RectanguloidCondition;
class DataPresentation;

class ManeuverPlanner: public AggregatableObject<IPC, ConditionManager, DataPresentation>,
		public IRunnableObject,
		public ConfigurableObject<ManeuverPlannerParams>
{
public:

	static constexpr TypeId typeId = "maneuver_planner";

	ManeuverPlanner();

	static std::shared_ptr<ManeuverPlanner>
	create(const Configuration& config);

	bool
	configure(const Configuration& config);

	bool
	run(RunStage stage) override;

	void
	setManualOverride(const Override& override);

	void
	setManeuverOverride(const std::string& maneuverSet);

	void
	setControllerOutputOffset(const ControllerOutput& offset);

	void
	interruptOverride();

	void
	resumeOverride();

	Override
	getOverride() const;

	unsigned int
	getOverrideNr() const;

	Rectanguloid
	getSafetyBounds() const;

	ControllerOutput
	getControllerOutputTrim() const;

private:

	void
	setOverride(const std::string& maneuverSet, const bool& manualActive,
			const bool& maneuverActive);

	void
	startManeuverAnalysis();

	void
	stopManeuverAnalysis();

	void
	startOverride();

	void
	stopOverride();

	void
	nextManeuverOverride();

	void
	activateManeuverOverride(const ICondition::ConditionTrigger& conditionTrigger);

	void
	deactivateManeuverOverride();

	void
	safetyTrigger(int trigger);

	void
	overrideControllerOutput(Override& override,
			const std::map<ControllerOutputs, bool>& overrideMap,
			const ControllerOutput& outputOverride);

	void
	onControllerOutput(const ControllerOutput& packet);

	void
	onControllerOutputTrim(const ControllerOutput& packet);

	void
	onAdvancedControl(const AdvancedControl& advanced);

	using ManeuverSetMap = std::unordered_map<std::string, ManeuverSet>;

	Override override_;
	mutable Mutex overrideMutex_;

	AdvancedControl advancedControl_;
	mutable Mutex advancedControlMutex_;

	bool maneuverAnalysis_;
	mutable std::mutex maneuverAnalysisMutex_;

	ManeuverAnalysisStatus maneuverAnalysisStatus_;
	mutable std::mutex maneuverAnalysisStatusMutex_;

	bool trimAnalysis_;
	mutable std::mutex trimAnalysisMutex_;

	ControllerOutput controllerOutput_;
	mutable Mutex controllerOutputMutex_;

	ControllerOutput controllerOutputTrim_;
	mutable std::mutex controllerOutputTrimMutex_;

	ControllerOutput controllerOutputOffset_;
	mutable std::mutex controllerOutputOffsetMutex_;

	ManeuverSetMap maneuverSetMap_;
	ManeuverSetMap::const_iterator currentManeuverSet_;
	boost::optional<ManeuverSet::const_iterator> currentManeuver_;

	std::string maneuverSet_;
	bool overrideInterrupted_;
	bool manualActive_;
	bool maneuverActive_;

	Override lastManualOverride_;
	std::string lastManeuverSet_;
	boost::optional<ManeuverSet::const_iterator> lastManeuver_;
	bool lastManualActive_;
	bool lastManeuverActive_;

	bool manualRestart_;
	bool maneuverRestart_;

	unsigned int overrideSeqNr_;
	mutable Mutex overrideSeqNrMutex_;

	std::shared_ptr<RectanguloidCondition> safetyCondition_;

	Publisher<Packet> overridePublisher_;
	Publisher<bool> maneuverAnalysisPublisher_;
	Publisher<AdvancedControl> advancedControlPublisher_;
	Publisher<Packet> maneuverAnalysisStatusPublisher_;
	Publisher<bool> trimAnalysisPublisher_;
	Subscription controllerOutputSubscription_;
	Subscription controllerOutputTrimSubscription_;
	Subscription advancedControlSubscription_;

};

#endif /* UAVAP_MISSIONCONTROL_MANEUVERPLANNER_MANEUVERPLANNER_H_ */
