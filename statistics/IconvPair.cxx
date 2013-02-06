#include "statistics/IconvPair.hxx"
#include <cstdlib>
#include <cerrno>
#include <cassert>
#include <climits>
#include <exception>

IconvPair::IconvPair(const std::string &toCode, const std::string &fromCode)
	: mToCode(toCode)
	, mFromCode(fromCode)
	, mIconv(reinterpret_cast<iconv_t>(-1))
	, mBuffer(NULL)
	, mBufferLen(0)
	, mContentLen(0)
{
	if (mToCode.empty() || mFromCode.empty())
	{
		throw std::exception();
	}

	mBufferLen = INIT_BUFFER;
	mBuffer = reinterpret_cast<char*>(malloc(mBufferLen));
	if (NULL == mBuffer)
	{
		throw std::bad_alloc();
	}

	mIconv = iconv_open(toCode.c_str(), fromCode.c_str());
	if (reinterpret_cast<iconv_t>(-1) == mIconv)
	{
		throw std::exception();
	}
}

IconvPair::~IconvPair()
{
	if (reinterpret_cast<iconv_t>(-1) != mIconv)
	{
		iconv_close(mIconv);
	}

	free(mBuffer);
}

size_t IconvPair::convert(const char **inBuffer, size_t *inBytesLeft)
{
	assert((NULL != mBuffer) && (reinterpret_cast<iconv_t>(-1) != mIconv));
	assert((NULL != inBuffer) && (NULL != *inBuffer) && (NULL != inBytesLeft));

	iconv(mIconv, NULL, NULL, NULL, NULL);

	char *outBuffer = mBuffer;
	size_t outBytesLeft = mBufferLen;
	size_t ret = iconv(mIconv, inBuffer, inBytesLeft, &outBuffer,
		&outBytesLeft);
	while ((UINT_MAX == ret) && (0 == outBytesLeft)/*(E2BIG == errno)*/)
	{
		size_t oldBufferLen = mBufferLen;
		incBuffer();

		outBuffer = mBuffer + oldBufferLen;
		outBytesLeft = mBufferLen - oldBufferLen;
		ret = iconv(mIconv, inBuffer, inBytesLeft, &outBuffer,
			&outBytesLeft);
	}

	mContentLen = mBufferLen - outBytesLeft;
	if (0 == outBytesLeft)
	{
		incBuffer();
	}
	mBuffer[mContentLen] = 0;
	return ret;
}

void IconvPair::incBuffer()
{
	mBufferLen *= 2;
	mBuffer = reinterpret_cast<char*>(realloc(mBuffer, mBufferLen));
	if (NULL == mBuffer)
	{
		throw std::bad_alloc();
	}
}
