#pragma once
#include "WRecursiveLock.hpp"
#include <vector>
using namespace std;

template <class T>
class CWCircularBuff
{
public:
	CWCircularBuff() = default;
	~CWCircularBuff()
	{
		this->UnInit();
	}

public:
	/**
	* 接口名称: Init
	* 功能: 初始化环形队列
	* @参数 int nCount: 环形队列元素的个数
	* @返回值: BOOL TRUE：成功	FALSE：失败
	*/
	bool Init(size_t nCount)
	{
		if (this->m_bInit)
		{
			this->UnInit();
		}

		this->m_bInit = true;
		this->Reset();

		if (nCount <= 0)
		{
			//assert(FALSE);
			return false;
		}

		this->m_vecItem.resize(nCount);

		for (size_t i = 0; i < nCount; i++)
		{
			this->m_vecItem[i] = new T();
		}

		return true;
	}

	/**
	* 接口名称: UnInit
	* 功能: 反初始化环形队列
	* @返回值: void
	*/
	void UnInit()
	{
		if (!this->m_bInit)
		{
			return;
		}

		for (size_t i = 0; i < this->m_vecItem.size(); i++)
		{
			delete this->m_vecItem[i];
			this->m_vecItem[i] = NULL;
		}
		this->m_vecItem.clear();

		this->m_bInit = false;
		this->Reset();
	}

	//************************************
	// Method:    Read
	// FullName:  CCircularBuff<T>::Read
	// Access:    public 
	// Returns:   T*
	// Qualifier:
	// Function:  获取读位置的元素，如果为空，则表示没有可读元素
	//************************************
	T* GetReadItem()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);

		if (!this->m_bInit)
		{
			return NULL;
		}

		if (this->m_nItemCount == 0)				// 环形队列为空
		{
			return NULL;
		}

		return this->m_vecItem[this->m_nReadIndex];
	}

	//************************************
	// Method:    MoveNextIndexOfRead
	// FullName:  CCircularBuff<T>::MoveNextIndexOfRead
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Function:  读完一个元素后，移到下一个读取位置，如果失败，表示没有可读元素
	//************************************
	bool MoveNextIndexOfRead()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);
		if (this->m_nItemCount == 0)				// 环形队列为空
		{
			return false;
		}

		this->m_nReadIndex++;

		if (this->m_nReadIndex >= this->m_vecItem.size())
		{
			this->m_nReadIndex = 0;
		}

		this->m_nItemCount--;

		return true;
	}

	//************************************
	// Method:    Write
	// FullName:  CCircularBuff<T>::Write
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Parameter: T * pItem
	// Function:  获取在写位置的元素，如果为空，则表示队列已满
	//************************************
	T* GetWriteItem()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);

		if (!this->m_bInit)
		{
			return NULL;
		}

		if (this->m_nItemCount >= this->m_vecItem.size())
		{
			return NULL;
		}

		return this->m_vecItem[this->m_nWriteIndex];
	}

	//************************************
	// Method:    MoveNextIndexOfWrite
	// FullName:  CCircularBuff<T>::MoveNextIndexOfWrite
	// Access:    public 
	// Returns:   BOOL
	// Qualifier:
	// Function:  写完一个元素后，移到下一个位置，如果失败，则表示队列已满
	//************************************
	bool MoveNextIndexOfWrite()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);

		if (this->m_nItemCount >= this->m_vecItem.size())
		{
			return false;
		}
		this->m_nWriteIndex++;
		this->m_nItemCount++;

		if (this->m_nWriteIndex >= this->m_vecItem.size())
		{
			this->m_nWriteIndex = 0;
		}

		return true;
	}
	/**
	* 接口名称: Reset
	* 功能: 重置读取和写入位置
	* @返回值: void
	*/
	void Reset()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);
		this->m_nReadIndex = 0;
		this->m_nWriteIndex = 0;
		this->m_nItemCount = 0;
	}
	/**
	* 接口名称: GetItemCount
	* 功能: 获取当前保存元素的个数
	* @返回值: int
	*/
	int GetItemCount()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);

		return this->m_nItemCount;
	}

private:
	vector<T *> m_vecItem;
	CWRecursiveLock m_mutexCircBuff;

	size_t m_nItemCount = 0;
	size_t m_nReadIndex = 0;
	size_t m_nWriteIndex = 0;
	bool m_bInit = false;

};