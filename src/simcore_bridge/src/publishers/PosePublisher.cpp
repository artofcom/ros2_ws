#include "simcore_bridge/publishers/PosePublisher.h"

namespace simcore_bridge
{

PosePublisher::PosePublisher(
    rclcpp::Node* node,
    const std::string& topicName)
{
    publisher_ =
        node->create_publisher<geometry_msgs::msg::Pose2D>(
            topicName,
            10);
}

void PosePublisher::Publish(const simcore::Pose2D& pose)
{
    geometry_msgs::msg::Pose2D msg;

    msg.x = pose.x;
    msg.y = pose.y;
    msg.theta = pose.theta;

    publisher_->publish(msg);
}

}