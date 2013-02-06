#include "statistics/Strip.hxx"

std::string strip_gb2312(const std::string &src, const std::string &tag)
{
	unsigned pos = src.find(tag);
	if (pos == std::string::npos)
	{
		return src;
	}

	return src.substr(pos + tag.size());
}

std::string stripClubName(const std::string &clubName)
{
	return strip_gb2312(clubName, "¡¢");
}

std::string stripCoachName(const std::string &coachName)
{
	return strip_gb2312(coachName, "£º");
}
