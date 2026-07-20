cmake_minimum_required(VERSION 4.3)
project(EvoSpace)

include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

add_executable(app src/simulation.cpp)

target_link_libraries(app PRIVATE  nlohmann_json::nlohmann_json)
