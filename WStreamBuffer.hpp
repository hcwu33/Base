#pragma once
#include "WRecursiveLock.hpp"
#include <stdlib.h>
#include <string.h>


class CWStreamBuffer
{
public:
	CWStreamBuffer()
	{
		this->mBuffer = (char *)malloc(g_nInitSize + g_nPrepend);
		this->mBufferSize = g_nInitSize + g_nPrepend;
		this->mIndexRead = g_nPrepend;
		this->mIndexWrite = g_nPrepend;
	}
	~CWStreamBuffer()
	{
		free(this->mBuffer);
		this->mBuffer = NULL;
	}

public:


	size_t ReadableBytes()
	{
		return this->mIndexWrite - this->mIndexRead;
	}

	size_t WriteableBytes()
	{
		return this->mBufferSize - this->mIndexWrite;
	}

	size_t BufferSize()
	{
		return this->mBufferSize;
	}

	size_t PrependableBytes()
	{
		return this->mIndexRead;
	}

	const char* BeginRead() const
	{
		return this->mBuffer + this->mIndexRead;
	}

	const char* Prepend() const
	{
		return this->mBuffer;
	}

	int MoveBackReadIndex(size_t len)
	{
		assert(len <= this->ReadableBytes());
		if (len > this->ReadableBytes())
		{
			return 0;
		}

		this->mIndexRead += len;

		return 1;
	}

	int MoveBackWriteIndex(size_t len)
	{
		assert(len <= this->WriteableBytes());
		if (len > this->WriteableBytes())
		{
			return 0;
		}

		this->mIndexWrite += len;

		return 1;
	}

	void RetrieveAll()
	{
		this->mIndexRead = g_nPrepend;
		this->mIndexWrite = g_nPrepend;
	}

	int Append(const char* data, size_t len)
	{
		this->EnsureWriteable(len);

		memcpy(this->mBuffer + this->mIndexWrite, data, len);

		this->mIndexWrite += len;

		return 1;
	}

	int Read(char** data, size_t len)
	{
		if (len > this->ReadableBytes())
		{
			len = this->ReadableBytes();
		}

		memcpy(*data, this->mBuffer + this->mIndexRead, len);
		this->mIndexRead += len;

		return len;
	}

	int Shrink(size_t sizeReserve)
	{
		char* buff;
		this->mBufferSize = g_nPrepend + this->ReadableBytes() + sizeReserve;
		buff = (char *)malloc(this->mBufferSize);

		memcpy(buff + g_nPrepend, this->BeginRead(), this->ReadableBytes());

		int nReadableBytes = this->ReadableBytes();

		free(this->mBuffer);
		this->mBuffer = buff;

		this->mIndexRead = g_nPrepend;
		this->mIndexWrite = this->mIndexRead + nReadableBytes;

		return 1;
	}

	void Swap(CWStreamBuffer& buff)
	{
		char* buffTemp = this->mBuffer;
		this->mBuffer = buff.mBuffer;
		buff.mBuffer = buffTemp;

		uint32_t valTemp = this->mBufferSize;
		this->mBufferSize = buff.mBufferSize;
		buff.mBufferSize = valTemp;

		valTemp = this->mIndexRead;
		this->mIndexRead = buff.mIndexRead;
		buff.mIndexRead = valTemp;

		valTemp = this->mIndexWrite;
		this->mIndexWrite = buff.mIndexWrite;
		buff.mIndexWrite = valTemp;
	}

	void EnsureWriteable(size_t sizeWrite)
	{
		if (sizeWrite <= this->WriteableBytes())
			return;

		if (this->PrependableBytes() + this->WriteableBytes() - g_nPrepend >= sizeWrite)
		{
			int nReadableBytes = this->ReadableBytes();
			memmove(this->mBuffer + g_nPrepend, this->BeginRead(), nReadableBytes);
			this->mIndexRead = g_nPrepend;
			this->mIndexWrite = this->mIndexRead + nReadableBytes;
		}
		else
		{
			this->mBufferSize += sizeWrite - this->WriteableBytes();
			char* temp = (char*)malloc(this->mBufferSize);
			memcpy(temp, this->mBuffer,
				this->ReadableBytes() + this->mIndexRead);
			free(this->mBuffer);
			this->mBuffer = temp;
		}
	}

	char* BeginWrite()
	{
		return this->mBuffer + this->mIndexWrite;
	}


	CWRecursiveLock& GetLock()
	{
		return this->mLock;
	}


private:

	char *mBuffer;
	size_t mBufferSize;

	size_t mIndexRead;
	size_t mIndexWrite;

	CWRecursiveLock mLock;

	static const size_t g_nPrepend = 1024;
	static const size_t g_nInitSize = 1024;

};