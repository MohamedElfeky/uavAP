/*
 * AggregatableObjectImpl.hpp
 *
 *  Created on: Jun 12, 2019
 *      Author: mirco
 */

#ifndef UAVAP_CORE_OBJECT_AGGREGATABLEOBJECTIMPL_HPP_
#define UAVAP_CORE_OBJECT_AGGREGATABLEOBJECTIMPL_HPP_

#include <uavAP/Core/Object/AggregatableObject.hpp>
#include <uavAP/Core/Logging/APLogger.h>

template<class ... Objects>
inline void
AggregatableObject<Objects...>::notifyAggregationOnUpdate(const Aggregator& agg)
{
	container_.setFromAggregationIfNotSet(agg);
}

template<class ... Objects>
template<class Agg>
void
AggregatableObject<Objects...>::notifyAggregationOnUpdate(const Agg& agg)
{
	container_.setFromAggregationIfNotSet(agg);
}

template<class ... Objects>
template<class Ret>
inline typename AggregatableObject<Objects...>::template PtrType<Ret>
AggregatableObject<Objects...>::get() const
{
	return container_.template get<Ret>();
}

template<class ... Objects>
template<class Ret>
inline bool
AggregatableObject<Objects...>::isSet() const
{
	return container_.template isSet<Ret>();
}

template<class ... Objects>
template<class Check>
inline bool
AggregatableObject<Objects...>::checkIsSet() const
{
	if (isSet<Check>())
		return true;
	APLOG_ERROR << Check::typeId << " Missing";
	return false;
}

template<class ... Objects>
template<class Check, class More, class ... Others>
inline bool
AggregatableObject<Objects...>::checkIsSet() const
{
	return this->template checkIsSet<Check>() && this->template checkIsSet<More, Others...>();
}

#endif /* UAVAP_CORE_OBJECT_AGGREGATABLEOBJECTIMPL_HPP_ */
