#ifndef _STATISTICS_CLUB_HXX_
#define _STATISTICS_CLUB_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsClub : public StatisticsType
{
public:
	static bool registerStatisticsClub();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsClub = StatisticsClub::
	registerStatisticsClub();

#endif
