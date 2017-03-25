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
	* ���ܣ���Ԫ�ز����β
	************************************************************************/
	void AddItemToBack(const T& item)
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.push_back(item);
	}

	/************************************************************************
	* ���ܣ��Ӷ���ͷ����ȡԪ�أ����Ӷ�����ɾ����Ԫ��
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
	* ���ܣ������������Ԫ��
	/************************************************************************/
	void Clear()
	{
		CWRecursiveLockGuard lock(m_mutexList);
		this->m_list.clear();
	}

	/************************************************************************/
	/* ���ܣ���ȡԪ�ظ���
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
