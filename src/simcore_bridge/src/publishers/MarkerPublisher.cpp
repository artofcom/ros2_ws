#include "simcore_bridge/publishers/MarkerPublisher.h"
#include "simcore/Obstacle.h"

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

    obstaclePublisher_ =
        node_->create_publisher<
            visualization_msgs::msg::MarkerArray>(
            "obstacle_markers",
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

void MarkerPublisher::PublishObstacles(
    const simcore::Simulation& simulation)
{
    visualization_msgs::msg::MarkerArray markers;

    for(size_t i = 0; i < simulation.GetObstacleCount(); ++i)
    {
        auto obstacle = simulation.GetObstacle(i);

        visualization_msgs::msg::Marker marker;

        marker.header.frame_id = "map";
        marker.header.stamp = node_->now();

        marker.ns = "obstacle";
        marker.id = static_cast<int>(i);

        marker.type =
            visualization_msgs::msg::Marker::CYLINDER;

        marker.action =
            visualization_msgs::msg::Marker::ADD;

        const auto& pose = obstacle->GetPose();

        marker.pose.position.x = pose.x;
        marker.pose.position.y = pose.y;
        marker.pose.position.z = 0.0;

        marker.pose.orientation.w = 1.0;

        float diameter = obstacle->GetRadius() * 2.0f;

        marker.scale.x = diameter;
        marker.scale.y = diameter;
        marker.scale.z = 1.0;

        marker.color.r = 0.2f;
        marker.color.g = 0.2f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0f;

        markers.markers.push_back(marker);
    }

    obstaclePublisher_->publish(markers);
}

void MarkerPublisher::PublishPath(
    const simcore::Pose2D& start,
    const simcore::Pose2D& goal)
{
    visualization_msgs::msg::Marker marker;

    marker.header.frame_id = "map";
    marker.header.stamp = node_->now();

    marker.ns = "path";
    marker.id = 100;

    marker.type =
        visualization_msgs::msg::Marker::LINE_STRIP;

    marker.action =
        visualization_msgs::msg::Marker::ADD;

    marker.scale.x = 0.05;

    marker.color.r = 1.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0f;

    geometry_msgs::msg::Point p;

    p.x = start.x;
    p.y = start.y;
    p.z = 0.02;

    marker.points.push_back(p);

    p.x = goal.x;
    p.y = goal.y;

    marker.points.push_back(p);

    publisher_->publish(marker);
}
}