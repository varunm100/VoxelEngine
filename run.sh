#!bin/bash

cmake -H. -G Ninja -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
./VulkanTest
