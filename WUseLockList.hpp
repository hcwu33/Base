#pragma once

#include "WRecursiveLock.hpp"

#include <list>
using namespace std;

template <typename T>
class CWUseLockList
{
public:
	CWUseLockList() = default;
	~CWUseLockList()
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.clear();
	}
	/************************************************************************
	* 功能：将元素插入队尾
	************************************************************************/
	void AddItemToBack(const T& item)
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.push_back(item);
	}

	/************************************************************************
	* 功能：从队列头部获取元素，并从队列中删除此元素
	************************************************************************/
	BOOL GetItemFromFront(T& item)
	{
		CWRecursiveLockGuard lock(m_mutexList);

		if (this->m_list.size() <= 0)
		{
			return FALSE;
		}

		item = this->m_list.front();
		this->m_list.pop_front();

		return TRUE;
	}

	BOOL Front(T& item)
	{
		CWRecursiveLockGuard lock(m_mutexList);

		if (this->m_list.size() <= 0)
		{
			return FALSE;
		}

		item = this->m_list.front();

		return TRUE;
	}

	void PopFront()
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.pop_front();
	}

	/************************************************************************
	* 功能：清除队列所有元素
	/************************************************************************/
	void Clear()
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.clear();
	}

	/************************************************************************/
	/* 功能：获取元素个数
	/************************************************************************/
	size_t GetItemCount()
	{
		CWRecursiveLockGuard lock(m_mutexList);
		return m_list.size();
	}

	CWRecursiveLock* GetLock()
	{
		return &m_mutexList;
	}

protected:

	list<T> m_list;

private:
	CWRecursiveLock m_mutexList;

};
