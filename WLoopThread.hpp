#pragma once
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class CWLoopThread
{
public:
	typedef boost::function<int()> ThreadCallback;

	CWLoopThread(void) = default;
	~CWLoopThread(void) = default;

	bool Init(const std::string strThreadName,
		ThreadCallback cbRunning,
		ThreadCallback cbBegin = NULL,
		ThreadCallback cbEnd = NULL)
	{
		this->SetRun(false);
		m_cbRunning = cbRunning;
		this->m_strThreadName = strThreadName;

		return true;
	}

	bool Start()
	{
		boost::unique_lock<boost::mutex> lock(this->m_trdMutex);
		if (this->IsRun())
		{
			return true;
		}
		this->SetRun(true);
		m_pThread.reset(new boost::thread(boost::bind(&CWLoopThread::ThreadRun, this)));

		return true;
	}
	void Stop(boost::int32_t waitTime = 2000)
	{
		boost::unique_lock<boost::mutex> lock(this->m_trdMutex);
		if (!this->IsRun())
		{
			return;
		}
		this->SetRun(false);
		if (m_pThread->joinable())
		{
			//bool bRes = m_pThread->timed_join(boost::posix_time::seconds(10));
			bool bRes = m_pThread->timed_join(boost::posix_time::milliseconds(waitTime));

			//boost::posix_time::microseconds(waitTime)
			BOOST_ASSERT(bRes);
		}
	}

	bool IsRun()
	{
		return m_bRun;
	}
protected:
	void SetRun(bool bRun)
	{
		this->m_bRun = bRun;

	}

	void ThreadRun()
	{
		if (m_cbBegin != NULL)
		{
			this->m_cbBegin();
		}

		while (this->IsRun())
		{
			int nSleepTimes = this->m_cbRunning();
			if (nSleepTimes <= -1)
			{
				break;
			}
			else if (nSleepTimes > 0)
			{
				boost::this_thread::sleep(boost::posix_time::milliseconds(nSleepTimes));
			}
		}

		if (this->m_cbEnd != NULL)
		{
			this->m_cbEnd();
		}
	}
private:
	boost::mutex m_trdMutex;
	boost::shared_ptr<boost::thread> m_pThread; 
	ThreadCallback m_cbRunning = NULL;
	ThreadCallback m_cbBegin = NULL;
	ThreadCallback m_cbEnd = NULL;
	std::string m_strThreadName;
	bool m_bRun = false;
};

