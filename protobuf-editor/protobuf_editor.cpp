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

#include "protobuf_editor.h"

#include <fstream>

#include "clip/clip.h"
#include "file.h"
#include "proto.h"
#include "string.h"
#include "system.h"

static bool is_not_set(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc) {
  return !field_desc->is_required() && !msg->GetReflection()->HasField(*msg, field_desc);
}

int ProtobufEditor::Init() {
  // Setup window
  // glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return 1;
  }

  // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
  // GL ES 2.0 + GLSL 100
  const char* glsl_version = "#version 100";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else   // __APPLE__
  // GL 3.0 + GLSL 130
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif  // __APPLE__

  // Create window with graphics context
  window_ = glfwCreateWindow(1280, 720, "protobuf editor", nullptr, nullptr);
  if (nullptr == window_) {
    return 1;
  }
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(1);  // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  g_im_gui_ = ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window_, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  /* Make the window's context current */
  glfwMakeContextCurrent(window_);

  return 0;
}

bool ProtobufEditor::NewMessageField(::google::protobuf::Message* msg,
                                     const ::google::protobuf::FieldDescriptor* field_desc) {
  ::google::protobuf::Message* field_msg = nullptr;
  if (field_desc->is_repeated()) {
    field_msg = msg->GetReflection()->AddMessage(msg, field_desc);
  } else {
    field_msg = msg->GetReflection()->MutableMessage(msg, field_desc);
  }
  auto* desc = field_msg->GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc2 = desc->field(i);
    if (field_desc2->is_required()) {
      if (!IsSet(*field_msg, field_desc2)) {
        if (!NewField(field_msg, field_desc2)) {
          return false;
        }
      }
    }
  }

  return true;
}

bool ProtobufEditor::NewField(::google::protobuf::Message* msg, const ::google::protobuf::FieldDescriptor* field_desc) {
  switch (field_desc->type()) {
    case ::google::protobuf::FieldDescriptor::TYPE_SINT32:
      /* FALLTHROUGH */
    case ::google::protobuf::FieldDescriptor::TYPE_INT32: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddInt32(msg, field_desc, 0);
      } else {
        msg->GetReflection()->SetInt32(msg, field_desc, 0);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_UINT32: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddUInt32(msg, field_desc, 0);
      } else {
        msg->GetReflection()->SetUInt32(msg, field_desc, 0);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_FLOAT: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddFloat(msg, field_desc, 0);
      } else {
        msg->GetReflection()->SetFloat(msg, field_desc, 0);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_DOUBLE: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddDouble(msg, field_desc, 0);
      } else {
        msg->GetReflection()->SetDouble(msg, field_desc, 0);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_BYTES:
      /* FALLTHROUGH */
    case ::google::protobuf::FieldDescriptor::TYPE_STRING: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddString(msg, field_desc, "");
      } else {
        msg->GetReflection()->SetString(msg, field_desc, "");
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_BOOL: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddBool(msg, field_desc, false);
      } else {
        msg->GetReflection()->SetBool(msg, field_desc, false);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_ENUM: {
      if (field_desc->is_repeated()) {
        msg->GetReflection()->AddEnumValue(msg, field_desc, 0);
      } else {
        msg->GetReflection()->SetEnumValue(msg, field_desc, 0);
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_MESSAGE: {
      if (!NewMessageField(msg, field_desc)) {
        return false;
      }

      break;
    }

    default:
      return false;
  }
  return true;
}

bool ProtobufEditor::SelectRepeatedField(::google::protobuf::Message* msg,
                                         const ::google::protobuf::FieldDescriptor* field_desc, int ind) {
  auto* field_msg = msg->GetReflection()->MutableRepeatedMessage(msg, field_desc, ind);
  field_waiting_to_be_added_ = field_msg;
  auto* desc = field_msg->GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc2 = desc->field(i);
    if (field_desc2->is_required()) {
      if (!IsSet(*field_msg, field_desc2)) {
        if (!NewField(field_msg, field_desc2)) {
          return false;
        }
      }
    }
  }

  return true;
}

bool ProtobufEditor::AddRemoveRepeatedField(::google::protobuf::Message* msg,
                                            const ::google::protobuf::FieldDescriptor* field_desc, int ind, int size,
                                            const std::string& name) {
  if (field_desc->type() == ::google::protobuf::FieldDescriptor::TYPE_MESSAGE) {
    auto& msg2 = msg->GetReflection()->GetRepeatedMessage(*msg, field_desc, ind);
    auto* name_field = msg2.GetDescriptor()->FindFieldByName("name");
    if (nullptr != name_field) {
      ImGui::Text("%s", msg2.GetReflection()->GetString(msg2, name_field).c_str());
      ImGui::SameLine();
    }
  }

  ImGui::SameLine();
  if (ImGui::Button(("X " + name).c_str())) {
    for (int m = ind; m < size - 1; ++m) {
      msg->GetReflection()->SwapElements(msg, field_desc, m, m + 1);
    }
    // delete elements from reflection
    msg->GetReflection()->RemoveLast(msg, field_desc);
    return true;
  }
  return false;
}

bool ProtobufEditor::RemoveSimpleField(::google::protobuf::Message* msg,
                                       const ::google::protobuf::FieldDescriptor* field_desc, const std::string& name) {
  ImGui::SameLine();

  if (ImGui::Button(("X " + name).c_str())) {
    // delete elements from reflection
    msg->GetReflection()->ClearField(msg, field_desc);
    return true;
  }
  return false;
}

bool ProtobufEditor::SelectFieldToAdd(::google::protobuf::Message* msg,
                                      const ::google::protobuf::FieldDescriptor* field_desc) {
  auto* field_msg = msg->GetReflection()->MutableMessage(msg, field_desc);
  field_waiting_to_be_added_ = field_msg;
  auto* desc = field_msg->GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc2 = desc->field(i);
    if (field_desc2->is_required()) {
      if (!IsSet(*field_msg, field_desc2)) {
        if (!NewField(field_msg, field_desc2)) {
          return false;
        }
      }
    }
  }

  return true;
}

bool ProtobufEditor::AddRemoveField(::google::protobuf::Message* msg,
                                    const ::google::protobuf::FieldDescriptor* field_desc, const std::string& name) {
  ImGui::SameLine();

  if (ImGui::Button(("X " + name).c_str())) {
    // delete elements from reflection
    msg->GetReflection()->ClearField(msg, field_desc);
    return true;
  }

  return false;
}

void ProtobufEditor::SetRepeatedIntField(::google::protobuf::Message* msg,
                                         const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddInt32(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }
  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());
  ImGui::SameLine();

  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      int val = msg->GetReflection()->GetRepeatedInt32(*msg, field_desc, k);

      std::string name = field_desc->name() + std::to_string(k);
      ImGui::InputInt(name.c_str(), &val, 1);
      ImGui::SameLine();

      if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
        break;
      }

      msg->GetReflection()->SetRepeatedInt32(msg, field_desc, k, val);
    }
    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedIntField(::google::protobuf::Message* msg,
                                            const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetInt32(msg, field_desc, 0);
    } else {
      return;
    }
  }
  int val = msg->GetReflection()->GetInt32(*msg, field_desc);
  ImGui::InputInt(field_desc->name().c_str(), &val, 1);

  msg->GetReflection()->SetInt32(msg, field_desc, val);
  RemoveSimpleField(msg, field_desc, field_desc->name());
}

void ProtobufEditor::SetIntField(::google::protobuf::Message* msg,
                                 const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedIntField(msg, field_desc);
  } else {
    SetNonRepeatedIntField(msg, field_desc);
  }
}

void ProtobufEditor::SetRepeatedEnumField(::google::protobuf::Message* msg,
                                          const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddEnumValue(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();

  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());

  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      auto* enum_desc = msg->GetReflection()->GetRepeatedEnum(*msg, field_desc, k);
      int num_values = enum_desc->type()->value_count();
      char **names = new char*[static_cast<uint64_t>(num_values)];
      for (int i = 0; i < num_values; ++i) {
        names[i] = strdup(enum_desc->type()->value(i)->name().c_str());
      }
      int selected = enum_desc->index();
      std::string name = field_desc->name() + std::to_string(k);
      ImGui::Combo(name.c_str(), &selected, const_cast<const char **>(names), num_values);
      auto selected_val = enum_desc->type()->FindValueByName(names[selected]);
      for (int i = 0; i < num_values; ++i) {
        delete names[i];
      }
      delete[] names;
      msg->GetReflection()->SetRepeatedEnum(msg, field_desc, k, selected_val);
    }
    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedEnumField(::google::protobuf::Message* msg,
                                             const ::google::protobuf::FieldDescriptor* field_desc) {
  auto* enum_desc = msg->GetReflection()->GetEnum(*msg, field_desc);
  int num_values = enum_desc->type()->value_count();
  char** names = new char *[static_cast<uint64_t>(num_values)];
  for (int i = 0; i < num_values; ++i) {
    names[i] = strdup(enum_desc->type()->value(i)->name().c_str());
  }

  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      auto selected_val = enum_desc->type()->FindValueByName(names[0]);
      msg->GetReflection()->SetEnum(msg, field_desc, selected_val);
    } else {
      return;
    }
  }

  int selected = enum_desc->index();
  ImGui::Combo(field_desc->name().c_str(), &selected, const_cast<const char **>(names), num_values);
  auto selected_val = enum_desc->type()->FindValueByName(names[selected]);
  for (int i = 0; i < num_values; ++i) {
    delete names[i];
  }
  delete[] names;
  msg->GetReflection()->SetEnum(msg, field_desc, selected_val);
  RemoveSimpleField(msg, field_desc, field_desc->name());
}

void ProtobufEditor::SetEnumField(::google::protobuf::Message* msg,
                                  const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedEnumField(msg, field_desc);
  } else {
    SetNonRepeatedEnumField(msg, field_desc);
  }
}

void ProtobufEditor::InputText(const std::string& name, std::string* str) {
  ImGui::InputText(name.c_str(), str);
  ImGui::SameLine();
  if (ImGui::Button(("Copy " + name).c_str())) {
    clip::set_text(*str);
  }
  ImGui::SameLine();
  if (ImGui::Button(("Paste " + name).c_str())) {
    clip::get_text(*str);
  }
}

static bool validate_uint(const std::string& str, uint32_t* val) {
  if (str.empty() || str.size() > 10) {
    return false;
  }
  for (const auto& c : str) {
    if (!isdigit(c)) {
      return false;
    }
  }
  uint64_t temp = static_cast<uint64_t>(stoll(str));
  if (temp > UINT32_MAX) {
    return false;
  }
  *val = static_cast<uint32_t>(temp);
  return true;
}

static bool validate_double(const std::string& str, double* val) {
  try {
    *val = std::stod(str);
    return true;
  } catch (std::invalid_argument e) {
    return false;
  } catch (std::out_of_range e2) {
    return false;
  }
}

static bool validate_float(const std::string& str, float* val) {
  try {
    *val = std::stof(str);
    return true;
  } catch (std::invalid_argument e) {
    return false;
  } catch (std::out_of_range e2) {
    return false;
  }
}

void ProtobufEditor::SetRepeatedUintFieldInner(::google::protobuf::Message* msg,
                                               const ::google::protobuf::FieldDescriptor* field_desc,
                                               const std::string& name, int k, int size, uint32_t val,
                                               const std::string& val_str, bool* should_break) {
  if (validate_uint(val_str, &val)) {
    ImGui::SameLine();

    if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
      *should_break = true;
      return;
    }

    msg->GetReflection()->SetRepeatedUInt32(msg, field_desc, k, val);
  } else {
    ImGui::Text("%s is not a valid uint32_t", val_str.c_str());
  }
}

void ProtobufEditor::SetRepeatedUintField(::google::protobuf::Message* msg,
                                          const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddUInt32(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }

  ImGui::SameLine();
  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());

  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      uint32_t val = msg->GetReflection()->GetRepeatedUInt32(*msg, field_desc, k);

      std::string name = field_desc->name() + std::to_string(k);
      std::string val_str = std::to_string(val);
      InputText(name, &val_str);
      bool removed = RemoveSimpleField(msg, field_desc, field_desc->name());
      if (!removed) {
        bool should_break = false;
        SetRepeatedUintFieldInner(msg, field_desc, name, k, size, val, val_str, &should_break);
        if (should_break) {
          break;
        }
      }
    }
    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedUintField(::google::protobuf::Message* msg,
                                             const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetUInt32(msg, field_desc, 0);
    } else {
      return;
    }
  }

  uint32_t val = msg->GetReflection()->GetUInt32(*msg, field_desc);

  std::string val_str = std::to_string(val);
  InputText(field_desc->name(), &val_str);
  bool removed = RemoveSimpleField(msg, field_desc, field_desc->name());
  if (!removed) {
    if (validate_uint(val_str, &val)) {
      msg->GetReflection()->SetUInt32(msg, field_desc, val);
    } else {
      ImGui::Text("%s is not a valid uint32_t", val_str.c_str());
    }
  }
}

void ProtobufEditor::SetUintField(::google::protobuf::Message* msg,
                                  const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedUintField(msg, field_desc);
  } else {
    SetNonRepeatedUintField(msg, field_desc);
  }
}

void ProtobufEditor::SetRepeatedBoolField(::google::protobuf::Message* msg,
                                          const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddBool(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();
  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());

  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      bool val = msg->GetReflection()->GetRepeatedBool(*msg, field_desc, k);

      std::string name = field_desc->name() + std::to_string(k);
      ImGui::Checkbox(name.c_str(), &val);
      ImGui::SameLine();

      if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
        break;
      }

      msg->GetReflection()->SetRepeatedBool(msg, field_desc, k, val);
    }
    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedBoolField(::google::protobuf::Message* msg,
                                             const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetBool(msg, field_desc, false);
    } else {
      return;
    }
  }

  bool val = msg->GetReflection()->GetBool(*msg, field_desc);
  ImGui::Checkbox(field_desc->name().c_str(), &val);

  msg->GetReflection()->SetBool(msg, field_desc, val);
  RemoveSimpleField(msg, field_desc, field_desc->name());
}

void ProtobufEditor::SetBoolField(::google::protobuf::Message* msg,
                                  const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedBoolField(msg, field_desc);
  } else {
    SetNonRepeatedBoolField(msg, field_desc);
  }
}

void ProtobufEditor::AllValsAddRemove(::google::protobuf::Message* msg,
                                      const ::google::protobuf::FieldDescriptor* field_desc, std::string* all_vals,
                                      std::vector<std::string>* all_vals_vec, int size) {
  InputText((field_desc->name() + "-all").c_str(), all_vals);
  split_by_multiple_delimiters(",", *all_vals, all_vals_vec);
  int diff = static_cast<int>(all_vals_vec->size()) - size;
  if (diff > 0) {
    for (int m = 0; m < diff; ++m) {
      NewField(msg, field_desc);
    }
  } else if (diff < 0) {
    for (int m = 0; m < -diff; ++m) {
      msg->GetReflection()->RemoveLast(msg, field_desc);
    }
  }
}

void ProtobufEditor::AllRepeatedFloatVals(::google::protobuf::Message* msg,
                                          const ::google::protobuf::FieldDescriptor* field_desc) {
  std::string all_vals;
  int size = msg->GetReflection()->FieldSize(*msg, field_desc);
  for (int k = 0; k < size; ++k) {
    float val = msg->GetReflection()->GetRepeatedFloat(*msg, field_desc, k);
    std::string name = field_desc->name() + std::to_string(k);

    std::string val_str = std::to_string(val);
    all_vals += val_str;
    if (k < size - 1) {
      all_vals += ",";
    }
  }

  std::vector<std::string> all_vals_vec;
  AllValsAddRemove(msg, field_desc, &all_vals, &all_vals_vec, size);

  int m = 0;
  for (const auto& val_s : all_vals_vec) {
    float flt;
    if (!validate_float(val_s, &flt)) {
      ImGui::Text("%s is not a valid float", val_s.c_str());
      continue;
    }
    msg->GetReflection()->SetRepeatedFloat(msg, field_desc, m, flt);
    ++m;
  }
}

void ProtobufEditor::SetRepeatedFloatField(::google::protobuf::Message* msg,
                                           const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddFloat(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();

  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());
  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      float val = msg->GetReflection()->GetRepeatedFloat(*msg, field_desc, k);
      std::string name = field_desc->name() + std::to_string(k);

      std::string val_str = std::to_string(val);
      InputText(name, &val_str);
      if (validate_float(val_str, &val)) {
        ImGui::SameLine();
        if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
          break;
        }

        msg->GetReflection()->SetRepeatedFloat(msg, field_desc, k, val);
      } else {
        ImGui::Text("%s is not a valid float", val_str.c_str());
      }
    }

    AllRepeatedFloatVals(msg, field_desc);

    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedFloatField(::google::protobuf::Message* msg,
                                              const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetFloat(msg, field_desc, 0);
    } else {
      return;
    }
  }

  float val = msg->GetReflection()->GetFloat(*msg, field_desc);

  std::string val_str = std::to_string(val);
  InputText(field_desc->name(), &val_str);
  bool removed = RemoveSimpleField(msg, field_desc, field_desc->name());
  if (!removed) {
    if (validate_float(val_str, &val)) {
      msg->GetReflection()->SetFloat(msg, field_desc, val);
    } else {
      ImGui::Text("%s is not a valid float", val_str.c_str());
    }
  }
}

void ProtobufEditor::SetFloatField(::google::protobuf::Message* msg,
                                   const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedFloatField(msg, field_desc);
  } else {
    SetNonRepeatedFloatField(msg, field_desc);
  }
}

void ProtobufEditor::SetRepeatedDoubleField(::google::protobuf::Message* msg,
                                            const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddDouble(msg, field_desc, 0);
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();

  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());
  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      double val = msg->GetReflection()->GetRepeatedDouble(*msg, field_desc, k);
      std::string name = field_desc->name() + std::to_string(k);

      std::string val_str = std::to_string(val);
      InputText(name, &val_str);
      if (validate_double(val_str, &val)) {
        ImGui::SameLine();

        if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
          break;
        }

        msg->GetReflection()->SetRepeatedDouble(msg, field_desc, k, val);
      } else {
        ImGui::Text("%s is not a valid double", val_str.c_str());
      }
    }
    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedDoubleField(::google::protobuf::Message* msg,
                                               const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetDouble(msg, field_desc, 0);
    } else {
      return;
    }
  }

  double val = msg->GetReflection()->GetDouble(*msg, field_desc);
  std::string val_str = std::to_string(val);
  InputText(field_desc->name(), &val_str);
  bool removed = RemoveSimpleField(msg, field_desc, field_desc->name());
  if (!removed) {
    if (validate_double(val_str, &val)) {
      msg->GetReflection()->SetDouble(msg, field_desc, val);
    } else {
      ImGui::Text("%s is not a valid double", val_str.c_str());
    }
  }
}

void ProtobufEditor::SetDoubleField(::google::protobuf::Message* msg,
                                    const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedDoubleField(msg, field_desc);
  } else {
    SetNonRepeatedDoubleField(msg, field_desc);
  }
}

void ProtobufEditor::AllRepeatedStringVals(::google::protobuf::Message* msg,
                                           const ::google::protobuf::FieldDescriptor* field_desc) {
  std::string all_vals;
  int size = msg->GetReflection()->FieldSize(*msg, field_desc);
  for (int k = 0; k < size; ++k) {
    std::string val_str = msg->GetReflection()->GetRepeatedString(*msg, field_desc, k);
    std::string name = field_desc->name() + std::to_string(k);

    all_vals += val_str;
    if (k < size - 1) {
      all_vals += ",";
    }
  }

  std::vector<std::string> all_vals_vec;
  AllValsAddRemove(msg, field_desc, &all_vals, &all_vals_vec, size);

  int m = 0;
  for (const auto& val_s : all_vals_vec) {
    msg->GetReflection()->SetRepeatedString(msg, field_desc, m, val_s);
    ++m;
  }
}

void ProtobufEditor::SetRepeatedStringField(::google::protobuf::Message* msg,
                                            const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    msg->GetReflection()->AddString(msg, field_desc, "");
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();

  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());
  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      std::string val = msg->GetReflection()->GetRepeatedString(*msg, field_desc, k);
      std::string name = field_desc->name() + std::to_string(k);
      InputText(name, &val);

      ImGui::SameLine();

      if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
        break;
      }

      msg->GetReflection()->SetRepeatedString(msg, field_desc, k, val);
    }

    AllRepeatedStringVals(msg, field_desc);

    ImGui::TreePop();
  }
}

void ProtobufEditor::SetNonRepeatedStringField(::google::protobuf::Message* msg,
                                               const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetString(msg, field_desc, "");
    } else {
      return;
    }
  }

  std::string val = msg->GetReflection()->GetString(*msg, field_desc);
  InputText(field_desc->name(), &val);

  msg->GetReflection()->SetString(msg, field_desc, val);
  RemoveSimpleField(msg, field_desc, field_desc->name());
}

void ProtobufEditor::SetStringField(::google::protobuf::Message* msg,
                                    const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    SetRepeatedStringField(msg, field_desc);
  } else {
    SetNonRepeatedStringField(msg, field_desc);
  }
}

static void browse(std::string* out) {
  if (ImGui::Button("Browse")) {
    std::string path = exec("zenity --file-selection --title=\"Select a file\"");
    if (!path.empty()) {
      *out = path.substr(0, path.size() - 1);
    }
  }
}

static std::string hex_str(const std::string& in) {
  std::string hexstr;
  int i = 0;
  for (const auto& c : in) {
    char temp[3] = {0};
    snprintf(temp, sizeof(temp), "%02x", static_cast<uint8_t>(c));
    hexstr += std::string(temp);
    ++i;
    if (i % 64 == 0) {
      hexstr += std::string("\n");
    }
  }

  return hexstr;
}

void ProtobufEditor::SetNonRepeatedBytesField(::google::protobuf::Message* msg,
                                              const ::google::protobuf::FieldDescriptor* field_desc) {
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      msg->GetReflection()->SetString(msg, field_desc, "");
    } else {
      return;
    }
  }
  std::string s = msg->GetReflection()->GetString(*msg, field_desc);

  std::string hex = hex_str(s);

  ImGui::InputTextMultiline(field_desc->name().c_str(), const_cast<char*>(hex.c_str()), ImGuiInputTextFlags_ReadOnly);
  bool removed = RemoveSimpleField(msg, field_desc, field_desc->name());
  if (!removed) {
    static std::string binary_file_path;
    browse(&binary_file_path);
    static bool cant_embed = false;
    if (ImGui::Button("Embed")) {
      if (!regular_file_exists(binary_file_path)) {
        cant_embed = true;
      } else {
        std::ifstream fin(binary_file_path, std::ios::binary);
        std::ostringstream ostrm;
        ostrm << fin.rdbuf();
        std::string data(ostrm.str());
        msg->GetReflection()->SetString(msg, field_desc, data);
        cant_embed = false;
      }
    }
    if (cant_embed) {
      ImGui::Text("can't embed");
    }
  }
}

void ProtobufEditor::SetBytesField(::google::protobuf::Message* msg,
                                   const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    ImGui::Text("repeated bytes is not supported");

  } else {
    SetNonRepeatedBytesField(msg, field_desc);
  }
}

bool ProtobufEditor::SetNonRepeatedMessage(::google::protobuf::Message* msg,
                                           const ::google::protobuf::FieldDescriptor* field_desc) {
  bool tree_selected = false;
  if (is_not_set(msg, field_desc)) {
    if (ImGui::Button(("create " + field_desc->name()).c_str())) {
      tree_selected = true;
    } else {
      return true;
    }
  }
  auto* field_msg = msg->GetReflection()->MutableMessage(msg, field_desc);
  auto* field_desc2 = field_msg->GetDescriptor();

  std::string name = field_desc->name();
  if (tree_selected) {
    ImGui::SetNextItemOpen(true);
  }
  bool temp_tree_selected = ImGui::TreeNode(name.c_str());
  if (!tree_selected) {
    tree_selected = temp_tree_selected;
  }
  ImGui::SameLine();

  if (tree_selected) {
    if (AddRemoveField(msg, field_desc, name)) {
      ImGui::TreePop();
    } else {
      if (!Tree(field_msg)) {
        return false;
      }
      ImGui::TreePop();
    }
  } else {
    AddRemoveField(msg, field_desc, name);
  }

  return true;
}

bool ProtobufEditor::IsSet(const ::google::protobuf::Message& msg,
                           const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    int size = msg.GetReflection()->FieldSize(msg, field_desc);
    return size;
  }
  return msg.GetReflection()->HasField(msg, field_desc);
}

bool ProtobufEditor::SelectRepeatedMessage(::google::protobuf::Message* msg,
                                           const ::google::protobuf::FieldDescriptor* field_desc) {
  auto* field_msg = msg->GetReflection()->AddMessage(msg, field_desc);
  auto* desc = field_msg->GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc2 = desc->field(i);
    if (!field_desc2->is_required()) {
      continue;
    }
    if (!IsSet(*field_msg, field_desc2)) {
      if (!NewField(field_msg, field_desc2)) {
        return false;
      }
    }
  }

  return true;
}

bool ProtobufEditor::SetRepeatedMessage(::google::protobuf::Message* msg,
                                        const ::google::protobuf::FieldDescriptor* field_desc) {
  if (ImGui::Button(("+ " + field_desc->name()).c_str())) {
    if (!SelectRepeatedMessage(msg, field_desc)) {
      return false;
    }
    ImGui::SetNextItemOpen(true);
  }
  ImGui::SameLine();

  bool tree_selected = ImGui::TreeNode(field_desc->name().c_str());
  if (tree_selected) {
    int size = msg->GetReflection()->FieldSize(*msg, field_desc);
    for (int k = 0; k < size; ++k) {
      auto* field_msg = msg->GetReflection()->MutableRepeatedMessage(msg, field_desc, k);
      std::string name = field_desc->name() + std::to_string(k);
      bool tree_selected2 = ImGui::TreeNode(name.c_str());
      ImGui::SameLine();
      if (tree_selected2) {
        if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
          ImGui::TreePop();
          break;
        }

        if (!Tree(field_msg)) {
          return false;
        }
        ImGui::TreePop();
      } else {
        if (AddRemoveRepeatedField(msg, field_desc, k, size, name)) {
          break;
        }
      }
    }
    ImGui::TreePop();
  }
  return true;
}

bool ProtobufEditor::SetMessageField(::google::protobuf::Message* msg,
                                     const ::google::protobuf::FieldDescriptor* field_desc) {
  if (field_desc->is_repeated()) {
    if (!SetRepeatedMessage(msg, field_desc)) {
      return false;
    }
  } else {
    if (!SetNonRepeatedMessage(msg, field_desc)) {
      return false;
    }
  }
  return true;
}

bool ProtobufEditor::SetFields(::google::protobuf::Message* msg,
                               const ::google::protobuf::FieldDescriptor* field_desc) {
  switch (field_desc->type()) {
    case ::google::protobuf::FieldDescriptor::TYPE_UINT32: {
      SetUintField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_SINT32:
      /* FALLTHROUGH */
    case ::google::protobuf::FieldDescriptor::TYPE_INT32: {
      SetIntField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_FLOAT: {
      SetFloatField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_DOUBLE: {
      SetDoubleField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_BOOL: {
      SetBoolField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_ENUM: {
      SetEnumField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_STRING: {
      SetStringField(msg, field_desc);
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_MESSAGE: {
      if (!SetMessageField(msg, field_desc)) {
        return false;
      }
      break;
    }
    case ::google::protobuf::FieldDescriptor::TYPE_BYTES: {
      SetBytesField(msg, field_desc);
      break;
    }
    default: {
      PBE_LOG_ERROR("wrong type %d\n", static_cast<int>(field_desc->type()));
      return false;
    }
  }

  return true;
}

bool ProtobufEditor::Tree(::google::protobuf::Message* msg) {
  auto* desc = msg->GetDescriptor();

  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc = desc->field(i);
    if (!SetFields(msg, field_desc)) {
      return false;
    }
  }
  return true;
}

void ProtobufEditor::WantToClose(bool* tried_to_load) {
  static bool want_to_close = false;
  if (ImGui::Button("Close")) {
    want_to_close = true;
  }
  if (want_to_close) {
    ImGui::SameLine();
    ImGui::Text("are you sure you want to close?");
    ImGui::SameLine();
    if (ImGui::Button("Yes")) {
      // pbe_ = pbe::File();

      *tried_to_load = false;
      want_to_close = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("No")) {
      want_to_close = false;
    }
  }
}

void ProtobufEditor::Save(bool* cant_save, std::string* error_str, const std::string& path) {
  try {
    size_t size = the_record_.ByteSizeLong();
    char* arr = new char[size];
    the_record_.SerializeToArray(arr, size);

    std::ofstream output(path, std::ios::out | std::ios::trunc | std::ios::binary);
    output.write(arr, static_cast<std::streamsize>(size));
    delete[] arr;
    output.close();

    *cant_save = false;
    error_str->clear();
  } catch (std::exception e) {
    *error_str = e.what();
    *cant_save = true;
  }
}

void ProtobufEditor::SaveFile(bool* cant_save, std::string* error_str) {
  if (ImGui::Button("Save")) {
    Save(cant_save, error_str, file_path_);
  }
  if (ImGui::Button("Save As")) {
    std::string cmd =
        "zenity --file-selection --save --title=\"Select a path to save the file\" --filename=" + file_path_;
    std::string path = exec(cmd.c_str());
    if (!path.empty()) {
      path = path.substr(0, path.size() - 1);
      Save(cant_save, error_str, path);
      file_path_ = path;
    }
  }
}

bool ProtobufEditor::SelectFieldsToAdd(::google::protobuf::Message* msg) {
  field_waiting_to_be_added_ = msg;
  auto* desc = msg->GetDescriptor();
  for (int i = 0; i < desc->field_count(); ++i) {
    auto* field_desc2 = desc->field(i);
    if (field_desc2->is_required()) {
      if (!IsSet(*msg, field_desc2)) {
        if (!NewField(msg, field_desc2)) {
          return false;
        }
      }
    }
  }

  return true;
}

void ProtobufEditor::MainScreen() {
  static bool cant_save = false;
  static bool cant_load = false;
  static bool tried_to_load = false;
  static std::string error_str;

  ImGui::Begin("main");
  ImGui::SetWindowFontScale(1.8f);

  browse(&file_path_);

  ImGui::SameLine();
  InputText("file path", &file_path_);

  if (ImGui::Button("Load")) {
    if (!read_file(file_path_, &the_record_)) {
      error_str = "can't load file";
      cant_load = true;
    } else {
      cant_load = false;
    }
    tried_to_load = true;
    cant_save = false;
  }
  if (ImGui::Button("Create")) {
    tried_to_load = true;
    cant_load = false;
    cant_save = false;
  }
  if (cant_save || cant_load) {
    ImGui::TextWrapped("%s", error_str.c_str());
  }

  if (tried_to_load && !cant_load) {
    WantToClose(&tried_to_load);
    SaveFile(&cant_save, &error_str);

    bool tree_selected = ImGui::TreeNode(the_record_.GetDescriptor()->name().c_str());
    auto* msg = (::google::protobuf::Message*)&the_record_;
    if (tree_selected) {
      Tree(msg);

      ImGui::TreePop();
    }
  }
  ImGui::End();
}

void ProtobufEditor::OneIteration() {
  ImGuiWindow* window = g_im_gui_->CurrentWindow;
  window->ScrollbarY = true;
  window->ScrollbarX = true;
  MainScreen();
}

void ProtobufEditor::MainLoop() {
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window_)) {
    glfwPollEvents();

    // ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OneIteration();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window_, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w,
                 clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window_);
  }
}

void ProtobufEditor::Stop() {
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window_);
  glfwTerminate();
}
