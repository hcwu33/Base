#pragma once
#include <exception>
#include <boost/exception/all.hpp>
#include <boost/utility/string_ref.hpp>
#include <boost/cstdint.hpp>
#include <cassert>

#define W_OK				0			// 成功
#define W_ERROR				-1			// 失败
#define W_NULLPTR			-2			// 指针为空
#define W_AGAIN				-3			// 未准备好，需重试

class CWException final : public virtual std::exception,
	public virtual boost::exception
{
public:
	typedef boost::string_ref string_ref_type;

private:
	boost::int32_t m_code;
	std::string m_msg;

public:
	CWException(const boost::int32_t code, string_ref_type msg) :
		m_code(code), m_msg(msg)
	{}
	CWException(const boost::int32_t code = W_ERROR) :CWException(code, "")
	{}
	CWException(string_ref_type msg) :CWException(W_ERROR, msg)
	{}
	virtual ~CWException()throw()
	{}

public:
	boost::int32_t code()const
	{
		return m_code;
	}
	virtual const char* what() const throw() override
	{
		return m_msg.c_str();
	}

public:
	static void raise(const boost::int32_t code = W_ERROR, string_ref_type msg = "")
	{
		//assert(false);
		//_set_error_mode(_OUT_TO_MSGBOX);
		throw CWException(code, msg);
	}
	// 检查条件是否满足
	static void require(bool cond, boost::int32_t code = W_ERROR, string_ref_type msg = "")
	{
		if (!cond)
		{
			raise(code, msg);
		}
	}
	// 检查错误码
	static void require(boost::int32_t code, boost::int32_t ck = W_OK)
	{
		require(code == ck, code);
	}
	// 检查空指针
	template<typename T>
	static void require(T *p, boost::int32_t code = W_NULLPTR)
	{
		require(nullptr != p, code, "point is null");
	}

	static void fail(bool cond, boost::int32_t code = W_ERROR)
	{
		if (cond)
		{
			raise(code);
		}
	}
protected:
private:
};