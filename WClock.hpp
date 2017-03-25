#pragma once

#include <boost/date_time/gregorian/gregorian.hpp> 
#include <boost/date_time/local_time/local_time.hpp> 
#include <sstream>
using namespace boost::local_time;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace std;
struct w_time 
{
	boost::uint32_t sec;
	boost::uint64_t msec;
};

class CWClock
{

public:
	
	CWClock()
	{
		m_epoch = boost::posix_time::microsec_clock::universal_time();
	}
	~CWClock() = default;

	static const w_time now()
	{
		boost::posix_time::ptime time_begin(boost::gregorian::date(1970, 1, 1));
		boost::posix_time::time_duration time_from_begin =
			boost::posix_time::microsec_clock::universal_time() - time_begin;
		w_time t;
		t.msec = time_from_begin.total_milliseconds();
		t.sec = time_from_begin.total_seconds();
		return t;
	}
	w_time delta() const
	{
		boost::posix_time::time_duration time_from_epoch
			= boost::posix_time::microsec_clock::universal_time() - m_epoch;
		w_time t;
		t.msec = time_from_epoch.total_milliseconds();
		t.sec = time_from_epoch.total_seconds();
		return t;
	}
	void reset()
	{
		m_epoch = boost::posix_time::microsec_clock::universal_time();
	}
	
protected:
private:
	boost::posix_time::ptime m_epoch;
	
};