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
 * WindAnalysis.cpp
 *
 *  Created on: Oct 19, 2019
 *      Author: simonyu
 */

#include <uavAP/Core/IPC/IPC.h>
#include <uavAP/Core/Object/AggregatableObjectImpl.hpp>
#include "uavAP/MissionControl/WindAnalysis/WindAnalysis.h"

std::shared_ptr<WindAnalysis>
WindAnalysis::create(const Configuration& config)
{
	return std::make_shared<WindAnalysis>();
}

bool
WindAnalysis::configure(const Configuration& config)
{
	return true;
}

bool
WindAnalysis::run(RunStage stage)
{
	switch (stage)
	{
	case RunStage::INIT:
	{
		if (!isSet<IPC>())
		{
			APLOG_ERROR << "ManualWindAnalysis: IPC missing.";
			return true;
		}

		break;
	}
	case RunStage::NORMAL:
	{
		break;
	}
	case RunStage::FINAL:
	{
		break;
	}
	default:
		break;
	}
	return false;
}

WindInfo
WindAnalysis::getWindInfo() const
{
	LockGuard lg(windInfoMutex_);
	return windInfo_;
}

WindAnalysisStatus
WindAnalysis::getWindAnalysisStatus() const
{
	LockGuard lg(windAnalysisStatusMutex_);
	return windAnalysisStatus_;
}

void
WindAnalysis::setWindAnalysisStatus(const WindAnalysisStatus& windAnalysisStatus)
{
	Lock windAnalysisStatusLock(windAnalysisStatusMutex_);
	windAnalysisStatus_ = windAnalysisStatus;
	windAnalysisStatusLock.unlock();

	if (windAnalysisStatus.manual)
	{
		Lock windInfoLock(windInfoMutex_);
		windInfo_.velocity = windAnalysisStatus.velocity;
		windInfoLock.unlock();
	}
}
