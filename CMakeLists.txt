cmake_minimum_required(VERSION 3.0.0)
project(msxtools VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Boost REQUIRED COMPONENTS program_options)

find_library(FINALLIB
            NAMES final
            PATHS /usr/lib /usr/local/lib
            )
find_library(NCURSESLIB
            NAMES ncurses
            PATHS /usr/lib /usr/local/lib
            )

add_executable(msxtools main.cpp msx_tools.cpp hex_editor.cpp)
target_link_libraries(msxtools ${FINALLIB} ${NCURSESLIB} ${Boost_LIBRARIES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
