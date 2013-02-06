#include "statistics/StatisticsRate.hxx"
#include <cassert>
#include <sstream>
#include <algorithm>
#include "statistics/StatisticsType.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/StatisticsTypeFactory.hxx"
#include "statistics/Singleton.hxx"
#include "statistics/Strip.hxx"

StatisticsType* createStatisticsRate()
{
	return new StatisticsRate;
}

bool StatisticsRate::registerStatisticsRate()
{
	return Singleton<StatisticsTypeFactory>::instance().registerType(
		"总评", createStatisticsRate);
}

bool StatisticsRate::statistics(const club_summaries &summaries,
								  const club_players &players, unsigned index,
								  StatisticsReport &report) const
{
	assert(summaries.size() == players.size());

	club_summaries::const_iterator sIter = summaries.begin();
	club_players::const_iterator pIter = players.begin();
	while (summaries.end() != sIter)
	{
		unsigned totalRate = 0;
		players::const_iterator iter = pIter->begin();
		std::advance(iter, 2); // 去掉2行表头
		while (iter != pIter->end())
		{
			const std::string tag("★");
			unsigned rate = 0;

			if (tag == (*iter)[index]) // <td align="right"><font size="2">★<font color="#FFFF00">88</font></font></td>
			{
				rate = static_cast<unsigned>(atoi((*iter)[index + 1].c_str()));
			}
			else if(0 == (*iter)[index].find(tag)) // <td align="right"><font size="2">★68</font></td>
			{
				const char *str = (*iter)[index].c_str() + tag.size();
				rate = static_cast<unsigned>(atoi(str));
			}
			else if(std::string::npos == (*iter)[index].find(tag))
			{
				rate = static_cast<unsigned>(atoi((*iter)[index].c_str()));
			}
			else
			{
				assert(false);
			}

			totalRate += rate;
			++iter;
		}

		StatisticsReport::summary summary;
		summary.push_back(stripClubName((*sIter)[0]));
		summary.push_back(stripCoachName((*sIter)[1]));
		std::ostringstream oss;
		oss << static_cast<float>(totalRate) / static_cast<float>(
			(pIter->size() - 2));
		report.mData.insert(std::make_pair(oss.str(), summary));

		++sIter;
		++pIter;
	}

	return true;
}
