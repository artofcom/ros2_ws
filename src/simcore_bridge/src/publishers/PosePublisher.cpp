#include "simcore_bridge/publishers/PosePublisher.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

namespace simcore_bridge
{

PosePublisher::PosePublisher(
    rclcpp::Node* node,
    const std::string& topicName)
{
    publisher_ =
         node->create_publisher<geometry_msgs::msg::PoseStamped>(
            topicName,
            10);
}

void PosePublisher::Publish(const simcore::Pose2D& pose)
{
    geometry_msgs::msg::PoseStamped msg;

    msg.header.stamp = rclcpp::Clock().now();
    msg.header.frame_id = "map";

    msg.pose.position.x = pose.x;
    msg.pose.position.y = pose.y;
    msg.pose.position.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, pose.theta);

    msg.pose.orientation = tf2::toMsg(q);

    publisher_->publish(msg);
}

}