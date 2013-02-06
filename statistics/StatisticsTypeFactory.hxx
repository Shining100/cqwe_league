#ifndef _STATISTICS_TYPE_FACTORY_HXX_
#define _STATISTICS_TYPE_FACTORY_HXX_

#include <map>
#include <string>

class StatisticsType;

class StatisticsTypeFactory
{
	typedef StatisticsType* (*createStatisticsType)();
	typedef std::map<std::string, createStatisticsType> creator_map;

private:
	StatisticsTypeFactory(){}
	StatisticsTypeFactory(const StatisticsTypeFactory&);
	StatisticsTypeFactory& operator=(const StatisticsTypeFactory&);
	template <class T> friend class Singleton;

public:
	bool registerType(const std::string &identifier,
		createStatisticsType creator);
	StatisticsType* create(const std::string &identifier) const;

private:
	creator_map mCreators;
};

#endif
