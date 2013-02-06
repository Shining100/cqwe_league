#ifndef _LEAGUE_PARSER_IMPL_HXX_
#define _LEAGUE_PARSER_IMPL_HXX_

#include <cstdlib>
#include <vector>
#include <list>
#include <string>
#include <libxml/HTMLparser.h>
#include "statistics/IconvPair.hxx"

class LeagueParserImpl
{
	typedef std::vector<std::string> meta_datas;
	typedef std::list<meta_datas> club_summaries;
	typedef std::list<meta_datas> players;
	typedef std::list<players> club_players;

	enum State
	{
		Initial,
		Summery,
		Players,
	};

	enum SummaryState
	{
		ClubName,
		CoachName
	};

	LeagueParserImpl(club_summaries &summeries, club_players &players,
		bool debug);
	LeagueParserImpl(const LeagueParserImpl&);
	LeagueParserImpl& operator=(const LeagueParserImpl);

public:
	~LeagueParserImpl();

private:
	void parse(xmlNodePtr node);
	void parseInitial(xmlNodePtr node);
	void parseSummary(xmlNodePtr node);
	void parsePlayers(xmlNodePtr node);
	xmlChar* skipPrefixWhiteSpace(xmlNodePtr node);

	club_summaries &mSummaries;
	club_players &mPlayers;
	bool mDebug;
	FILE *mFile;
	State mState;
	SummaryState mSummaryState;
	bool mIsBeginTable;
	IconvPair mg2u;
	IconvPair mu2g;
	char mClubTag[16];
	char mCoachTag[16];
	friend class LeagueParser;
};

#endif
