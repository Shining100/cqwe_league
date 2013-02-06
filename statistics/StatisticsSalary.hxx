#ifndef _STATISTICS_SALARY_HXX_
#define _STATISTICS_SALARY_HXX_

#include "statistics/StatisticsType.hxx"

class StatisticsSalary : public StatisticsType
{
public:
	static bool registerStatisticsSalary();

public:
	virtual bool statistics(const club_summaries &summaries,
		const club_players &players, unsigned index,
		StatisticsReport &report) const;
};

static bool invokeRegisterStatisticsSalary = StatisticsSalary::
	registerStatisticsSalary();

#endif
