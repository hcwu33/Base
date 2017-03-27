#pragma once
#include "WString.hpp"
#include <boost/utility/string_ref.hpp>
#include <boost/filesystem.hpp>
//#include <io.h>
//#include <direct.h>
class CWFileSystem
{
public:
	typedef boost::string_ref string_ref_type;
	CWFileSystem() = default;
	~CWFileSystem() = default;

	static bool file_exists(string_ref_type path)
	{
		namespace bf = boost::filesystem;
		bf::path file_path(path.data());

		return bf::exists(file_path);
	}
	static bool is_directory(string_ref_type path)
	{
		namespace bf = boost::filesystem;
		bf::path folder_path(path.data());
		return boost::filesystem::is_directory(folder_path);
	}
	static bool delete_file(string_ref_type file_path)
	{
		namespace bf = boost::filesystem;
		bf::path path(file_path.data());
		return bf::remove(path);
	}
	static bool delete_folder(string_ref_type folder_path, bool force)
	{
		namespace bf = boost::filesystem;
		bf::path path(folder_path.data());
		boost::filesystem::remove_all(path);
		return true;
	}
	static void rename(string_ref_type oldName, string_ref_type newName)
	{
		boost::filesystem::rename(oldName.data(), newName.data());
	}
	static void split_file_name(string_ref_type fileName, string &name, string &extension, char separator = '.')
	{
		size_t dotPosition = fileName.find_last_of(separator);
		if (dotPosition == string::npos) {
			name = fileName.data();
			extension = "";
			return;
		}
		name = fileName.substr(0, dotPosition).to_string();
		extension = fileName.substr(dotPosition + 1).to_string();
	}

	static std::string get_exe_file_path()
	{
		//return boost::filesystem::initial_path<boost::filesystem::path>().string();
		return boost::filesystem::current_path().string();
	}

// 	static std::string get_exe_file_name()
// 	{
// 		return boost::filesystem::basename().c_str();
// 	}

	static int create_directories(const string &strPath)
	{

		int nIndex = 0;

		try
		{
			while ((nIndex = strPath.find_first_of("/\\", nIndex + 1)) != -1)
			{
				string strSub = strPath.substr(0, nIndex);

				if (!boost::filesystem::is_directory(strSub))
				{
					if (!boost::filesystem::create_directories(strSub))
					{
						return -1;
					}
				}
			}
			if (!boost::filesystem::is_directory(strPath))
			{
				boost::filesystem::create_directories(strPath);
			}
		}

		catch (boost::filesystem::filesystem_error e)
		{
			return -1;
		}

		return 0;
	}
protected:
private:
};
