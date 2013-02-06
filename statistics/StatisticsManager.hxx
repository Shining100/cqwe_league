#ifndef _STATISTICS_MANAGER_HXX_
#define _STATISTICS_MANAGER_HXX_

#include <map>
#include <string>
#include "statistics/leagueParser.hxx"

class StatisticsReport;

class StatisticsManager
{
	typedef std::vector<std::string> meta_datas;
	typedef std::list<meta_datas> club_summaries;
	typedef std::list<meta_datas> players;
	typedef std::list<players> club_players;
	typedef std::map<std::string, unsigned> table_index_map;

public:
	explicit StatisticsManager(const std::string &fileName);

private:
	StatisticsManager(const StatisticsManager&);
	StatisticsManager& operator=(const StatisticsManager&);

public:
	bool statistics(const std::string &type, StatisticsReport &report) const;

private:
	LeagueParser mParser;
	table_index_map mTableIndex;
};

#endif
