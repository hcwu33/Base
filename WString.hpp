#pragma once
#include <boost/utility/string_ref.hpp>
#include <boost/cstdint.hpp>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
using namespace std;
class CWString
{
public:
	typedef boost::string_ref string_ref_type;
	typedef boost::wstring_ref wstring_ref_type;
	CWString() = default;
	~CWString() = default;
	
	template<typename ... Args>
	static void sprintf(char* dest, const size_t nBuffSize, const Args& ... args)
	{
		CWString::sprintf_s((char*)dest, nBuffSize, args ...);
	}

	template<typename ... Args>
	static void wsprintf(wchar_t* dest, const size_t nBuffSize, const Args& ... args)
	{
		CWString::wsprintf_s(dest, nBuffSize, args ...);
	}
	
	static void strcpy(char *dest, const size_t dest_size, const char* src)
	{
#ifdef WIN32
		strcpy_s(dest, dest_size, src);
#else
		size_t minsize = strlen(src);
		if (minsize > dest_size-1)//sizeOfDest-1,最后一位必须为‘\0’
		{
			minsize = dest_size - 1;
		}

		memcpy(dest, src, minsize);
		dest[minsize+1]='\0';
#endif
	}

	static void wcscpy(wchar_t *dest, const size_t dest_size, const wchar_t* src)
	{
#ifdef WIN32
		::wcscpy_s(dest, dest_size, src);
#else
		::wcscpy(dest, src);
#endif
	}
	template<size_t size>
	static void wcscpy(wchar_t (&dest)[size], const wchar_t* src)
	{
#ifdef WIN32
		::wcscpy_s(dest, src);
#else
		::wcscpy(dest, src);
#endif
	}

	static std::wstring c2w(string_ref_type strSrc)
	{
		string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
		setlocale(LC_ALL, "chs");
		std::wstring val = L"";

		if (nullptr == strSrc.data())
		{
			return val;
		}
		
		size_t char_num = strSrc.size()+1;
		if (char_num <= 1)
		{
			return val;
		}
		size_t convert_size = 0;
		wchar_t * pw = new wchar_t[char_num];
#ifdef WIN32
		mbstowcs_s(&convert_size, pw, char_num, strSrc.data(), _TRUNCATE);
#else
		convert_size = mbstowcs(pw, strSrc.data(), char_num);
#endif
		val = pw;
		delete[]pw;
		setlocale(LC_ALL, curLocale.c_str());
		return val;
	}
	static std::string w2c(wstring_ref_type strSrc)
	{
		string curLocale = setlocale(LC_ALL, NULL);   //curLocale="C"
		setlocale(LC_ALL, "chs");
		std::string val = "";
		if (nullptr == strSrc.data())
		{
			return val;
		}
		size_t char_num = strSrc.size() + 1;
		size_t conver_num;
#ifdef WIN32
		wcstombs_s(&conver_num, NULL, 0, strSrc.data(), _TRUNCATE);
#else
		conver_num = wcstombs(NULL, strSrc.data(), 0);  
#endif
		char_num = conver_num + 1;
		if (char_num <= 1)
		{
			return val;
		}
		char *pc = new char[char_num];
		if (nullptr == pc)
		{
			return val;
		}
#ifdef WIN32		
		wcstombs_s(&conver_num, pc, char_num, strSrc.data(), _TRUNCATE);
#else
		conver_num = wcstombs(pc, strSrc.data(), char_num);
#endif
		val = pc;
		delete[]pc;
		setlocale(LC_ALL, curLocale.c_str());
		return val;
	}
protected:
	static int sprintf_s(char* szDest, boost::uint32_t sizeOfDest, const char* szFmt, ...)
	{
		va_list argList;

		va_start(argList, szFmt);// 取头个参数地址

// 		const int nBuffSize = 5000;
// 		char szTemp[nBuffSize];

#ifdef WIN32
		vsprintf_s(szDest, sizeOfDest, szFmt, argList);
#else
		vsnprintf(szDest, sizeOfDest, szFmt, argList);
#endif

		va_end(argList);

//		dt_strcpy_s(szDest, sizeOfDest, szTemp);

		return 0;
	}
	static int wsprintf_s(wchar_t* szDest, boost::uint32_t sizeOfDest, const wchar_t* szFmt, ...)
	{
		va_list argList;

		va_start(argList, szFmt);// 取头个参数地址

		// 		const int nBuffSize = 5000;
		// 		char szTemp[nBuffSize];

#ifdef WIN32
		vswprintf_s(szDest, sizeOfDest, szFmt, argList);
#else
		::vswprintf(szDest, sizeOfDest, szFmt, argList);
#endif

		va_end(argList);

		//		dt_strcpy_s(szDest, sizeOfDest, szTemp);

		return 0;
	}
private:
	//string_ref_type m_str;
};
