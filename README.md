# Protobuf-Editor
## GUI for editing and review protocol buffers (=protobuf) packets

currently support only proto2

### Installation
Tested on Ubuntu 20.04

1. install required packages:
```
sudo apt-get install g++ cmake libglfw3 libglfw3-dev
```
2. install protoc version 24.3
from [here](https://github.com/protocolbuffers/protobuf/releases):

search for
```
protoc-24.3-linux-x86_64.zip
```
(if you need to upgrade your protoc, see [this](https://stackoverflow.com/a/57776284/4357938))

3. Clone this repo.
4. Put your protobuf schema file (".proto" file) in `your_schema` dir inside repo:
   ```
   cp path/to/your/schema your_schema/schema.proto
   ```
5. In the `your_schema/schema.proto`:
   
   a. Change the root message to be called `MyRecord`

   b. Change the package to be `protobuf.editor`
   for example:
    ```
    syntax = "proto2";
    package protobuf.editor;

    message OtherRecord {
      int32 other_id = 1;
    }

    message MyRecord {
      string f1 = 1;
      OtherRecord f2 = 2;
    }

7. Generate C++ code from your schema:
   ```
   protoc --proto_path=your_schema/ schema.proto --cpp_out=your_schema/
   ```
   
8. In the root directory of the repo, do:
   ```
   mkdir build
   cd build
   cmake -DABSL_PROPAGATE_CXX_STD=y -Dprotobuf_BUILD_TESTS=OFF ..
   make -j 8
   ```
9. Run GUI:
   ```
   ./protobuf-editor/protobuf-editor
   ```
