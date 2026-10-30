
target_declare(host
    ${CMAKE_CURRENT_LIST_DIR}/include/*.h
    ${CMAKE_CURRENT_LIST_DIR}/src/*.c
)

# TODO: Move into target_declare_use
include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

platform_use(host)

features_declare(
    TARGET_MAX_LOG_LINE=512
)
