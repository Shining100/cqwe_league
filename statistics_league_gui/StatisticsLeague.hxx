#ifndef _STATISTICS_LEAGUE_HXX_
#define _STATISTICS_LEAGUE_HXX_

#include <map>
#include <string>
#include <memory>

class StatisticsManager;
class StatisticsReport;

class StatisticsLeague
{
private:
	StatisticsLeague() {}
	StatisticsLeague(const StatisticsLeague&);
	StatisticsLeague& operator=(const StatisticsLeague&);
	template <class T> friend class Singleton;

public:
	bool statistics(const std::string &season, const std::string &type);

private:
	std::shared_ptr<StatisticsManager> createManager(
		const std::string &season);
	bool output(const StatisticsReport &report, const std::string &season,
		const std::string &type) const;

private:
	typedef std::map<std::string, std::shared_ptr<StatisticsManager> >
		manager_map;
	manager_map mManagers;
};

#endif
