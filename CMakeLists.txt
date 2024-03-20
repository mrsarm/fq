cmake_minimum_required(VERSION 3.24)
project(fq C)

include(FetchContent)
enable_testing()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out")

include_directories("${CMAKE_SOURCE_DIR}/src")

set(BASE_SOURCE_FILES
    src/freqlist.c
    src/fq.c
    src/util.c)

# Executable "fq"
add_executable(fq src/main.c
               ${BASE_SOURCE_FILES})

set(BASE_TEST_SOURCE_FILES
    test/util_t.c)

FetchContent_Declare(
        cheat_h
        URL      https://github.com/Tuplanolla/cheat/archive/refs/tags/1.0.4.tar.gz
        URL_HASH MD5=30c3edd18d1f6d96e2da0c6682f260b7
        DOWNLOAD_EXTRACT_TIMESTAMP true
)
FetchContent_MakeAvailable(cheat_h)

# Executable with unit tests "test_fq"
add_executable(test_fq test/test_fq.c
               ${BASE_TEST_SOURCE_FILES}
               ${BASE_SOURCE_FILES})
target_include_directories(test_fq PUBLIC "${cheat_h_SOURCE_DIR}")

# Install with `make install`
install(TARGETS fq
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)

# To trigger dependencies before `make test`
set_property(DIRECTORY APPEND
             PROPERTY TEST_INCLUDE_FILES
             "${CMAKE_CURRENT_BINARY_DIR}/BuildTestTarget.cmake")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/BuildTestTarget.cmake"
     "execute_process(COMMAND \"${CMAKE_COMMAND}\""
     " --build \"${CMAKE_BINARY_DIR}\""
     " --config \"\$ENV{CMAKE_CONFIG_TYPE}\")")

# Unit tests with `make test`
add_test(test_fq ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_fq)

# To clean everything: compiled binaries and *make* files
add_custom_target(clean-all
  make clean && rm -fR ${CMAKE_BINARY_DIR}/CMakeCache.txt
                       ${CMAKE_BINARY_DIR}/cmake_install.cmake
                       ${CMAKE_BINARY_DIR}/Makefile
                       ${CMAKE_BINARY_DIR}/CMakeFiles
                       ${CMAKE_BINARY_DIR}/cmake-build-debug
                       ${CMAKE_BINARY_DIR}/cmake-build-release
                       ${CMAKE_BINARY_DIR}/install_manifest.txt
                       ${CMAKE_BINARY_DIR}/Testing
                       ${CMAKE_BINARY_DIR}/_deps
             && rmdir  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
