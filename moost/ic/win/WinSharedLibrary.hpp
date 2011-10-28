#ifndef ICWINSHAREDLIB_HPP_
#define ICWINSHAREDLIB_HPP_

#include <moost/utils.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace moost {
namespace ic {

class WinSharedLibrary
{
public:
	WinSharedLibrary() : _handle()
	{
	}

	WinSharedLibrary(std::string const &path) : _handle()
	{
		Error err;

		load(path, err);
	}

	~WinSharedLibrary()
	{
		Error err;

		unload(err);
	}

	static std::string suffix()
	{
		return ".dll";
	}

	void load(std::string const &path, Error &err)
	{
		if (!_handle)
		{
			_handle = LoadLibrary(path.c_str());
			if (_handle != NULL)
			{
				err.assign(GetLastError());
			}
		}
	}

	void unload(Error &err)
	{
		if (_handle)
		{
			if (FreeLibrary(this->_handle) == 0)
			{
				err.assign(GetLastError());
			}
		}
	}

	bool has_symbol(std::string const &symbol)
	{
		return get_symbol(symbol) != NULL;
	}

	void *get_symbol(std::string const &symbol)
	{
		return GetProcAddress(this->_handle, symbol.c_str());
	}
private:
	HMODULE _handle;
};

} /* ic */
} /* moost */

#endif