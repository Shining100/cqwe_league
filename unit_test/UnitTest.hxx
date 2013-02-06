#ifndef _HTTP_TEST_HXX_
#define _HTTP_TEST_HXX_

#include <memory>
#include <cppunit/extensions/HelperMacros.h>

class UnitTest : public CPPUNIT_NS::TestFixture 
{
	CPPUNIT_TEST_SUITE(UnitTest);

//	CPPUNIT_TEST(testConnCqwe);
//	CPPUNIT_TEST(testParseClubSummary);
//	CPPUNIT_TEST(testParseClubPlayer);
//	CPPUNIT_TEST(testParseLeague);
	CPPUNIT_TEST(testIconv);
// 	CPPUNIT_TEST(testStatisticsSalary);
// 	CPPUNIT_TEST(testStatisticsAge);
// 	CPPUNIT_TEST(testStatisticsHeight);
// 	CPPUNIT_TEST(testStatisticsRate);
// 	CPPUNIT_TEST(testStatisticsNation);
// 	CPPUNIT_TEST(testStatisticsClub);
	CPPUNIT_TEST(completed);

	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp();
	virtual void tearDown();

public:
	void testConnCqwe();
	void testParseClubSummary();
	void testParseClubPlayer();
	void testParseLeague();
	void testIconv();
	void testStatisticsSalary();
	void testStatisticsAge();
	void testStatisticsHeight();
	void testStatisticsRate();
	void testStatisticsNation();
	void testStatisticsClub();

	void completed();

private:
	void statisticsDigit(const std::string &file, const std::string &type);
	void statisticsData(const std::string &file, const std::string &type);

private:
	static bool mIsComplete;
};

#endif
