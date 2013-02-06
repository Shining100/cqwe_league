#include "statistics/LeagueParser.hxx"
#include <iostream>
#include "statistics/LeagueParserImpl.hxx"

LeagueParser::LeagueParser()
	: mHtmlDoc(NULL)
{

}

LeagueParser::~LeagueParser()
{
	if (NULL != mHtmlDoc)
	{
		xmlFreeDoc(mHtmlDoc);
	}
}

bool LeagueParser::parse(const std::string &fileName, bool debug)
{
	mHtmlDoc = htmlReadFile(fileName.c_str(), "gb2312",
		XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
	if (NULL == mHtmlDoc)
	{
		return false;
	}

	mImpl.reset(new LeagueParserImpl(mSummaries, mPlayers, debug));
	htmlIterator((xmlNodePtr)mHtmlDoc);
	mSummaries.pop_back(); // É¾³ý½áÎ²µÄ·µ»Ø

	return true;
}

void LeagueParser::htmlIterator(xmlNodePtr node)
{
	for (xmlNodePtr cur = node; NULL != cur; cur = cur->next)
	{
		mImpl->parse(cur);
		htmlIterator(cur->children);
	}
}
