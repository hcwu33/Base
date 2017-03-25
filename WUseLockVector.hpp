#pragma once
#include "WRecursiveLock.hpp"

#include <vector>
using namespace std;

template <typename T>
class CWUseLockVector  
{
public:
	CWUseLockVector() = default;
	~CWUseLockVector() = default;

	void PushBack(const T& data)
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		this->m_vector.push_back(data);
	}
	int GetItemCount()
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		return this->m_vector.size();
	}
	BOOL GetDataByIndex(const uint32_t nIndex, T& data)
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		if (nIndex < 0 || nIndex >= this->m_vector.size())
		{
			return FALSE;
		}

		data = this->m_vector[nIndex];
		return TRUE;
	}
	void DelDataByIndex(const uint32_t nIndex)
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		if (nIndex < 0 || nIndex >= this->m_vector.size())
		{
			return;
		}

		typename std::vector<T>::iterator it = m_vector.begin();
		for (int i = 0; it != m_vector.end(); it++, i++)
		{
			if (nIndex == i)
			{
				this->m_vector.erase(it);
				return;
			}
		}
	}
	void DelAllData()
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		this->m_vector.clear();
	}
	void DelData(const T& data)
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);

		typename vector<T>::iterator it = m_vector.begin();
		for (int i = 0; it != m_vector.end(); it++, i++)
		{
			if (&data == &this->m_vector[i])
			{
				this->m_vector.erase(it);
				return;
			}
		}

	}

	typedef BOOL (*ForEachCallback)(void* lpUser, T& data);	
	void ForEach(void* lpUser, ForEachCallback action)// 回调函数返回FALSE，则退出函数
	{
		CWRecursiveLockGuard sync(this->m_mutexVector);
		for (int i = this->m_vector.size() - 1; i >= 0; i--)
		{
			T& a = this->m_vector[i];
			action(lpUser, this->m_vector[i]);
		}
	}


	//T& operator [](int i);

	CWRecursiveLock* GetLock()
	{
		return &m_mutexVector;
	}

public:
	std::vector<T> m_vector;

private:
	CWRecursiveLock m_mutexVector;
};
