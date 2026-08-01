#include "simcore_bridge/publishers/MarkerPublisher.h"

namespace simcore_bridge
{

MarkerPublisher::MarkerPublisher(
    rclcpp::Node* node,
    const std::string& topicName)
    : node_(node)
{
    publisher_ =
        node_->create_publisher<
            visualization_msgs::msg::Marker>(
            topicName,
            10);
}

void MarkerPublisher::PublishRobot(
    const simcore::Pose2D& pose)
{
    visualization_msgs::msg::Marker marker;

    marker.header.frame_id = "map";
    marker.header.stamp = node_->now();

    marker.ns = "robot";
    marker.id = 0;

    marker.type = visualization_msgs::msg::Marker::SPHERE;
    marker.action = visualization_msgs::msg::Marker::ADD;

    marker.pose.position.x = pose.x;
    marker.pose.position.y = pose.y;
    marker.pose.position.z = 0.0;

    marker.pose.orientation.w = 1.0;

    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    marker.color.a = 1.0;
    marker.color.r = 0.2;
    marker.color.g = 0.8;
    marker.color.b = 0.2;

    publisher_->publish(marker);
}

void MarkerPublisher::PublishGoal(
    const simcore::Pose2D& pose)
{
    visualization_msgs::msg::Marker marker;

    marker.header.frame_id = "map";
    marker.header.stamp = node_->now();

    marker.ns = "goal";
    marker.id = 1;

    marker.type = visualization_msgs::msg::Marker::CYLINDER;
    marker.action = visualization_msgs::msg::Marker::ADD;

    marker.pose.position.x = pose.x;
    marker.pose.position.y = pose.y;
    marker.pose.position.z = 0.05;

    marker.pose.orientation.w = 1.0;

    marker.scale.x = 0.4;
    marker.scale.y = 0.4;
    marker.scale.z = 0.1;

    marker.color.a = 1.0;
    marker.color.r = 1.0;
    marker.color.g = 0.1;
    marker.color.b = 0.1;

    publisher_->publish(marker);
}
}