#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "simcore/Simulation.h"
#include "simcore/Pose2D.h"
#include "geometry_msgs/msg/pose2_d.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include "simcore_bridge/publishers/PosePublisher.h"
#include "simcore_bridge/publishers/MarkerPublisher.h"
#include "simcore_bridge/publishers/TfPublisher.h"

using namespace std::chrono_literals;

class SimCoreNode : public rclcpp::Node
{
    static constexpr float kTimeStep = 0.1f;
public:
    SimCoreNode()
        : Node("simcore_bridge")
        , posePublisher_(this, "robot_pose")
        , markerPublisher_(this, "robot_marker")
        , tfPublisher_(this)
    {
        simulation_.Initialize();
        goalSubscriber_ = create_subscription<geometry_msgs::msg::PoseStamped>( "/goal_pose", 10,
                  std::bind(&SimCoreNode::OnGoalReceived,
                  this,
                  std::placeholders::_1));

        goalMarkerPublisher_ = create_publisher<visualization_msgs::msg::Marker>( "goal_marker", 10);
        
        timer_ = create_wall_timer(
            100ms,
            std::bind(&SimCoreNode::OnTimer, this));
    }

    ~SimCoreNode()
    {
        simulation_.Shutdown();
    }

private:
    void OnTimer()
    {
        simulation_.Update(kTimeStep);

        const auto& pose = simulation_.GetRobotPose();

        posePublisher_.Publish(pose);
        markerPublisher_.PublishRobot(pose);
        tfPublisher_.PublishRobot(pose);
        
        publishGoalMarker();
    }
    
    void publishGoalMarker()
    {
        if (!hasGoal_)
            return;

        visualization_msgs::msg::Marker marker;

        marker.header.frame_id = "map";
        marker.header.stamp = now();

        marker.ns = "goal";
        marker.id = 0;

        marker.type = visualization_msgs::msg::Marker::CYLINDER;
        marker.action = visualization_msgs::msg::Marker::ADD;

        marker.pose.position.x = currentGoal_.x;
        marker.pose.position.y = currentGoal_.y;
        marker.pose.position.z = 0.0;

        marker.pose.orientation.w = 1.0;

        marker.scale.x = 0.4;
        marker.scale.y = 0.4;
        marker.scale.z = 0.1;

        marker.color.a = 1.0;
        marker.color.r = 1.0;
        marker.color.g = 0.1;
        marker.color.b = 0.1;

        goalMarkerPublisher_->publish(marker);
}

    void OnGoalReceived(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
    {
        currentGoal_ = simulation_.GetGoalPose();
        hasGoal_ = true;

        simulation_.SetGoal(
        static_cast<float>(msg->pose.position.x),
        static_cast<float>(msg->pose.position.y));

        RCLCPP_INFO(
            get_logger(),
            "New Goal: (%.2f, %.2f)",
            msg->pose.position.x,
            msg->pose.position.y);
    }

private:
    
    simcore_bridge::PosePublisher posePublisher_;

    simcore::Simulation simulation_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr goalSubscriber_;
    
    simcore_bridge::MarkerPublisher markerPublisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr goalMarkerPublisher_;
    
    simcore_bridge::TfPublisher tfPublisher_;

    simcore::Pose2D currentGoal_;
    bool hasGoal_ = false;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SimCoreNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}