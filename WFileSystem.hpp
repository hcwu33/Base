#pragma once
#include "WString.hpp"
#include <boost/utility/string_ref.hpp>
#include <boost/filesystem.hpp>
#include <io.h>
#include <direct.h>
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
		_finddata_t fb;   //查找相同属性文件的存储结构体  
		const int path_len = 250;
		char  path[path_len];
		long    handle;
		int  resultone;
		int   noFile;            //对系统隐藏文件的处理标记  

		noFile = 0;
		handle = 0;


		//制作路径  
		CWString::strcpy(path, path_len, folder_path.data());
		strcat_s(path, path_len, "/*");

		handle = _findfirst(path, &fb);
		//找到第一个匹配的文件  
		if (handle != 0 && -1 != handle)
		{
			//当可以继续找到匹配的文件，继续执行  
			while (0 == _findnext(handle, &fb))
			{
				//windows下，常有个系统文件，名为“..”,对它不做处理  
				noFile = strcmp(fb.name, "..");

				if (0 != noFile)
				{
					//制作完整路径  
					memset(path, 0, sizeof(path));
					CWString::strcpy(path, path_len, folder_path.data());
					strcat_s(path, path_len, "/");
					strcat_s(path, path_len, fb.name);
					//属性值为16，则说明是文件夹，迭代  
					if (fb.attrib == 16)
					{
						CWFileSystem::delete_folder(path, true);
					}
					//非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。  
					else
					{
						remove(path);
					}
				}
			}
			//关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir  
			//经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。  
			_findclose(handle);
		}
		//移除文件夹  
		resultone = _rmdir(folder_path.data());
		return  (0 == resultone) ? true : false;
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