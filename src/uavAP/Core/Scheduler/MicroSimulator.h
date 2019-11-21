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
 * MicroSimulator.h
 *
 *  Created on: Jun 29, 2017
 *      Author: mircot
 */

#ifndef UAVAP_CORE_SCHEDULER_MICROSIMULATOR_H_
#define UAVAP_CORE_SCHEDULER_MICROSIMULATOR_H_

#include <boost/thread.hpp>
#include "uavAP/Core/Object/AggregatableObject.hpp"
#include "uavAP/Core/Scheduler/IScheduler.h"
#include "uavAP/Core/Scheduler/MicroSimulatorParams.h"
#include "uavAP/Core/Time.h"
#include "uavAP/Core/TimeProvider/ITimeProvider.h"
#include "uavAP/Core/PropertyMapper/ConfigurableObject.hpp"
#include <condition_variable>
#include <functional>
#include <map>
#include "uavAP/Core/LockTypes.h"

class MicroSimulator
		: public IScheduler,
		  public ITimeProvider,
		  public AggregatableObject<>,
		  public ConfigurableObject<MicroSimulatorParams>
{
public:

	static constexpr TypeId typeId = "micro_sim";

	MicroSimulator();

	Event
	schedule(const std::function<void
			()>& task, Duration initialFromNow) override;

	Event
	schedule(const std::function<void
			()>& task, Duration initialFromNow, Duration period) override;

	void
	stop() override;

	void
	setMainThread() override;

	void
	startSchedule() override;

	TimePoint
	now() override;

	bool
	waitFor(Duration duration, std::condition_variable& interrupt,
			std::unique_lock<std::mutex>& lock) override;

	bool
	waitUntil(TimePoint timePoint, std::condition_variable& interrupt,
			  std::unique_lock<std::mutex>& lock) override;

	int
	simulate(Duration duration);

	void
	stopOnWait();

	void
	releaseWait();

	void
	clearSchedule();

	bool
	isStopped();


private:

	std::multimap<TimePoint, std::shared_ptr<EventBody> > events_;

	TimePoint now_;
	int runs_;
	bool stopOnWait_;

	Mutex waitCondMutex_;
	std::condition_variable* waitCondition_;
	bool waitReleased_;
	bool stopped_;
};

#endif /* UAVAP_CORE_SCHEDULER_MICROSIMULATOR_H_ */
