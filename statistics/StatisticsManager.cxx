#include "statistics/StatisticsManager.hxx"
#include <memory>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsSalary.hxx"
#include "statistics/StatisticsAge.hxx"
#include "statistics/StatisticsHeight.hxx"
#include "statistics/StatisticsRate.hxx"
#include "statistics/StatisticsNation.hxx"
#include "statistics/StatisticsClub.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"

StatisticsManager::StatisticsManager(const std::string &fileName)
{
	if (!mParser.parse(fileName, false))
	{
		throw std::exception();
	}

	const club_players &clubPlayers = mParser.getPlayers();
	if (clubPlayers.empty())
	{
		throw std::exception();
	}

	const unsigned tableHeaderCount = 2;
	club_players::const_iterator cpIter = clubPlayers.begin();
	if (cpIter->empty())
	{
		throw std::exception();
	}

	players::const_iterator pIter = cpIter->begin();
	if (tableHeaderCount >= pIter->size())
	{
		throw std::exception();
	}
	++pIter; // 读取第二行表头

	for (unsigned i = 0; i < pIter->size(); ++i)
	{
		mTableIndex.insert(std::make_pair((*pIter)[i], i));
	}
}

bool StatisticsManager::statistics(const std::string &type,
	StatisticsReport &report) const
{
	table_index_map::const_iterator iter = mTableIndex.find(type);
	if (mTableIndex.end() == iter)
	{
		return false;
	}

	std::auto_ptr<StatisticsType> statisticsType(Singleton<
		StatisticsTypeFactory>::instance().create(type));
	statisticsType->statistics(mParser.getSummaries(), mParser.getPlayers(),
		iter->second, report);

	return true;
}
