#pragma once
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/cstdint.hpp>
#include <boost/filesystem.hpp>

using boost::property_tree::ptree;

class CWXmlConfig
{
public:
	CWXmlConfig() = default;
	~CWXmlConfig() = default;
	bool Init(std::string strFile)
	{
		if (m_bInit)
		{
			return true;
		}
		m_bInit = true;
		m_strFile = strFile;
		if (!boost::filesystem::exists(strFile))
		{
			try
			{
				write_xml(strFile, m_ptRead);
			}
			catch (boost::property_tree::xml_parser_error e)
			{
				//CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
				this->UnInit();
				return false;
			}
		}

		read_xml(strFile, m_ptRead);

		return true;
	}
	void UnInit()
	{
		if (!m_bInit)
		{
			return;
		}
		m_bInit = false;
	}

	bool GetConfigData(const std::string &strSectionName,
		const std::string &strKeyName, 
		std::string &strKeyVal, 
		const std::string &strDefaultVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;
		try
		{
			strKeyVal = m_ptRead.get<std::string>(keyName);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::trace);
			strKeyVal = strDefaultVal;
		}

		return true;
	}
	bool SetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		const std::string strKeyVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			m_ptWrite.put<std::string>(keyName, strKeyVal);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
			//return false;
		}
		return true;
	}

	bool GetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		uint32_t &nKeyVal,
		const uint32_t nDefaultVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			nKeyVal = m_ptRead.get<uint32_t>(keyName);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::trace);
			nKeyVal = nDefaultVal;
		}
		return true;
	}
	bool SetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		const uint32_t nKeyVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			m_ptWrite.put<uint32_t>(keyName, nKeyVal);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
		}
		return true;
	}

	bool GetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		uint16_t &nKeyVal,
		const uint16_t nDefaultVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			nKeyVal = m_ptRead.get<uint16_t>(keyName);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::trace);
			nKeyVal = nDefaultVal;
		}
		return true;
	}
	bool SetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		const uint16_t nKeyVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			m_ptWrite.put<uint16_t>(keyName, nKeyVal);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
		}
		return true;
	}

	bool GetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		uint8_t &bKeyVal,
		const uint8_t bDefaultVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			bKeyVal = m_ptRead.get<uint8_t>(keyName);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::trace);
			bKeyVal = bDefaultVal;
		}
		return true;
	}
	bool SetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		const uint8_t bKeyVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			m_ptWrite.put<uint8_t>(keyName, bKeyVal);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
		}
		return true;
	}

	bool GetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		double &dKeyVal,
		const double dDefaultVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			dKeyVal = m_ptRead.get<double>(keyName);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::trace);
			dKeyVal = dDefaultVal;
		}
		return true;
	}
	bool SetConfigData(const std::string &strSectionName,
		const std::string &strKeyName,
		const double dKeyVal)
	{
		std::string keyName = strSectionName + "." + strKeyName;

		try
		{
			m_ptWrite.put<double>(keyName, dKeyVal);
		}
		catch (boost::property_tree::ptree_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
		}
		return true;
	}

	bool WriteFile()
	{
		try
		{
			//boost::property_tree::xml_writer_settings<string> settings('\t', 1, "utf-8");
			boost::property_tree::xml_writer_settings<ptree::key_type> settings('\t', 1, "GB2312");
			write_xml(m_strFile, m_ptWrite, std::locale(), settings);
			//write_xml(m_strFile, m_pt);
		}
		catch (boost::property_tree::xml_parser_error e)
		{
			//		CWriteLog::WriteLog(defClassName, e.what(), CWriteLog::error);
			return false;
		}

		return true;
	}
protected:
private:
	ptree m_ptRead;
	ptree m_ptWrite;
	bool m_bInit = false;
	std::string m_strFile;
};