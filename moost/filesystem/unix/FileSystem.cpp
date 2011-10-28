#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
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

  bool FileSystem::isADirectory(const std::string &path) const
  {
    struct stat buf;

    if (stat(path.c_str(), &buf) == -1)
      return false;
    if (!S_ISDIR(buf.st_mode))
      return false;
    return true;
  }

  const std::string& FileSystem::getCurrentWorkingDirectory(void) const
  {
    return this->_cwd;
  }

  bool FileSystem::setCurrentWorkingDirectory(const std::string& path)
  {
    struct stat buf;

    if (stat(path.c_str(), &buf) == -1)
      return false;
    if (!S_ISDIR(buf.st_mode))
      return false;
    this->_cwd = path;
    return true;
  }

  std::list<std::string> FileSystem::listCurrentWorkingDirectory(void) const
  {
    DIR *dir;
    struct dirent* entry;
    std::list<std::string> files;

    dir = opendir(this->_cwd.c_str());
    if (dir != NULL)
      while ((entry = readdir(dir)) != NULL)
	files.push_back(entry->d_name);
    return files;
  }

  // Constructor/Destructor
  FileSystem::FileSystem(void) :
    _cwd(".")
  {
    char* path;

    path = getcwd(NULL, 0);
    if (path != NULL)
      {
	this->_cwd = path;
	free(path);
      }
  }

  FileSystem::~FileSystem(void)
  {
  }


} // namespace moost
