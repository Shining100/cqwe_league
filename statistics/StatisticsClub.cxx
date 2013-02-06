#include "statistics/StatisticsClub.hxx"
#include <cassert>
#include <sstream>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"
#include "statistics/Strip.hxx"

StatisticsType* createStatisticsClub()
{
	return new StatisticsClub;
}

bool StatisticsClub::registerStatisticsClub()
{
	return Singleton<StatisticsTypeFactory>::instance().registerType(
		"���ֲ�", createStatisticsClub);
}

bool StatisticsClub::statistics(const club_summaries &summaries,
								  const club_players &players, unsigned index,
								  StatisticsReport &report) const
{
	assert(summaries.size() == players.size());

	std::map<std::string, unsigned> mCount;
	club_summaries::const_iterator sIter = summaries.begin();
	club_players::const_iterator pIter = players.begin();
	while (summaries.end() != sIter)
	{
		unsigned totalClub = 0;
		players::const_iterator iter = pIter->begin();
		std::advance(iter, 2); // ȥ��2�б�ͷ
		while (iter != pIter->end())
		{
			mCount[(*iter)[index]]++;
			++iter;
		}

		++sIter;
		++pIter;
	}

	for (std::map<std::string, unsigned>::iterator iter = mCount.begin();
		iter != mCount.end(); ++iter)
	{
		StatisticsReport::summary summary;
		summary.push_back(iter->first);
		std::ostringstream oss;
		oss << iter->second;
		report.mData.insert(std::make_pair(oss.str(), summary));
	}

	return true;
}