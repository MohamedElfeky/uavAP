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
 * SplineGlobalPlanner.h
 *
 *  Created on: Dec 16, 2017
 *      Author: mircot
 */

#ifndef UAVAP_MISSIONCONTROL_GLOBALPLANNER_SPLINEGLOBALPLANNER_SPLINEGLOBALPLANNER_H_
#define UAVAP_MISSIONCONTROL_GLOBALPLANNER_SPLINEGLOBALPLANNER_SPLINEGLOBALPLANNER_H_

#include <cpsCore/cps_object>

#include <cpsCore/Utilities/IPC/Publisher.h>
#include <uavAP/MissionControl/GlobalPlanner/SplineGlobalPlanner/SplineGlobalPlannerParams.h>
#include "uavAP/MissionControl/GlobalPlanner/IGlobalPlanner.h"
#include "uavAP/MissionControl/GlobalPlanner/Trajectory.h"

class ILocalPlanner;

class IPC;

class DataPresentation;

class SplineGlobalPlanner : public IGlobalPlanner,
							public AggregatableObject<ILocalPlanner, IPC, DataPresentation>,
							public ConfigurableObject<SplineGlobalPlannerParams>,
							public IRunnableObject
{
public:

	static constexpr TypeId typeId = "spline";

	SplineGlobalPlanner();

	static std::shared_ptr<IGlobalPlanner>
	create(const Configuration& config);

	bool
	run(RunStage stage) override;

	void
	setMission(const Mission& mission) override;

	Mission
	getMission() const override;

private:

	Trajectory
	createNaturalSplines(const Mission& mission);

	Trajectory
	createCatmulRomSplines(const Mission& mission);

	Mission mission_;

	Publisher<Packet> trajectoryPublisher_;

};

#endif /* UAVAP_MISSIONCONTROL_GLOBALPLANNER_SPLINEGLOBALPLANNER_SPLINEGLOBALPLANNER_H_ */
