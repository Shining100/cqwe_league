#ifndef _LEAGUE_DOWNLOADER_HXX_
#define _LEAGUE_DOWNLOADER_HXX_

#include <string>

class LeagueDownloader
{
private:
	LeagueDownloader();
	~LeagueDownloader();
	LeagueDownloader(const LeagueDownloader&);
	LeagueDownloader& operator=(const LeagueDownloader&);
	template <class T> friend class Singleton;

public:
	bool download(const std::string &filename) const;
};

#endif
