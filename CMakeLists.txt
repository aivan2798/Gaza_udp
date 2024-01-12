cmake_minimum_required(VERSION 3.0.0)
project(Gaza_udp VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(Gaza_udp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)