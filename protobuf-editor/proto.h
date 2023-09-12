#ifndef PROTO_H_
#define PROTO_H_

#include <string>
#include "protobuf_include.h"

bool read_file(const std::string &model_path, protobuf::editor::MyRecord *record);

#endif /* PROTO_H_ */
