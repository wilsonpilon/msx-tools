cmake_minimum_required(VERSION 3.0.0)
project(msx-tools VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Boost REQUIRED COMPONENTS program_options)

find_library(FINALLIB
            NAMES final
            PATHS /usr/lib /usr/local/lib
            )

add_executable(msx-tools main.cpp msx_tools.cpp hex_editor.cpp desktop.cpp desktop.h msx.cpp msx.h)
target_link_libraries(msx-tools ${FINALLIB}  ${Boost_LIBRARIES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
