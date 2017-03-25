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
	* �ӿ�����: Init
	* ����: ��ʼ�����ζ���
	* @���� int nCount: ���ζ���Ԫ�صĸ���
	* @����ֵ: BOOL TRUE���ɹ�	FALSE��ʧ��
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
	* �ӿ�����: UnInit
	* ����: ����ʼ�����ζ���
	* @����ֵ: void
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
	// Function:  ��ȡ��λ�õ�Ԫ�أ����Ϊ�գ����ʾû�пɶ�Ԫ��
	//************************************
	T* GetReadItem()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);

		if (!this->m_bInit)
		{
			return NULL;
		}

		if (this->m_nItemCount == 0)				// ���ζ���Ϊ��
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
	// Function:  ����һ��Ԫ�غ��Ƶ���һ����ȡλ�ã����ʧ�ܣ���ʾû�пɶ�Ԫ��
	//************************************
	bool MoveNextIndexOfRead()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);
		if (this->m_nItemCount == 0)				// ���ζ���Ϊ��
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
	// Function:  ��ȡ��дλ�õ�Ԫ�أ����Ϊ�գ����ʾ��������
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
	// Function:  д��һ��Ԫ�غ��Ƶ���һ��λ�ã����ʧ�ܣ����ʾ��������
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
	* �ӿ�����: Reset
	* ����: ���ö�ȡ��д��λ��
	* @����ֵ: void
	*/
	void Reset()
	{
		CWRecursiveLockGuard lock(this->m_mutexCircBuff);
		this->m_nReadIndex = 0;
		this->m_nWriteIndex = 0;
		this->m_nItemCount = 0;
	}
	/**
	* �ӿ�����: GetItemCount
	* ����: ��ȡ��ǰ����Ԫ�صĸ���
	* @����ֵ: int
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