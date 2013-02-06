#ifndef _STATISTICS_REPORT_HXX_
#define _STATISTICS_REPORT_HXX_

#include <map>
#include <vector>
#include <string>
#include <functional>

class StatisticsReport
{
	struct digit_string_greater : public std::binary_function<
		std::string, std::string, bool>
	{
		bool operator()(const std::string &lhs, const std::string &rhs) const
		{
			return atof(lhs.c_str()) > atof(rhs.c_str());
		}
	};

public:
	typedef std::vector<std::string> summary;
	typedef std::multimap<std::string, summary, digit_string_greater> report_map;
	report_map mData;
};

#endif
