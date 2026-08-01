#pragma once

#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"

#include "simcore/Pose2D.h"

namespace simcore_bridge
{

class TfPublisher
{
public:

    explicit TfPublisher(rclcpp::Node* node);

    void PublishRobot(const simcore::Pose2D& pose);

private:

    rclcpp::Node* node_;

    std::unique_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
};

}