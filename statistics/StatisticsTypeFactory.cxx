#include "statistics/StatisticsTypeFactory.hxx"
#include <cassert>

bool StatisticsTypeFactory::registerType(const std::string &identifier,
	createStatisticsType creator)
{
	mCreators[identifier] = creator;
	return true;
}

StatisticsType*
StatisticsTypeFactory::create(const std::string &identifier) const
{
	creator_map::const_iterator iter = mCreators.find(identifier);
	assert(mCreators.end() != iter);
	return iter->second();
}
