#ifndef _STATISTIC_AGE_HXX_
#define _STATISTIC_AGE_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsAge : public StatisticsType
{
public:
	static bool registerStatisticsAge();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsAge = StatisticsAge::
	registerStatisticsAge();

#endif
