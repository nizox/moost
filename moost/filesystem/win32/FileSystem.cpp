#include <Windows.h>
#include <sys/stat.h>
#include <new>

#include "FileSystem.h"

namespace moost {


FileSystem* FileSystem::_instance = NULL;

FileSystem* FileSystem::getInstance(void)
{
  if (FileSystem::_instance == NULL)
    FileSystem::_instance = new FileSystem;
  return FileSystem::_instance;
}

// Public methods
void FileSystem::delInstance(void)
{
  if (FileSystem::_instance != NULL)
  {
    delete FileSystem::_instance;
    FileSystem::_instance = NULL;
  }
}

const std::string& FileSystem::getCurrentWorkingDirectory(void) const
{
  return this->_cwd;
}

bool	FileSystem::isADirectory(const std::string &path) const
{
	struct _stat buf;


	if (_stat(path.c_str(), &buf) == -1)
		return false;
	if (!_S_IFDIR & buf.st_mode)
		return false;
	return true;
}

bool FileSystem::setCurrentWorkingDirectory(const std::string& path)
{
	struct _stat buf;

	if (_stat(path.c_str(), &buf) == -1)
		return false;
	if (!_S_IFDIR & buf.st_mode)
		return false;
	this->_cwd = path;
	return true;
}

std::list<std::string> FileSystem::listCurrentWorkingDirectory(void) const
{
  HANDLE handle;
  WIN32_FIND_DATA data;
  std::list<std::string> files;

  handle = FindFirstFile(".\\*", &data);
  if (handle != INVALID_HANDLE_VALUE)
  {
    do
    {
      files.push_back(data.cFileName);
    }
    while (FindNextFile(handle, &data));
  }
  return files;
}

// Constructor/Destructor
FileSystem::FileSystem(void) :
    _cwd(".")
{
  DWORD size;
  LPTSTR path;

  size = GetCurrentDirectory(0, NULL);
  path = new (std::nothrow) char [size];
  if (path != NULL)
  {
    if (GetCurrentDirectory(size, path))
      this->_cwd = path;
    delete [] path;
  }
}

FileSystem::~FileSystem(void)
{
}


} // namespace moost
