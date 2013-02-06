#ifndef _STATISTICS_NATION_HXX_
#define _STATISTICS_NATION_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsNation : public StatisticsType
{
public:
	static bool registerStatisticsNation();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsNation = StatisticsNation::
	registerStatisticsNation();

#endif
