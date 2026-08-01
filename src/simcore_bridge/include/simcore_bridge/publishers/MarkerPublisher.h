#pragma once

#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"

#include "simcore/Pose2D.h"

namespace simcore_bridge
{

class MarkerPublisher
{
public:

    MarkerPublisher(
        rclcpp::Node* node,
        const std::string& topicName);

    void PublishRobot(const simcore::Pose2D& pose);

    void PublishGoal(const simcore::Pose2D& pose);
private:

    rclcpp::Node* node_;

    rclcpp::Publisher<
        visualization_msgs::msg::Marker>::SharedPtr publisher_;
};

}