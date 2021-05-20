#!/bin/sh

if [ $# -gt 0 ]
then
  eval cppcheck --force --language=c++ --std=c++11 --enable=all -I../src/include/ "$@"
else
  eval cppcheck --force --language=c++ --std=c++11 --enable=all -I../src/include/ ../src/ ../examples/
fi

