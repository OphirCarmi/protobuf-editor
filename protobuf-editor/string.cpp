#include "string.h"

#include <algorithm>
#include <fstream>

void split_by_multiple_delimiters(const std::string &delims, const std::string &str, std::vector<std::string> *out) {
  if (nullptr == out) {
    return;
  }
  size_t beg, pos = 0;
  out->clear();
  while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos) {
    pos = str.find_first_of(delims, beg + 1);
    out->push_back(str.substr(beg, pos - beg));
  }
}
