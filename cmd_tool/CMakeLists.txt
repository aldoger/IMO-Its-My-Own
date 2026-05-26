cmake_minimum_required(VERSION 3.23)

project(imo-cli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(JSONC REQUIRED json-c)

add_executable(imo-cli
    main.cpp
    send.cpp
)

target_include_directories(imo-cli
    PRIVATE
        "include/"
        ${JSONC_INCLUDE_DIRS}
)

target_link_libraries(imo-cli
    PRIVATE
        CURL::libcurl
        ${JSONC_LIBRARIES}
)

install(TARGETS imo-cli DESTINATION bin)