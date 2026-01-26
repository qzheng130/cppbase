#!/bin/sh

# set -x 是一个单字符选项，用于打开 xtrace 模式，显示命令的执行过程
set -x

SOURCE_DIR=`pwd`

# :- 默认值
# 对于-号，例如表达式var=${str-expr}或var=${str:-expr}，当变量str未定义时var为expr，变量str已定义时var为str
# 对于等号，例如表达式var=${str=expr}或var=${str:=expr}，当变量str未定义时，str及var都为expr；变量str已定义时，str的值不变，var为str

BUILD_DIR=${BUILD_DIR:-./build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-../${BUILD_TYPE}-install}
CXX=${CXX:-g++}

# 省略了compile_commands.json，它是软链接，指向源文件
# ln -sf $BUILD_DIR/$BUILD_TYPE/compile_commands.json compile_commands.json
ln -sf $BUILD_DIR/$BUILD_TYPE/compile_commands.json

# -DCMAKE_EXPORT_COMPILE_COMMANDS=ON：生成compile_commands.json，包含所有编译单元执行的命令
# $*是用单个字符串表示shell脚本参数

mkdir -p $BUILD_DIR/$BUILD_TYPE \
  && cd $BUILD_DIR/$BUILD_TYPE \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $* 

# Use the following command to run all the unit tests
# at the dir $BUILD_DIR/$BUILD_TYPE :
# CTEST_OUTPUT_ON_FAILURE=TRUE make test

# cd $SOURCE_DIR && doxygen

# BUILD_TYPE=debug ./build.sh -j4 编译debug版本
# BUILD_TYPE=debug ./build.sh -j4 install
# ./build.sh -j4 默认编译release版本