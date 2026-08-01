#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "simcore/Simulation.h"
#include "simcore/World.h"
#include "simcore/Robot.h"
#include "simcore/Pose2D.h"
#include "simcore/Goal.h"
#include "geometry_msgs/msg/pose2_d.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "visualization_msgs/msg/marker.hpp"

using namespace std::chrono_literals;

class SimCoreNode : public rclcpp::Node
{
public:
    SimCoreNode()
        : Node("simcore_bridge")
    {
        simulation_.Initialize();
        posePublisher_ = create_publisher<geometry_msgs::msg::Pose2D>("robot_pose", 10);
        goalSubscriber_ = create_subscription<geometry_msgs::msg::Point>( "goal", 10,
                  std::bind(&SimCoreNode::OnGoalReceived,
                  this,
                  std::placeholders::_1));

        robotMarkerPublisher_ = create_publisher<visualization_msgs::msg::Marker>( "robot_marker", 10);
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
        simulation_.Update(0.1f);

        auto& world = simulation_.GetWorld();
        if (world.GetRobotCount() == 0)
            return;
        
        auto robot = world.GetRobot(0);
        auto pose = robot->GetPose();

        geometry_msgs::msg::Pose2D msg;
        msg.x = pose.x;
        msg.y = pose.y;
        msg.theta = pose.theta;
        posePublisher_->publish(msg);

        publishMarker(pose);
        publishGoalMarker();
    }
    
    void publishMarker(simcore::Pose2D& pose)
    {
        visualization_msgs::msg::Marker marker;

        marker.header.frame_id = "map";
        marker.header.stamp = now();
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

        robotMarkerPublisher_->publish(marker);
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

    void OnGoalReceived(const geometry_msgs::msg::Point::SharedPtr msg)
    {
        auto goal = std::make_shared<simcore::Goal>();

        goal->SetPosition(
            static_cast<float>(msg->x),
            static_cast<float>(msg->y));

        currentGoal_ = goal->GetPose();
        hasGoal_ = true;

        simulation_.GetWorld().SetGoal(goal);

        RCLCPP_INFO(
            get_logger(),
            "New Goal: (%.2f, %.2f)",
            msg->x,
            msg->y);
    }

    rclcpp::Publisher<geometry_msgs::msg::Pose2D>::SharedPtr posePublisher_;

private:
    simcore::Simulation simulation_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr goalSubscriber_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr robotMarkerPublisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr goalMarkerPublisher_;

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