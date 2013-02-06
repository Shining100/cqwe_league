#include "stdafx.h"
#include "statistics_league_gui/LeagueDownloader.hxx"
#include <sstream>
#include <curl/curl.h>

LeagueDownloader::LeagueDownloader()
{
	CURLcode code = curl_global_init(CURL_GLOBAL_WIN32);
	if (CURLE_OK != code)
	{
		throw std::exception();
	}
}

LeagueDownloader::~LeagueDownloader()
{
	curl_global_cleanup();
}

size_t writeFunc(char *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *f = (FILE*)userdata;
	fwrite(ptr, size, nmemb, f);
	return size * nmemb;
}

bool LeagueDownloader::download(const std::string &filename) const
{
	const std::string urlPrefix("http://www.cqwe.com/league/");
	std::ostringstream os;
	os << urlPrefix << filename;

	CURL *url = NULL;
	url = curl_easy_init();
	if (NULL == url)
	{
		return false;
	}

	CURLcode code = curl_easy_setopt(url, CURLOPT_URL, os.str().c_str());
	if (CURLE_OK != code)
	{
		goto error_handle;
	}
	code = curl_easy_setopt(url, CURLOPT_WRITEFUNCTION, writeFunc);
	if (CURLE_OK != code)
	{
		goto error_handle;
	}
	FILE *f = fopen(filename.c_str(), "w");
	if (NULL == f)
	{
		goto error_handle;
	}
	code = curl_easy_setopt(url, CURLOPT_WRITEDATA, f);
	if (CURLE_OK != code)
	{
		goto error_handle2;
	}
	code = curl_easy_perform(url);
	if (CURLE_OK != code)
	{
		goto error_handle2;
	}

	fclose(f);
	f = NULL;
	curl_easy_cleanup(url);
	url = NULL;

	return true;

error_handle2:
	fclose(f);
	f = NULL;
error_handle:
	curl_easy_cleanup(url);
	url = NULL;

	return false;
}
