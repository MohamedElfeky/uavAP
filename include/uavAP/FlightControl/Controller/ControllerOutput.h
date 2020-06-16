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
/**
 *  @file         ControllerOutput.h
 *  @author Simon Yu, Mirco Theile
 *  @date      26 June 2017
 *  @brief      UAV Autopilot Controller Output Header File
 *
 *  Description
 */

#ifndef UAVAP_FLIGHTCONTROL_CONTROLLER_CONTROLLEROUTPUT_H_
#define UAVAP_FLIGHTCONTROL_CONTROLLER_CONTROLLEROUTPUT_H_

#include <cpsCore/Utilities/LinearAlgebra.h>
#include <cpsCore/Utilities/EnumMap.hpp>
#include <cpsCore/Utilities/DataPresentation/detail/SerializeCustom.h>

enum class ControllerOutputs
{
	INVALID, ROLL, PITCH, YAW, THROTTLE, NUM_OUTPUT
};

enum class ControllerOutputsWaveforms
{
	INVALID, ROLL, PITCH, YAW, THROTTLE, NUM_OUTPUT
};

enum class ControllerOutputsOverrides
{
	INVALID, FREEZE, TRIM, OFFSET, NUM_OVERRIDE
};

enum class Waveforms
{
	INVALID, NONE, SINE, SQUARE, RAMP, SAWTOOTH, TRIANGULAR, NUM_WAVEFORM
};

ENUMMAP_INIT(ControllerOutputs, { {ControllerOutputs::ROLL, "roll"}, {ControllerOutputs::PITCH,
		"pitch"}, {ControllerOutputs::YAW, "yaw"}, {ControllerOutputs::THROTTLE, "throttle"} });

ENUMMAP_INIT(ControllerOutputsWaveforms, { {ControllerOutputsWaveforms::ROLL, "roll"},
		{ControllerOutputsWaveforms::PITCH, "pitch"}, {ControllerOutputsWaveforms::YAW, "yaw"},
		{ControllerOutputsWaveforms::THROTTLE, "throttle"} });

ENUMMAP_INIT(ControllerOutputsOverrides, { {ControllerOutputsOverrides::FREEZE, "freeze"},
		{ControllerOutputsOverrides::TRIM, "trim"}, {ControllerOutputsOverrides::OFFSET,
		"offset"} });

ENUMMAP_INIT(Waveforms, { {Waveforms::NONE, "none"}, {Waveforms::SINE, "sine"}, {Waveforms::SQUARE,
		"square"}, {Waveforms::RAMP, "ramp"}, {Waveforms::SAWTOOTH, "sawtooth"},
		{Waveforms::TRIANGULAR, "triangular"} });

struct ControllerOutput : SerializeCustom
{
	FloatingType rollOutput;
	FloatingType pitchOutput;
	FloatingType yawOutput;
	FloatingType throttleOutput;

//	uint32_t sequenceNr; //Trace sequence number to get timing

	ControllerOutput() :
			rollOutput(0), pitchOutput(0), yawOutput(0), throttleOutput(-1)
//			, sequenceNr(0)
	{
	}
};

namespace dp
{
template<class Archive, typename Type>
inline void
serialize(Archive& ar, ControllerOutput& t)
{
	ar & t.rollOutput;
	ar & t.pitchOutput;
	ar & t.yawOutput;
	ar & t.throttleOutput;
//	ar & t.sequenceNr;
}
}

#endif /* UAVAP_FLIGHTCONTROL_CONTROLLER_CONTROLLEROUTPUT_H_ */
