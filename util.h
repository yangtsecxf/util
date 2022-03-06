#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <iomanip>
#include <fstream>
#include <regex>

namespace util
{

   static bool read_file(const char* filepath, void*& buf, int64_t& size)
	{
		// open file
		FILE* file_yv12 = nullptr;
#ifdef _WIN32
		fopen_s(&file_yv12, filepath, "r");
#else
		file_yv12 = fopen(filepath, "r");
#endif
		if (!file_yv12)
		{
			return false;
		}

		// file size
		fseek(file_yv12, 0L, SEEK_END);
		size = ftell(file_yv12);

		// read file to buf
		fseek(file_yv12, 0L, SEEK_SET);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
		buf = malloc((size_t)size);
		fread(buf, 1, (size_t)size, file_yv12);

		// close file
		fclose(file_yv12);

		return true;
	}

   static bool write_file(const char* filepath, const void* buf, const int64_t size)
   {
	   FILE* file_rgba = NULL;
#ifdef _WIN32
	   fopen_s(&file_rgba, filepath, "wb");
#else
	   file_rgba = fopen(filepath, "wb");
#endif
	   if (!file_rgba)
	   {
		   return false;
	   }
	   fwrite(buf, 1, (size_t)size, file_rgba);
	   fclose(file_rgba);

	   return true;
   }

   static bool read_file(const wchar_t* filepath, void*& buf, size_t& size)
   {
	   // open file
	   FILE* file_yv12 = nullptr;
	   if (!file_yv12)
	   {
		   return false;
	   }

	   // file size
	   fseek(file_yv12, 0L, SEEK_END);
	   size = ftell(file_yv12);

	   // read file to buf
	   fseek(file_yv12, 0L, SEEK_SET);
	   buf = malloc(size);
	   fread(buf, 1, size, file_yv12);

	   // close file
	   fclose(file_yv12);

	   return true;
   }

   static bool write_file(const wchar_t* filepath, const void* buf, const size_t size)
   {
	   FILE* file_rgba = NULL;
	   if (!file_rgba)
	   {
		   return false;
	   }
	   fwrite(buf, 1, size, file_rgba);
	   fclose(file_rgba);

	   return true;
   }

   static std::string read_file(const char* file)
   {
	   std::ifstream in(file);

	   if (!in)
		   throw std::invalid_argument("could not open file");

	   std::string content;

	   in.seekg(0, std::ios::end);
	   content.reserve(in.tellg());
	   in.seekg(0, std::ios::beg);

	   content.assign(
		   (std::istreambuf_iterator<char>(in)),
		   std::istreambuf_iterator<char>());

	   return content;
   }

	// microseconds cost calculate functions, example:
	/*
	auto start = util::cost_start();
	......
	int64_t cost = util::cost_end(start);
	*/
   static std::chrono::steady_clock::time_point cost_start()
	{
		return std::chrono::steady_clock::now();
	}

   static int64_t cost_end(std::chrono::steady_clock::time_point& start)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start).count();
	}

	// from 1970-01-01 08:00:00 to now, return microseconds
   static int64_t get_now_us()
	{
		std::chrono::system_clock::time_point start;
		int64_t now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count();
		return now;
	}

   static int64_t get_now_ms()
	{
		std::chrono::system_clock::time_point start;
		int64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
		return now;
	}

	// from 1970-01-01 08:00:00 to now, return "YYYY-MM-DD HH:MM:SS"
   static std::string get_now_datetime()
	{
		std::stringstream ss;
		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
		return ss.str();
	}

   static std::wstring get_now_datetime_w()
	{
		std::wstringstream ss;
		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ss << std::put_time(std::localtime(&t), L"%Y-%m-%d_%X");
		return ss.str();
	}

	// from 1970-01-01 08:00:00 to now, return "HH:MM:SS"
   static std::string get_now_time_ms()
	{
		std::stringstream ss;

		auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		ss << std::put_time(std::localtime(&t), "%X");  // HH:MM:SS

		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();  // .MMM

		return ss.str();
	}

	// from 1970-01-01 08:00:00 to time, return "HH:MM:SS"
   static std::string get_time_ms(int64_t time)
	{
		std::chrono::milliseconds dur(time);
		std::chrono::time_point<std::chrono::system_clock> tp(dur);
		auto t = std::chrono::system_clock::to_time_t(tp);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&t), "%X");  // HH:MM:SS
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()) % 1000;
		ss << '.' << std::setfill('0') << std::setw(3) << ms.count();  // .MMM

		return ss.str();
	}

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif
   static std::string get_current_working_dir()
	{
		char *buffer = nullptr;

#ifdef _WIN32
		if ((buffer = _getcwd(NULL, 0)) == NULL)
#else
		if ((buffer = getcwd(NULL, 0)) == NULL)
#endif
		{
			throw std::exception();
		}

		std::string stwimg(buffer);
		free(buffer);

		return stwimg + "\\";
	}

   // should init seed first, call "srand((unsigned)time(0));"
   static int random(int begin, int end)
	{
		int range;
		if (begin == end)
		{
			return begin;
		}
		else if (begin > end)
		{
			range = begin - end + 1;
			return end + rand() % range;
		}
		else
		{
			range = end - begin + 1;
			return begin + rand() % range;
		}
	}

   static std::vector<std::string> s_split(const std::string& in, const std::string& delim)
	{
		std::regex re{ delim };
		return std::vector<std::string> { std::sregex_token_iterator(in.begin(), in.end(), re, -1), std::sregex_token_iterator()};
	}

   static std::vector<std::wstring> ws_split(const std::wstring& in, const std::wstring& delim)
	{
		std::wregex re{ delim };
		return std::vector<std::wstring> {
			std::wsregex_token_iterator(in.begin(), in.end(), re, -1),
				std::wsregex_token_iterator()
		};
	}

   static void s_replace(std::string& src, std::string old, std::string neww)
	{
		size_t pos = src.find(old);
		while (pos != std::string::npos)
		{
			if (pos != std::string::npos)
			{
				src.replace(pos, 1, neww);
				pos = src.find(old);
			}
		}
	}

   static void s_replace_w(std::wstring& str, std::wstring old_str, std::wstring new_str)
	{
		size_t r = str.find(old_str);
		while (r != std::wstring::npos)
		{
			if (r != std::wstring::npos)
			{
				str.replace(r, 1, new_str);
				r = str.find(old_str);
			}
		}
	}

   static bool is_num(const std::string& s)
   {
	   std::stringstream sin(s);
	   double t;
	   char p;
	   if (!(sin >> t))
		   /*解释：
			   sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
		   */
		   return false;
	   if (sin >> p)
		   /*解释：此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，如果此时传入字符串是数字加字符串的输入形式，则此部分可以识别并接收字符部分，例如上面所说的，接收的是.f这部分，所以条件成立，返回false;如果剩下的部分不是字符，那么则sin>>p就为0,则进行到下一步else里面
			 */
		   return false;
	   else
		   return true;
   }

   class InstrumentationTimer
   {
   public:
	   InstrumentationTimer(const char* name)
		   : name_(name)
	   {
		   start_ = util::cost_start();
	   }
	   ~InstrumentationTimer()
	   {
		   int64_t cost = util::cost_end(start_);
		   std::cout << name_ << " cost(us): " << cost << std::endl;
	   }

   private:
	   std::chrono::steady_clock::time_point start_;
	   const char* name_;
   };

#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCTION__)
#else
#define PROFILE_SCOPE(name)
#endif

}

static void logx()
{
	std::cout << "[tid:" << std::this_thread::get_id() << "]" << "[" << util::get_now_time_ms() << "]" << std::endl;
}

template<typename T, typename...Args>
static void logx(const T& head, const Args&... rest)
{
	std::cout << std::dec << head << " ";
	logx(rest...);
}