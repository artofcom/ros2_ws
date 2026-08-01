# SimCore ROS2 Bridge

ROS2 bridge for **SimCore**, a lightweight 2D robot simulation library.

This package connects the standalone SimCore engine with the ROS2 ecosystem, providing visualization in RViz and preparing the simulation for NVIDIA Isaac Sim integration.

---

## Features

- ✅ SimCore integration
- ✅ Robot pose publishing
- ✅ Goal subscription
- ✅ TF broadcasting
- ✅ Robot visualization
- ✅ Goal visualization
- ✅ Obstacle visualization (MarkerArray)
- ✅ Path visualization
- ✅ Collision detection
- ✅ RViz interactive goal selection

---

## Architecture

```
           +----------------------+
           |     RViz / Isaac     |
           +----------+-----------+
                      |
                  ROS2 Topics
                      |
           +----------v-----------+
           |   simcore_bridge     |
           +----------+-----------+
                      |
                 SimCore API
                      |
           +----------v-----------+
           |      SimCore         |
           +----------------------+
```

---

## Current Topics

| Topic | Type | Description |
|--------|------|-------------|
| `/goal` | geometry_msgs/PoseStamped | Navigation goal |
| `/robot_pose` | geometry_msgs/Pose2D | Robot pose |
| `/robot_marker` | visualization_msgs/Marker | Robot visualization |
| `/obstacle_markers` | visualization_msgs/MarkerArray | Obstacles |
| `/tf` | tf2 | Robot transform |

---

## Build

```bash
cd ~/workspace/ros2_ws

colcon build

source install/setup.bash
```

---

## Run

```bash
ros2 run simcore_bridge simcore_bridge
```

---

## RViz

Add the following displays:

- TF
- Marker (`/robot_marker`)
- MarkerArray (`/obstacle_markers`)
- Path (`/robot_marker`)
- Goal Tool (Publish Point)

Click anywhere on the map to send a navigation goal.

---

## Project Status

Current implementation:

- Robot kinematics
- Goal controller
- Collision detection
- Obstacle visualization
- Path visualization
- ROS2 bridge

Planned:

- NVIDIA Isaac Sim integration
- A* Path Planning
- Occupancy Grid
- Sensor Simulation
- Multi-Robot Support

---

## Repository Structure

```
ros2_ws
└── src
    └── simcore_bridge
        ├── include
        ├── src
        │   ├── publishers
        │   ├── tf
        │   └── main.cpp
        └── CMakeLists.txt
```

---

## Related Project

This package depends on:

- **SimCore**
  - Standalone robotics simulation library
  - Motion
  - World
  - Goal Controller
  - Collision Detection

Future integration target:

- NVIDIA Isaac Sim