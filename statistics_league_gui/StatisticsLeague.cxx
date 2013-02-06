#include "stdafx.h"
#include "statistics_league_gui/StatisticsLeague.hxx"
#include <io.h>
#include <sstream>
#include "statistics/StatisticsManager.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/Singleton.hxx"
#include "statistics_league_gui/LeagueDownloader.hxx"

bool StatisticsLeague::statistics(const std::string &season,
	const std::string &type)
{
	std::shared_ptr<StatisticsManager> manager;
	manager_map::iterator iter = mManagers.find(season);
	if (mManagers.end() == iter)
	{
		manager = createManager(season);
		if (NULL == manager)
		{
			return false;
		}
	}
	else
	{
		manager = iter->second;
	}

	StatisticsReport report;
	if (!manager->statistics(type, report))
	{
//		AfxMessageBox("统计失败");
		return false;
	}

	return output(report, season, type);
}

std::shared_ptr<StatisticsManager> StatisticsLeague::createManager(
	const std::string &season)
{
	const std::string filePrefix("league");
	const std::string suffix(".htm");
	std::ostringstream os;
	os << filePrefix << season << suffix;

	if (-1 == _access(os.str().c_str(), 0))
	{
		try
		{
			if (!Singleton<LeagueDownloader>::instance().download(os.str()))
			{
//				AfxMessageBox("下载联赛数据失败, 请检查网络连接");
				return NULL;
			}
		}
		catch(std::exception&)
		{
//			AfxMessageBox("初始化HTTP下载失败");
			return NULL;
		}
	}

	try
	{
		std::shared_ptr<StatisticsManager> manager(new StatisticsManager(
			os.str()));
		mManagers.insert(std::make_pair(season, manager));
		return manager;
	}
	catch(std::exception)
	{
//		AfxMessageBox("解析联赛数据失败，请重新下载数据进行尝试");
		return NULL;
	}
}

bool StatisticsLeague::output(const StatisticsReport &report,
	const std::string &season, const std::string &type) const
{
	std::ostringstream os;
	os << season << type << ".txt";
	FILE *f = fopen(os.str().c_str(), "w");
	if (NULL == f)
	{
//		AfxMessageBox("打开统计文件失败");
		return false;
	}

	typedef StatisticsReport::report_map report_map;
	if ("国籍" == type || "俱乐部" == type)
	{
		for (report_map::const_iterator iter = report.mData.begin();
			iter != report.mData.end(); ++iter)
		{
			fprintf(f, "%-20s%-10s\r\n", iter->second[0].c_str(),
				iter->first.c_str());
		}	
	}
	else
	{
		for (report_map::const_iterator iter = report.mData.begin();
			iter != report.mData.end(); ++iter)
		{
			fprintf(f, "%-30s%-20s%-10s\r\n", iter->second[0].c_str(),
				iter->second[1].c_str(), iter->first.c_str());
		}
	}

	fclose(f);

	return true;
}
