#ifndef _STATISTICS_TYPE_HXX_
#define _STATISTICS_TYPE_HXX_

#include <vector>
#include <list>
#include <string>
#include <map>

class StatisticsReport;

class StatisticsType
{
protected:
	typedef std::vector<std::string> meta_datas;
	typedef std::list<meta_datas> club_summaries;
	typedef std::list<meta_datas> players;
	typedef std::list<players> club_players;

public:
	virtual ~StatisticsType() {}

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const = 0;
};

#endif
