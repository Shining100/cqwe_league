#include "statistics/LeagueParserImpl.hxx"
#include <cassert>
#include <algorithm>

LeagueParserImpl::LeagueParserImpl(club_summaries &summeries,
	club_players &players, bool debug)
	: mSummaries(summeries)
	, mPlayers(players)
	, mDebug(debug)
	, mFile(mDebug ? fopen("debug_html.txt", "w") : NULL)
	, mState(Initial)
	, mSummaryState(ClubName)
	, mIsBeginTable(false)
	, mg2u("utf-8", "gb2312")
	, mu2g("gb2312", "utf-8")
{
	if (mDebug && NULL == mFile)
	{
		throw std::exception();
	}

	const char *clubTag = "俱乐部";
	const char *clubTagIn = clubTag;
	size_t clubTagLen = strlen(clubTag);
	mg2u.convert(&clubTagIn, &clubTagLen);
	strcpy(mClubTag, mg2u.buffer());

	const char *coachTag = "教练";
	const char *coachTagIn = coachTag;
	size_t coachTagLen = strlen(coachTag);
	mg2u.convert(&coachTagIn, &coachTagLen);
	strcpy(mCoachTag, mg2u.buffer());
}

LeagueParserImpl::~LeagueParserImpl()
{
	if (NULL != mFile)
	{
		class print_club_summary
		{
		public:
			explicit print_club_summary(FILE *file)
				: mFile(file)
			{

			}

			void operator()(const std::string &prop)
			{
				fprintf(mFile, "%s ", prop.c_str());
			}

		private:
			FILE *mFile;
		};

		class print_player_meta_data
		{
		public:
			explicit print_player_meta_data(FILE *file)
				: mFile(file)
			{

			}

			void operator()(const std::string &prop)
			{
				fprintf(mFile, "%s ", prop.c_str());
			}

		private:
			FILE *mFile;
		};

		class print_player
		{
		public:
			explicit print_player(FILE *file)
				: mFile(file)
			{

			}

			void operator()(const meta_datas &md)
			{
				std::for_each(md.begin(), md.end(),
					print_player_meta_data(mFile));
				fprintf(mFile, "\r\n");
			}

		private:
			FILE *mFile;
		};

		class print_club_info
		{
		public:
			print_club_info(club_players::iterator playerIter, FILE *file)
				: mPlayerIter(playerIter)
				, mFile(file)
			{

			}

			void operator()(const meta_datas &md)
			{
				std::for_each(md.begin(), md.end(), print_club_summary(mFile));
				fprintf(mFile, "\r\n\r\n");
				std::for_each(mPlayerIter->begin(), mPlayerIter->end(),
					print_player(mFile));
				++mPlayerIter;
				fprintf(mFile, "\r\n\r\n\r\n");
			}

		private:
			club_players::iterator mPlayerIter;
			FILE *mFile;
		};

		fprintf(mFile, "================================================\r\n");
		std::for_each(mSummaries.begin(), mSummaries.end(), print_club_info(
			mPlayers.begin(), mFile));

		fclose(mFile);
	}
}

void LeagueParserImpl::parse(xmlNodePtr node)
{
	if (XML_TEXT_NODE == node->type &&
		0 == *skipPrefixWhiteSpace(node))
	{
		return;
	}

	switch (mState)
	{
	case Initial:
		parseInitial(node);
		break;
	case Summery:
		parseSummary(node);
		break;
	case Players:
		parsePlayers(node);
		break;
	default:
		break;
	}
}

void LeagueParserImpl::parseInitial(xmlNodePtr node)
{
	if (XML_TEXT_NODE == node->type)
	{
		if (NULL != xmlStrstr(node->content, BAD_CAST(mClubTag)))
		{
			mState = Summery;
			return;
		}
	}
}

void LeagueParserImpl::parseSummary(xmlNodePtr node)
{
	if (XML_TEXT_NODE != node->type)
	{
		return;
	}

	const char *rawContent = (const char*)skipPrefixWhiteSpace(node);
	size_t contentLen = strlen(rawContent);
	mu2g.convert(&rawContent, &contentLen);
	const char *content = mu2g.buffer();
	switch (mSummaryState)
	{
	case ClubName:
		if (mDebug)
		{
			fprintf(mFile, "%s\r\n", content);
		}
		mSummaries.push_back(meta_datas());
		mSummaries.rbegin()->push_back(content);

		mSummaryState = CoachName;
		break;
	case CoachName:
		if (NULL == xmlStrstr(node->content, BAD_CAST(mCoachTag)))
		{
			return;
		}

		if (mDebug)
		{
			fprintf(mFile, "%s\r\n", content);
		}
		mSummaries.rbegin()->push_back(content);
		mPlayers.push_back(players());

		mSummaryState = ClubName;
		mState = Players;
		break;
	default:
		break;
	}
}

void LeagueParserImpl::parsePlayers(xmlNodePtr node)
{
	if (!mIsBeginTable)
	{
		if (0 == xmlStrcmp(BAD_CAST"table", node->name))
		{
			mIsBeginTable = true;
		}
		return;
	}

	if (XML_ELEMENT_NODE == node->type)
	{
		if (0 == xmlStrcmp(BAD_CAST"tr", node->name))
		{
			if (mDebug)
			{
				fprintf(mFile, "\r\n");
			}
			mPlayers.rbegin()->push_back(meta_datas());
		}
		else if (0 == xmlStrcmp(BAD_CAST"p", node->name) &&
				 (NULL == node->properties /*忽略tr中出现的p标签*/||
				 0 == xmlStrcmp(BAD_CAST"a", node->children->name)/*忽略结尾的返回*/))
		{
			if (mDebug)
			{
				fprintf(mFile, "\r\n");
			}
			mIsBeginTable = false;
			mState = Summery;
		}
	}
	else if (XML_TEXT_NODE == node->type)
	{
		const char *rawContent = (const char*)skipPrefixWhiteSpace(node);
		size_t contentLen = strlen(rawContent);
		mu2g.convert(&rawContent, &contentLen);
		const char *content = mu2g.buffer();
		if (mDebug)
		{
			fprintf(mFile, "%s ", content);
		}
		mPlayers.rbegin()->rbegin()->push_back(content);
	}
}

xmlChar* LeagueParserImpl::skipPrefixWhiteSpace(xmlNodePtr node)
{
	assert(NULL != node->content);
	xmlChar *p = node->content;
	while (xmlChar('\r') == *p || xmlChar('\n') == *p ||
		   xmlChar(' ') == *p || 0xe3 == *p || 0x80 == *p)
	{
		++p;
	}

	return p;
}
