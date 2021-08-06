cmake_minimum_required(VERSION 3.1)
project(fq C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")

include_directories("${CMAKE_SOURCE_DIR}/src")

set(BASE_SOURCE_FILES
    src/freqlist.c
    src/fq.c
    src/util.c)

add_executable(fq src/main.c ${BASE_SOURCE_FILES})

set(TEST_SOURCE_FILES
    test/util_t.c
    test/test_fq.c)

add_executable(test_fq ${TEST_SOURCE_FILES} ${BASE_SOURCE_FILES})

install(TARGETS fq
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

add_custom_target(clean-all
  make clean && rm -fR ${CMAKE_BINARY_DIR}/CMakeCache.txt
                       ${CMAKE_BINARY_DIR}/cmake_install.cmake
                       ${CMAKE_BINARY_DIR}/Makefile
                       ${CMAKE_BINARY_DIR}/CMakeFiles
                       ${CMAKE_BINARY_DIR}/cmake-build-debug
                       ${CMAKE_BINARY_DIR}/install_manifest.txt
                       ${CMAKE_BINARY_DIR}/Testing
             && rmdir  ${CMAKE_BINARY_DIR}/out
)
