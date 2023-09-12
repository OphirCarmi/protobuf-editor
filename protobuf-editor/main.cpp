#include "protobuf_editor.h"

int main() {
  ProtobufEditor editor = ProtobufEditor();

  editor.Init();
  editor.MainLoop();
  editor.Stop();

  return 0;
}
