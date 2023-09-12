/************************************************************************
 * Copyright (c) 2023 Ophir Carmi
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef PROTOBUF_EDITOR_SRC_PROTOBUF_EDITOR_H_
#define PROTOBUF_EDITOR_SRC_PROTOBUF_EDITOR_H_

#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif                   // IMGUI_IMPL_OPENGL_ES2
#include <GLFW/glfw3.h>  // Will drag system OpenGL headers

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "imgui_includes.h"
#include "log.h"
#include "protobuf_include.h"

class ProtobufEditor {
 public:
  ProtobufEditor() {}
  int Init();
  void MainLoop();
  void Stop();

 private:
  void OneIteration();
  void MainScreen();

  void SetBoolField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedBoolField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetEnumField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedEnumField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetFloatField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedFloatField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetDoubleField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedDoubleField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetBytesField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetStringField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedStringField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetIntField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedIntField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetUintField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void SetRepeatedUintField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool SetMessageField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool SetRepeatedMessage(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool SetNonRepeatedMessage(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool AddRemoveField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc,
                      const std::string& name);
  bool AddRemoveRepeatedField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc,
                              int ind, int size, const std::string& name);
  bool SetFields(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool IsSet(const ::google::protobuf::Message& msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool Tree(::google::protobuf::Message* msg);
  bool NewField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool RemoveSimpleField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc,
                         const std::string& name);

  void InputText(const std::string& name, std::string* str);
  void AllRepeatedFloatVals(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void AllValsAddRemove(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc,
                        std::string* all_vals, std::vector<std::string>* all_vals_vec, int size);
  void AllRepeatedStringVals(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  void WantToClose(bool* tried_to_load);
  void SaveFile(bool* cant_save, std::string* error_str);
  void Save(bool* cant_save, std::string* error_str, const std::string& path);

  bool SelectFieldToAdd(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);
  bool NewMessageField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  bool SelectRepeatedField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc,
                           int ind);

  void SetRepeatedUintFieldInner(::google::protobuf::Message* msg,
                                 const ::google::protobuf::FieldDescriptor* field_desc, const std::string& name, int k,
                                 int size, uint32_t val, const std::string& val_str, bool* should_break);

  bool SelectRepeatedMessage(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  bool SelectFieldsToAdd(::google::protobuf::Message* msg);

  void SetNonRepeatedUintField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedBoolField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedFloatField(::google::protobuf::Message* msg,
                                const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedIntField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedDoubleField(::google::protobuf::Message* msg,
                                 const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedEnumField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedStringField(::google::protobuf::Message* msg,
                                 const ::google::protobuf::FieldDescriptor* field_desc);

  void SetNonRepeatedBytesField(::google::protobuf::Message* msg,
                                const ::google::protobuf::FieldDescriptor* field_desc);
  GLFWwindow* window_ = nullptr;
  std::string file_path_ = "/home/ophir/temp/sandbox/1.proto";

  ImGuiContext* g_im_gui_ = nullptr;

  std::vector<std::string> add_fields_;

  std::string selected_field_to_add_;

  ::google::protobuf::Message* field_waiting_to_be_added_ = nullptr;

  protobuf::editor::MyRecord the_record_;
};

#endif  // PROTOBUF_EDITOR_SRC_PROTOBUF_EDITOR_H_
