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
find_package(tf2_ros REQUIRED)
find_package(tf2 REQUIRED)

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

add_executable(
    simcore_bridge
    src/main.cpp
    src/publishers/PosePublisher.cpp
    src/publishers/MarkerPublisher.cpp
    src/publishers/TfPublisher.cpp
)

target_include_directories(simcore_bridge
    PRIVATE
        include
)

target_link_libraries(simcore_bridge
    simcore
)

ament_target_dependencies(
    simcore_bridge
    rclcpp
    geometry_msgs
    visualization_msgs
    tf2
    tf2_ros
)

install(TARGETS
    simcore_bridge
    DESTINATION lib/${PROJECT_NAME}
)

ament_package()