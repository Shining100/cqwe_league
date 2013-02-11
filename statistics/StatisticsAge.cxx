#include "statistics/StatisticsAge.hxx"
#include <cassert>
#include <sstream>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"
#include "statistics/Strip.hxx"

StatisticsType* createStatisticsAge()
{
	return new StatisticsAge;
}

bool StatisticsAge::registerStatisticsAge()
{
	return Singleton<StatisticsTypeFactory>::instance().registerType(
		"生年", createStatisticsAge);
}

bool StatisticsAge::statistics(const club_summaries &summaries,
								  const club_players &players, unsigned index,
								  StatisticsReport &report) const
{
	assert(summaries.size() == players.size());

	club_summaries::const_iterator sIter = summaries.begin();
	club_players::const_iterator pIter = players.begin();
	while (summaries.end() != sIter)
	{
		unsigned totalAge = 0;
		players::const_iterator iter = pIter->begin();
		std::advance(iter, 2); // 去掉2行表头
		while (iter != pIter->end())
		{
			totalAge += static_cast<unsigned>(atoi((*iter)[index].c_str()));
			++iter;
		}

		StatisticsReport::summary summary;
		summary.push_back(stripClubName((*sIter)[0]));
		summary.push_back(stripCoachName((*sIter)[1]));
		std::ostringstream oss;
		oss << static_cast<float>(totalAge) / static_cast<float>(
			(pIter->size() - 2));
		report.mData.insert(std::make_pair(std::move(oss.str()),
			std::move(summary)));

		++sIter;
		++pIter;
	}

	return true;
}
