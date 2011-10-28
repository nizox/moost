#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_

#include <string>
#include <list>

namespace moost {


class FileSystem
{
public:
  static FileSystem* getInstance(void);
  static void delInstance(void);

  bool	isADirectory(const std::string &path) const;
  const std::string& getCurrentWorkingDirectory(void) const;
  bool setCurrentWorkingDirectory(const std::string& path);
  std::list<std::string> listCurrentWorkingDirectory(void) const;

private:
  FileSystem(void);
  ~FileSystem(void);
  static FileSystem* _instance;

  std::string _cwd;
};


} // namespace moost

#endif // !_FILESYSTEM_H_
