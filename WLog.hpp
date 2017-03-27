#pragma once
#include "WDef.hpp"
#include "WXmlConfig.hpp"
#include "WLoopThread.hpp"
#include "WCircularBuff.hpp"
#include "WRecursiveLock.hpp"
#include "WFileSystem.hpp"
#include "WDatetime.hpp"
#include "WClock.hpp"

#include <iostream>
#include <fstream>

#include <string>

using namespace std;
class CWLog
{
public:
	typedef boost::string_ref string_ref_type;

	enum LogLevel
	{
		trace,
		debug,
		info,
		warning,
		assert,
		fatal
	};
	CWLog() :m_logFile(&m_fileBuf)
	{

	}
	~CWLog()
	{
		this->StopServer();
		this->UnInit();
	}

	bool Init(const string& strPath, const string& strLogName)
	{
		if (m_bInit)
		{
			return true;
		}
		m_bInit = true;
		m_strLogName = strLogName;
		m_strPath = strPath + "\\logs";
		CWFileSystem::create_directories(m_strPath);

		this->ReadConfigData();
		this->WriteConfigData();

		try
		{
			string strLogPath = m_strPath + "\\" + m_strLogName + ".log";
			m_fileBuf.open(strLogPath, std::ios::out | std::ios::app);
			m_proWriteLog.Init("写日志线程", boost::bind(&CWLog::OnWriteLog, this));
			m_cirString.Init(1000);
		}
		catch (...)
		{
			this->UnInit();
			return true;
		}

		return true;
	}

	void UnInit()
	{
		if (!m_bInit)
		{
			return;
		}
		m_bInit = false;

		m_cirString.UnInit();
		m_configData.UnInit();
		m_fileBuf.close();
	}

	bool StartServer()
	{
		if (m_bServer)
		{
			return true;
		}
		m_bServer = true;
		m_proWriteLog.Start();
		return true;
	}

	void StopServer()
	{
		if (!m_bServer)
		{
			return;
		}
		m_bServer = false;
		m_proWriteLog.Stop();
	}

	bool WriteLog(const LogLevel level, string_ref_type fileName, const uint32_t lineNumber,
		string_ref_type functionName, string_ref_type strLog)
	{
		CWRecursiveLockGuard lock(m_lock);
		string *pStr = m_cirString.GetWriteItem();
		if (NULL == pStr)
		{
			return false;
		}

		if ((uint32_t)level < m_nLogLevel)
		{
			return true;
		}

		char* szLogLevel[] =
		{
			"[trace]",
			"[debug]",
			"[info]",
			"[warning]",
			"[assert]",
			"[fatal]"
		};
		const size_t nSize = 1024;
		char szTemp[nSize];
		uint32_t nMs = (uint32_t)(m_clock.now().msec - (m_clock.now().sec*1000));
		CWString::sprintf(szTemp, nSize, "%s[%s %03d][%s,%d,%s]:%s",
			szLogLevel[level], 
			CWDatetime::get_str_date_time().c_str(), 
			nMs,
			fileName.data(), 
			lineNumber, 
			functionName.data(), strLog.data());
		*pStr = szTemp;

		m_cirString.MoveNextIndexOfWrite();

		return true;
	}

	void ReadConfigData()
	{
		string strConfigPath = m_strPath + "\\" + m_strLogName + ".xml";
		m_configData.UnInit();
		m_configData.Init(strConfigPath.c_str());

		const string LogAttribute = "LogAttribute";
		m_configData.GetConfigData(LogAttribute, "CurIndex", m_nCurIndex, 0);
		m_configData.GetConfigData(LogAttribute, "LogFileMaxCount", m_nLogFileMaxCount, 100);
		m_configData.GetConfigData(LogAttribute, "LogFileMaxSize", m_nLogFileMaxSize, 1000);
		m_configData.GetConfigData(LogAttribute, "LogLevel", m_nLogLevel, 0);
		m_configData.GetConfigData(LogAttribute, "AutoFlush", m_nFlush, 0);

	}

	void WriteConfigData()
	{
		string strConfigPath = m_strPath + "\\" + m_strLogName + ".xml";
		const string LogAttribute = "LogAttribute";
		m_configData.SetConfigData(LogAttribute, "CurIndex", m_nCurIndex);
		m_configData.SetConfigData(LogAttribute, "LogFileMaxCount", m_nLogFileMaxCount);
		m_configData.SetConfigData(LogAttribute, "LogFileMaxSize", m_nLogFileMaxSize);
		m_configData.SetConfigData(LogAttribute, "LogLevel", m_nLogLevel);
		m_configData.SetConfigData(LogAttribute, "AutoFlush", m_nFlush);
		m_configData.WriteFile();
	}

	int OnWriteLog()
	{
		if (!m_bServer)
		{
			return -1;
		}

		string *pStr = m_cirString.GetReadItem();
		if (NULL == pStr)
		{
			return 10;
		}

		m_strLog.swap(*pStr);

		m_cirString.MoveNextIndexOfRead();

		try
		{
			m_logFile << m_strLog << endl;

			if (m_nFlush != 0)
			{
				m_logFile.flush();
			}
			std::ios::pos_type fileSize = m_logFile.tellp();
			if ((uint32_t)fileSize >= m_nLogFileMaxSize * 1024)
			{
				m_logFile.flush();
				m_fileBuf.close();

				string strOldPath = m_strPath + "\\" + m_strLogName + ".log";

				ostringstream oStrNewPath;
				oStrNewPath << m_strPath << "\\" << m_strLogName << m_nCurIndex << ".log";

				//fileRename(strOldPath, oStrNewPaht.str());
				//rename(strOldPath.c_str(), oStrNewPaht.str().c_str());
				if (CWFileSystem::file_exists(oStrNewPath.str()))
				{
					CWFileSystem::delete_file(oStrNewPath.str());
				}
				CWFileSystem::rename(strOldPath, oStrNewPath.str());
				m_fileBuf.open(strOldPath, std::ios::out | std::ios::trunc);

				m_nCurIndex++;
				if (m_nCurIndex > m_nLogFileMaxCount)
				{
					m_nCurIndex = 0;
				}
				WriteConfigData();

			}
		}
		catch (...)
		{
			return 0;
		}

		return 0;
	}


protected:
private:
	bool m_bInit = false;
	bool m_bServer = false;
	std::filebuf m_fileBuf;
	std::ostream m_logFile;

	string m_strPath;
	string m_strLogName;

	CWXmlConfig m_configData;
	uint32_t m_nCurIndex;
	uint32_t m_nLogFileMaxCount;
	uint32_t m_nLogFileMaxSize;
	uint32_t m_nLogLevel;
	uint32_t m_nFlush;

	CWLoopThread m_proWriteLog;

	CWCircularBuff<string> m_cirString;
	string m_strLog;
	CWRecursiveLock m_lock;
	CWClock m_clock;
};

class CWLogger
{
public:
	static bool Init(boost::string_ref strFileName)
	{
		bool &bInit = GetInitFlag();
		if (bInit)
		{
			return true;
		}
		bInit = true;
		string strPath;
		strPath = CWFileSystem::get_exe_file_path();

		GetLog().Init(strPath, strFileName.data());
		GetLog().StartServer();
		return true;
	}
	static void UnInit()
	{
		bool &bInit = GetInitFlag();
		if (!bInit)
		{
			return;
		}
		bInit = false;
		GetLog().StopServer();
		GetLog().UnInit();
	}
	template<typename ... Args>
	static bool WriteLog(const CWLog::LogLevel level,
		const string &fileName, const uint32_t lineNumber,
		const string &functionName, const Args& ... args)
	{
		if (!GetInitFlag())
		{
			return true;
		}

		const int nSize = 1024;
		char szTemp[nSize];
		CWString::sprintf(szTemp, nSize, args ...);

		GetLog().WriteLog(level, fileName, lineNumber, functionName, szTemp);
		return true;
	}

protected:
	CWLogger() = default;
	~CWLogger() = default;

	static CWLog& GetLog()
	{
		static CWLog m_log;
		return m_log;
	}
	static bool& GetInitFlag()
	{
		static bool m_bInit = false;
		return m_bInit;
	}
private:
// 	static CWLog m_log;
// 	static bool m_bInit;
};

// CWLog CWLogger::m_log;
// bool CWLogger::m_bInit = false;

#define WLOG(level, ...) \
do \
{ \
	CWLogger::WriteLog(Level, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

#define W_TRACE_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::trace, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

#define W_DEBUG_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::debug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

#define W_INfO_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

#define W_WARN_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

#define W_ASSERT_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::assert, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
	_set_error_mode(_OUT_TO_MSGBOX); \
} while (0);

#define W_FATAL_LOG(...) \
do \
{ \
	CWLogger::WriteLog(CWLog::fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); \
} while (0);

