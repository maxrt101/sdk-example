cmake_minimum_required(VERSION 4.3)
set(PROJECT sdk2)

project(${PROJECT} C)

set(CMAKE_C_STANDARD 23)

include(sdk/sdk.cmake)

features_declare(
    VERBOSE=0
    WITH_DEFAULT_LOG_BACKEND=1
)

project_declare(${PROJECT}
    src/*.c
)

requires(
    util
    lds
    device
    tty
    shell
    log
    async
)

target_use(${TARGET})

platform_use(common)

project_finalize()
