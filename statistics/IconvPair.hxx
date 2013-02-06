#ifndef _ICONV_PAIR_HXX_
#define _ICONV_PAIR_HXX_

#include <string>
#include <iconv/include/iconv.h>

class IconvPair
{
	enum 
	{
		INIT_BUFFER = 4096
	};

public:
	IconvPair(const std::string &toCode, const std::string &fromCode);
	~IconvPair();

private:
	IconvPair(const IconvPair&);
	IconvPair& operator=(const IconvPair&);

public:
	friend bool operator<(const IconvPair &lhs, const IconvPair &rhs)
	{
		if (lhs.mToCode < rhs.mToCode)
		{
			return true;
		}
		else if (lhs.mToCode > rhs.mToCode)
		{
			return false;
		}

		return lhs.mFromCode <=rhs.mFromCode ? true : false;
	}

	const char* buffer() const {return mBuffer;}
	size_t bufferLen() const {return mBufferLen;}
	size_t contentLen() const {return mContentLen;}

	size_t convert(const char **inBuffer, size_t *inBytesLeft);

private:
	void incBuffer();

private:
	std::string mToCode;
	std::string mFromCode;
	iconv_t mIconv;
	char *mBuffer;
	size_t mBufferLen;
	size_t mContentLen;
};

#endif
