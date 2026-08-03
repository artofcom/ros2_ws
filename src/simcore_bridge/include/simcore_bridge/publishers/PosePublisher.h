#pragma once

#include <memory>

#include "geometry_msgs/msg/pose_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "simcore/Pose2D.h"

namespace simcore_bridge
{

class PosePublisher
{
public:
    PosePublisher(
        rclcpp::Node* node,
        const std::string& topicName);

    void Publish(const simcore::Pose2D& pose);

private:
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr publisher_;
};

}