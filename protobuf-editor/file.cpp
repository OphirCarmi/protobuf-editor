#include "file.h"

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>

#include "log.h"

bool regular_file_exists(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode)) &&
         access(name.c_str(), R_OK) != -1;
}

