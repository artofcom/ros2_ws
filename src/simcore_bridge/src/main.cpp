#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "simcore/Simulation.h"

using namespace std::chrono_literals;

class SimCoreNode : public rclcpp::Node
{
public:
    SimCoreNode()
        : Node("simcore_bridge")
    {
        simulation_.Initialize();

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
    }

private:
    simcore::Simulation simulation_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<SimCoreNode>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}