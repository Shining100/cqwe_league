#include "statistics/StatisticsSalary.hxx"
#include <cassert>
#include <sstream>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"
#include "statistics/Strip.hxx"

StatisticsType* createStatisticsSalary()
{
	return new StatisticsSalary;
}

bool StatisticsSalary::registerStatisticsSalary()
{
	return Singleton<StatisticsTypeFactory>::instance().registerType(
		"工资", createStatisticsSalary);
}

bool StatisticsSalary::statistics(const club_summaries &summaries,
	const club_players &players, unsigned index,
	StatisticsReport &report) const
{
	assert(summaries.size() == players.size());

	club_summaries::const_iterator sIter = summaries.begin();
	club_players::const_iterator pIter = players.begin();
	while (summaries.end() != sIter)
	{
		unsigned totalSalary = 0;
		players::const_iterator iter = pIter->begin();
		std::advance(iter, 2); // 去掉2行表头
		while (iter != pIter->end())
		{
			totalSalary += static_cast<unsigned>(atoi((*iter)[index].c_str()));
			++iter;
		}

		StatisticsReport::summary summary;
		summary.push_back(stripClubName((*sIter)[0]));
		summary.push_back(stripCoachName((*sIter)[1]));
		std::ostringstream oss;
		oss << totalSalary;
		report.mData.insert(std::make_pair(std::move(oss.str()),
			std::move(summary)));

		++sIter;
		++pIter;
	}

	return true;
}
