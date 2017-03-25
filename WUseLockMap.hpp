#pragma once

#include "WRecursiveLock.hpp"

#include <map>
#include <algorithm>
using namespace std;

template<class T, class V>
class CWUseLockMap  
{
public:
	CWUseLockMap() = default;
	~CWUseLockMap() = default;

	BOOL AddData(const T& key, const V& val)
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);
		typename std::map<T, V>::iterator it = m_mapData.find(key);
		if (it != this->m_mapData.end())
		{
			//ASSERT(FALSE);
			return FALSE;
		}
		this->m_mapData[key] = val;

		return TRUE;

	}
	void DelData(const T& key)
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);
		typename std::map<T, V>::iterator it = m_mapData.find(key);
		if (it == this->m_mapData.end())
		{
			return;
		}
		this->m_mapData.erase(it);
	}
	BOOL FindData(const T& key, V& val)
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);
		typename std::map<T, V>::iterator it = m_mapData.find(key);
		if (it == this->m_mapData.end())
		{
			return FALSE;
		}

		val = it->second;
		return TRUE;
	}
	void Clear()
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);
		this->m_mapData.clear();

	}
	int GetItemCount()
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);
		return this->m_mapData.size();

	}

	void Erase(const T& key)
	{
		CWRecursiveLockGuard sync(this->m_mutexMap);

		this->DelData(key);

	}


	CWRecursiveLock* GetLock()
	{
		return &m_mutexMap;
	}
	BOOL Lock()
	{
		return m_mutexMap.lock();

	}
	BOOL UnLock()
	{
		return m_mutexMap.unlock();
	}

public:
	std::map<T, V> m_mapData;
	CWRecursiveLock m_mutexMap;
};

