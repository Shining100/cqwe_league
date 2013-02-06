#ifndef _LEAGUE_PARSER_HXX_
#define _LEAGUE_PARSER_HXX_

#include <vector>
#include <list>
#include <string>
#include <memory>
#include <libxml/HTMLparser.h>

class LeagueParserImpl;

class LeagueParser
{
public:
	typedef std::vector<std::string> meta_datas;
	typedef std::list<meta_datas> club_summaries;
	typedef std::list<meta_datas> players;
	typedef std::list<players> club_players;

public:
	explicit LeagueParser();
	~LeagueParser();

private:
	LeagueParser(const LeagueParser&);
	LeagueParser& operator=(const LeagueParser&);

public:
	bool parse(const std::string &fileName, bool debug);
	const club_summaries& getSummaries() const {return mSummaries;}
	const club_players& getPlayers() const {return mPlayers;}

private:
	void htmlIterator(xmlNodePtr node);

private:
	htmlDocPtr mHtmlDoc;
	club_summaries mSummaries;
	club_players mPlayers;
	std::auto_ptr<LeagueParserImpl> mImpl;
};

#endif
