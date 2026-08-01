#include "simcore_bridge/publishers/TfPublisher.h"

#include <tf2/LinearMath/Quaternion.h>

namespace simcore_bridge
{

TfPublisher::TfPublisher(rclcpp::Node* node)
    : node_(node)
{
    broadcaster_ =
        std::make_unique<tf2_ros::TransformBroadcaster>(*node_);
}

void TfPublisher::PublishRobot(const simcore::Pose2D& pose)
{
    geometry_msgs::msg::TransformStamped transform;

    transform.header.stamp = node_->now();
    transform.header.frame_id = "map";
    transform.child_frame_id = "base_link";

    transform.transform.translation.x = pose.x;
    transform.transform.translation.y = pose.y;
    transform.transform.translation.z = 0.0;

    tf2::Quaternion q;
    q.setRPY(0.0, 0.0, pose.theta);

    transform.transform.rotation.x = q.x();
    transform.transform.rotation.y = q.y();
    transform.transform.rotation.z = q.z();
    transform.transform.rotation.w = q.w();

    broadcaster_->sendTransform(transform);
}

}