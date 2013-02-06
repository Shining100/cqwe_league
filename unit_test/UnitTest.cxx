#include "unit_test/UnitTest.hxx"
#include <cstdio>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <cppunit/config/SourcePrefix.h>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include "statistics/StatisticsManager.hxx"
#include "statistics/StatisticsReport.hxx"
#include "statistics/LeagueParser.hxx"
#include "statistics/IconvPair.hxx"

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest);

bool UnitTest::mIsComplete = false;

void UnitTest::setUp()
{
	static bool mIsInit = false;

	if (!mIsInit)
	{
		CURLcode code = curl_global_init(CURL_GLOBAL_WIN32);
		if (CURLE_OK != code)
		{
			std::cerr << "curl_global_init failed: "
					  << curl_easy_strerror(code);
		}
	}
}

void UnitTest::tearDown()
{
	if (mIsComplete)
	{
		curl_global_cleanup();
	}
}

size_t writeFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *f = (FILE*)userdata;
	fwrite(ptr, size, nmemb, f);
	return size * nmemb;
}

void UnitTest::testConnCqwe()
{
	CURL *url = NULL;
	url = curl_easy_init();
	CPPUNIT_ASSERT(NULL != url);

	CURLcode code = curl_easy_setopt(url, CURLOPT_URL,
		"http://www.cqwe.com/league/league2012b.htm");
	if (CURLE_OK != code)
	{
		std::cerr << "curl_easy_setopt failed: "
				  << curl_easy_strerror(code);
		goto error_handle;
	}
	code = curl_easy_setopt(url, CURLOPT_WRITEFUNCTION, writeFunc);
	if (CURLE_OK != code)
	{
		std::cerr << "curl_easy_setopt failed: "
				  << curl_easy_strerror(code);
		goto error_handle;
	}
	FILE *f = fopen("league2012b.htm", "w");
	if (NULL == f)
	{
		std::cerr << "fopen failed";
		goto error_handle;
	}
	code = curl_easy_setopt(url, CURLOPT_WRITEDATA, f);
	if (CURLE_OK != code)
	{
		std::cerr << "curl_easy_setopt failed: "
				  << curl_easy_strerror(code);
		goto error_handle2;
	}
	code = curl_easy_perform(url);
	if (CURLE_OK != code)
	{
		std::cerr << "curl_easy_perform failed: "
				  << curl_easy_strerror(code);
		goto error_handle2;
	}

	fclose(f);
	f = NULL;
	curl_easy_cleanup(url);
	url = NULL;
	return;

error_handle2:
	fclose(f);
	f = NULL;
error_handle:
	curl_easy_cleanup(url);
	url = NULL;
	CPPUNIT_ASSERT(false);
}

char* ConvertEnc( char *encFrom, char *encTo, const char * in)
{
	static char bufin[1024], bufout[1024], *sin, *sout;
	int lenin, lenout, ret;
	iconv_t c_pt;

	if ((c_pt = iconv_open(encTo, encFrom)) == (iconv_t)-1)
	{
		printf("iconv_open false: %s ==> %s", encFrom, encTo);
		return NULL;
	}
	iconv(c_pt, NULL, NULL, NULL, NULL);

	lenin  = strlen(in) + 1;
	lenout = 1024;
	sin    = (char *)in;
	sout   = bufout;
	ret = iconv(c_pt, (const char**)&sin, (size_t *)&lenin, &sout, (size_t *)&lenout);

	if (ret == -1)
	{
		return NULL;
	}
	iconv_close(c_pt);

	return bufout;
}

class HtmlIterator
{
public:
	virtual ~HtmlIterator(){}

public:
	bool parse(const char* fileName)
	{
		htmlDocPtr htmlDoc = htmlReadFile(fileName, "gb2312",
			XML_PARSE_NOERROR | XML_PARSE_NOWARNING);
		if (NULL == htmlDoc)
		{
			xmlErrorPtr error =  xmlGetLastError();
			printf("%s\r\n", error->message);
			return false;
		}

		iterator((xmlNode*)htmlDoc);
		xmlFreeDoc(htmlDoc);
		return true;
	}

protected:
	virtual void onNode(xmlNode *node) = 0;

private:
	void iterator(xmlNode *node)
	{
		for (xmlNode *cur = node; NULL != cur; cur = cur->next)
		{
			onNode(cur);
			iterator(cur->children);
		}
	}
};

void UnitTest::testParseClubSummary()
{
	class SummaryParser : public HtmlIterator
	{
	public:
		SummaryParser()
			: mIsParseClubName(true)
			, mIsParseClubCoach(true)
		{

		}

		virtual void onNode(xmlNode *node)
		{
			if (XML_TEXT_NODE == node->type)
			{
				if (0 == xmlStrcmp(BAD_CAST"\r", node->content))
				{
					return;
				}

				if (mIsParseClubName)
				{
					printf("%s\r\n", ConvertEnc("utf-8", "gb2312",
						(char*)node->content));
					mIsParseClubName = false;
					return;
				}
				else if (mIsParseClubCoach)
				{
					const char *coachTag = "教练";
					const char *coachName = ConvertEnc("utf-8",
						"gb2312", (char*)node->content);
					if (NULL ==strstr(coachName, coachTag))
					{
						return;
					}

					printf("%s\r\n", coachName);
					mIsParseClubCoach = false;
					return;
				}
			}
		}

	private:
		bool mIsParseClubName;
		bool mIsParseClubCoach;
	};

	SummaryParser parser;
	CPPUNIT_ASSERT(parser.parse("summary.html"));
}

void UnitTest::testParseClubPlayer()
{
	class PlayersParser : public HtmlIterator
	{
	public:
		PlayersParser()
			: mFile(fopen("players.txt", "w"))
			, mTableHeader(0)
		{
			if (NULL == mFile)
			{
				throw std::exception();
			}
		}

		~PlayersParser()
		{
			if (NULL != mFile)
			{
				fclose(mFile);
			}
		}

	protected:
		virtual void onNode(xmlNode *node)
		{
			if (XML_ELEMENT_NODE == node->type && !isTableContent())
			{
				const xmlChar *tr = (xmlChar*)"tr";
				if (0 == xmlStrcmp(tr, node->name))
				{
					++mTableHeader;
					return;
				}
			}
			else if (XML_TEXT_NODE == node->type && isTableContent())
			{
				fprintf(mFile, "%s", ConvertEnc("utf-8", "gb2312",
					(char*)node->content));
			}
		}

	private:
		bool isTableContent() {return 3 == mTableHeader;}

	private:
		FILE *mFile;
		unsigned mTableHeader;
	};

	PlayersParser parser;
	CPPUNIT_ASSERT(parser.parse("players.html"));
}

void UnitTest::testParseLeague()
{
	/*class LeagueParser : public HtmlIterator
	{
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

		enum PlayerState
		{
			Header,
			Data,
		};

		struct PlayerCtx
		{
			PlayerCtx()
				: mHeaderCounter(0)
				, mState(Header)
			{

			}

			unsigned mHeaderCounter;
			PlayerState mState;
		};

	public:
		LeagueParser()
			: mFile(fopen("league.txt", "w"))
			, mState(Initial)
			, mSummaryState(ClubName)
		{
			if (NULL == mFile)
			{
				throw std::exception();
			}

			strcpy(mClubTag, ConvertEnc("gb2312", "utf-8", "俱乐部"));
			strcpy(mCoachTag, ConvertEnc("gb2312", "utf-8", "教练"));
		}

		~LeagueParser()
		{
			if (NULL != mFile)
			{
				fclose(mFile);
			}
		}

	protected:
		virtual void onNode(xmlNode *node)
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

	private:
		void parseInitial(xmlNode *node)
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

		void parseSummary(xmlNode *node)
		{
			if (XML_TEXT_NODE != node->type)
			{
				return;
			}

			switch (mSummaryState)
			{
			case ClubName:
				printf("%s\r\n", ConvertEnc("utf-8", "gb2312",
					(char*)skipPrefixWhiteSpace(node)));
				fprintf(mFile, "%s\r\n", ConvertEnc("utf-8", "gb2312",
					(char*)skipPrefixWhiteSpace(node)));
				mSummaryState = CoachName;
				
				break;
			case CoachName:
				if (NULL == xmlStrstr(node->content, BAD_CAST(mCoachTag)))
				{
					return;
				}

				printf("%s\r\n", ConvertEnc("utf-8", "gb2312",
					(char*)skipPrefixWhiteSpace(node)));
				fprintf(mFile, "%s\r\n", ConvertEnc("utf-8", "gb2312",
					(char*)skipPrefixWhiteSpace(node)));
				mSummaryState = ClubName;
				mState = Players;
				
				break;
			default:
				break;
			}
		}

		void parsePlayers(xmlNode *node)
		{
			switch (mPlayerCtx.mState)
			{
			case Header:
				if (XML_ELEMENT_NODE == node->type)
				{
					if (0 == xmlStrcmp(BAD_CAST"tr", node->name))
					{
						if (3 == ++mPlayerCtx.mHeaderCounter)
						{
							mPlayerCtx.mHeaderCounter = 0;
							mPlayerCtx.mState = Data;
						}

						return;
					}
				}
				break;
			case Data:
				if (XML_ELEMENT_NODE == node->type)
				{
					if (0 == xmlStrcmp(BAD_CAST"p", node->name))
					{
						// 忽略tr中出现的p标签。。。
						xmlAttrPtr attr = node->properties;
						xmlNodePtr attrNode = (NULL != attr) ?
							attr->children : NULL;
						if (NULL != attrNode && 0 == xmlStrcmp(
							BAD_CAST"right", attrNode->content))
						{
							return;
						}

						fprintf(mFile, "\r\n");
						mPlayerCtx.mState = Header;
						mState = Summery;
					}
					else if (0 == xmlStrcmp(BAD_CAST"tr", node->name))
					{
						fprintf(mFile, "\r\n");
					}
				}
				else if (XML_TEXT_NODE == node->type)
				{
					fprintf(mFile, "%s ", ConvertEnc("utf-8", "gb2312",
						(char*)skipPrefixWhiteSpace(node)));
				}
				break;
			default:
				break;
			}
		}

		xmlChar* skipPrefixWhiteSpace(xmlNode *node)
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

	private:
		FILE *mFile;
		State mState;
		SummaryState mSummaryState;
		PlayerCtx mPlayerCtx;
		char mClubTag[16];
		char mCoachTag[16];
	};*/

	LeagueParser parser;
	CPPUNIT_ASSERT(parser.parse("league2012b.htm",  true));
}

void UnitTest::testIconv()
{
	const size_t pageSize = 4096;
	IconvPair g2u("utf-8", "gb2312");

	char array[pageSize] = {'a'};
	const char *inArray = array;
	size_t inArrayLen = pageSize;
	size_t ret = g2u.convert(&inArray, &inArrayLen);
	CPPUNIT_ASSERT((0 == ret) && ((pageSize * 2) == g2u.bufferLen()));

	char *buffer = reinterpret_cast<char*>(malloc(1024 * 1024 * sizeof(char)));
	CPPUNIT_ASSERT(NULL != buffer);

	FILE *f = fopen("league2012b.htm", "r");
	CPPUNIT_ASSERT(NULL != f);
	size_t fileLen = 0;
	while (!feof(f))
	{
		fileLen += fread(buffer + fileLen, 1, pageSize, f);
	}
	fclose(f);

	const char *bufferIn = buffer;
	ret = g2u.convert(&bufferIn, &fileLen);
	CPPUNIT_ASSERT(0 == ret);

	f = fopen("league2012b.txt", "w");
	CPPUNIT_ASSERT(NULL != f);
	fwrite(g2u.buffer(), 1, g2u.contentLen(), f);
	fclose(f);

	free(buffer);
}

void UnitTest::testStatisticsSalary()
{
	statisticsDigit("league2012b.htm", "工资");
}

void UnitTest::testStatisticsAge()
{
	statisticsDigit("league2012b.htm", "生年");
}

void UnitTest::testStatisticsHeight()
{
	statisticsDigit("league2012b.htm", "身高");
}

void UnitTest::testStatisticsRate()
{
	statisticsDigit("league2012b.htm", "总评");
}

void UnitTest::testStatisticsNation()
{
	statisticsData("league2012b.htm", "国籍");
}

void UnitTest::testStatisticsClub()
{
	statisticsData("league2012b.htm", "俱乐部");
}

void UnitTest::statisticsDigit(const std::string &file,
	const std::string &type)
{
	StatisticsManager manager(file);
	StatisticsReport report;
	CPPUNIT_ASSERT(manager.statistics(type, report));
	CPPUNIT_ASSERT(!report.mData.empty());

	std::ostringstream oss;
	oss << type << ".txt";
	FILE *f = fopen(oss.str().c_str(), "w");
	if (NULL == f)
	{
		CPPUNIT_ASSERT(false);
		return;
	}

	typedef StatisticsReport::report_map report_map;
	for (report_map::iterator iter = report.mData.begin();
		iter != report.mData.end(); ++iter)
	{
		fprintf(f, "%-30s%-20s%-10s\r\n", iter->second[0].c_str(),
			iter->second[1].c_str(), iter->first.c_str());
	}

	fclose(f);
}

void UnitTest::statisticsData(const std::string &file, const std::string &type)
{
	StatisticsManager manager(file);
	StatisticsReport report;
	CPPUNIT_ASSERT(manager.statistics(type, report));
	CPPUNIT_ASSERT(!report.mData.empty());

	std::ostringstream oss;
	oss << type << ".txt";
	FILE *f = fopen(oss.str().c_str(), "w");
	if (NULL == f)
	{
		CPPUNIT_ASSERT(false);
		return;
	}

	typedef StatisticsReport::report_map report_map;
	for (report_map::iterator iter = report.mData.begin();
		iter != report.mData.end(); ++iter)
	{
		fprintf(f, "%-20s%-10s\r\n", iter->second[0].c_str(),
			iter->first.c_str());
	}

	fclose(f);
}

void UnitTest::completed()
{
	mIsComplete = true;
}
