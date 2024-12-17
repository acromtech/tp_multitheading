cmake_minimum_required(VERSION 3.10)
project(low_level)

# Ajouter C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ajouter les bibliothèques
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# CPR depuis le sous-module ou le système
include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 081771c80dc7ed88fb49931a1dbb888b5b1bf19c
)
FetchContent_MakeAvailable(cpr)

# Ajouter nlohmann/json
include(FetchContent)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# Fichiers sources
add_executable(low_level client.cpp)

# Lier les bibliothèques
target_link_libraries(low_level PRIVATE cpr::cpr nlohmann_json::nlohmann_json)

