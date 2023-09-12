#include "proto.h"

#include <sstream>

#include "file.h"
#include "log.h"
#include "protobuf_include.h"

bool read_file(const std::string &file_path, protobuf::editor::MyRecord *record) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  const char *file_path_c = file_path.c_str();

  if (!regular_file_exists(file_path)) {
    // File does not exists
    PBE_LOG_ERROR("Can't create file. %s does not exist\r\n", file_path_c);
    return false;
  }

  std::ifstream input(file_path, std::ios::in | std::ios::binary);
  bool ret = record->ParseFromIstream(&input);

  input.close();

  return true;
}
