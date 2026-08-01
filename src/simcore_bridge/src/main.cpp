#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "simcore/Simulation.h"
#include "simcore/Pose2D.h"
#include "geometry_msgs/msg/pose_stamped.hpp"
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
        
        if (simulation_.HasGoal())
        {
            const auto& goal = simulation_.GetGoalPose();

            markerPublisher_.PublishGoal(goal);

            markerPublisher_.PublishPath(
                pose,
                goal);
        }
        markerPublisher_.PublishObstacles(simulation_);

        tfPublisher_.PublishRobot(pose);
    }
    
    void OnGoalReceived(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
    {
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
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SimCoreNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}