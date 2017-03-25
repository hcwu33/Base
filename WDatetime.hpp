#pragma once
class CWDatetime
{
public:
	typedef boost::string_ref string_ref_type;

	CWDatetime() = default;
	~CWDatetime() = default;

	static std::string get_str_cur_date()
	{
		ptime now = second_clock::local_time();
		return to_iso_extended_string(now.date());
	}
	static std::string get_str_cur_date(string_ref_type sep)
	{
		ptime now = second_clock::local_time();
		ostringstream oStr;
		oStr << now.date().year() << sep
			<< setw(2) << setfill('0') << (int)now.date().month() << sep
			<< setw(2) << setfill('0') << now.date().day();
		return oStr.str();
	}
	static std::string get_str_cur_time()
	{
		ptime now = second_clock::local_time();
		ostringstream oStr;
		oStr << setw(2) << setfill('0') << (int)now.time_of_day().hours()
			<< ":" << setw(2) << setfill('0') << (int)now.time_of_day().minutes()
			<< ":" << setw(2) << setfill('0') << (int)now.time_of_day().seconds();
		return oStr.str();
	}
	static std::string get_str_cur_time(string_ref_type sep)
	{
		ptime now = second_clock::local_time();
		ostringstream oStr;
		oStr << setw(2) << setfill('0') << now.time_of_day().hours()
			<< sep << setw(2) << setfill('0') << now.time_of_day().minutes()
			<< sep << setw(2) << setfill('0') << now.time_of_day().seconds();
		return oStr.str();
	}
	static std::string get_str_date_time()
	{
		string str;
		str = get_str_cur_date() + " " + get_str_cur_time();
		return str;
	}
	static std::string get_str_ms()
	{
		ptime now = second_clock::local_time();
		ostringstream oStr;
		oStr << setw(3) << setfill('0') << now.time_of_day().total_milliseconds();
		return oStr.str();
	}
protected:
private:
};