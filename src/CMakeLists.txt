find_package(Boost REQUIRED COMPONENTS program_options)

find_library(FINALLIB
            NAMES final
            PATHS /usr/lib /usr/local/lib
            )

add_subdirectory(desktop)
add_subdirectory(hexeditor)
add_subdirectory(msx)

add_executable(msx-tools main.cpp)
target_link_libraries(msx-tools desktop hexeditor msx ${FINALLIB}  ${Boost_LIBRARIES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
