cmake_minimum_required(VERSION 3.8)
project(simcore_bridge)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# --------------------------------------------------
# Dependencies
# --------------------------------------------------

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(visualization_msgs REQUIRED)

# --------------------------------------------------
# SimCore
# --------------------------------------------------

set(SIMCORE_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SIMCORE_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../SimCore
    ${CMAKE_BINARY_DIR}/simcore_build
)

# --------------------------------------------------
# Executable
# --------------------------------------------------

add_executable(simcore_bridge
    src/main.cpp
)

target_link_libraries(simcore_bridge
    simcore
)

ament_target_dependencies(
    simcore_bridge
    rclcpp
    geometry_msgs
    visualization_msgs
)

install(TARGETS
    simcore_bridge
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()