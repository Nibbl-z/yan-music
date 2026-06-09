cmake_minimum_required(VERSION 3.16)
project(yan-music)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(nlohmann_json 3.11.3 REQUIRED)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

include(${wxWidgets_USE_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)