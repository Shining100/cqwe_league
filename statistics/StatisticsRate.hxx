#ifndef _STATISTICS_RATE_HXX_
#define _STATISTICS_RATE_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsRate : public StatisticsType
{
public:
	static bool registerStatisticsRate();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsRate = StatisticsRate::
	registerStatisticsRate();

#endif
