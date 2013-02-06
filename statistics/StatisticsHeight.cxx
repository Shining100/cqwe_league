#include "statistics/StatisticsHeight.hxx"
#include <cassert>
#include <sstream>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"
#include "statistics/Strip.hxx"

StatisticsType* createStatisticsHeight()
{
	return new StatisticsHeight;
}

bool StatisticsHeight::registerStatisticsHeight()
{
	return Singleton<StatisticsTypeFactory>::instance().registerType(
		"���", createStatisticsHeight);
}

bool StatisticsHeight::statistics(const club_summaries &summaries,
							   const club_players &players, unsigned index,
							   StatisticsReport &report) const
{
	assert(summaries.size() == players.size());

	club_summaries::const_iterator sIter = summaries.begin();
	club_players::const_iterator pIter = players.begin();
	while (summaries.end() != sIter)
	{
		unsigned totalHeight = 0;
		players::const_iterator iter = pIter->begin();
		std::advance(iter, 2); // ȥ��2�б�ͷ
		while (iter != pIter->end())
		{
			totalHeight += static_cast<unsigned>(atoi((*iter)[index].c_str()));
			++iter;
		}

		StatisticsReport::summary summary;
		summary.push_back(stripClubName((*sIter)[0]));
		summary.push_back(stripCoachName((*sIter)[1]));
		std::ostringstream oss;
		oss << static_cast<float>(totalHeight) / static_cast<float>(
			(pIter->size() - 2));
		report.mData.insert(std::make_pair(oss.str(), summary));

		++sIter;
		++pIter;
	}

	return true;
}
