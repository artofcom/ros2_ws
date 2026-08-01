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
        {
            return;
        }

        auto robot = world.GetRobot(0);

        auto pose = robot->GetPose();

        geometry_msgs::msg::Pose2D msg;

        msg.x = pose.x;
        msg.y = pose.y;
        msg.theta = pose.theta;

        posePublisher_->publish(msg);
    }
    
    void OnGoalReceived(const geometry_msgs::msg::Point::SharedPtr msg)
    {
        auto goal = std::make_shared<simcore::Goal>();

        goal->SetPosition(
            static_cast<float>(msg->x),
            static_cast<float>(msg->y));

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
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SimCoreNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}