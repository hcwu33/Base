#pragma once
#include<string.h>
class CWMemory
{
public:
	CWMemory() = default;
	~CWMemory() = default;
	static int memcpy(void *pDest, const size_t nSizeOfDest, 
		const void* pSrc, const size_t nCpySize)
	{
		
		//memset(dest, 0, sizeOfDest);
#ifdef WIN32
		return memcpy_s(pDest, nSizeOfDest, pSrc, nCpySize);
#else
		size_t minsize = nSizeOfDest;
		if (minsize > nCpySize)
		{
			minsize = nCpySize;
		}

		memcpy(pDest, pSrc, minsize);
		//return memcpy(dest, src, min(sizeOfDest, sizeOfCopy));
		return minsize;
#endif

	}
protected:
private:
};

#define wmemcpy(pDest,nSizeOfDest,pSrc,nCpySize)	\
	CWMemory::memcpy(pDest, nSizeOfDest, pSrc, nCpySize)