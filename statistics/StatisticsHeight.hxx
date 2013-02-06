#ifndef _STATISTICS_HEIGHT_HXX_
#define _STATISTICS_HEIGHT_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsHeight : public StatisticsType
{
public:
	static bool registerStatisticsHeight();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsHeight = StatisticsHeight::
	registerStatisticsHeight();

#endif
