sudo apt-get install libglfw3
sudo apt-get install libglfw3-dev
cmake -DABSL_PROPAGATE_CXX_STD=y ..
protoc --proto_path=../ schema.proto --cpp_out=..