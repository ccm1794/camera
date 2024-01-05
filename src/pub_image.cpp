#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "std_msgs/msg/header.hpp"
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat frame1, frame2, image1, image2;
void on_mouse(int event, int x, int y, int flags, void *);

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = rclcpp::Node::make_shared("image_publisher");

  auto publisher1 = node->create_publisher<sensor_msgs::msg::Image>("video1", 10);
  // sensor_msgs::msg::Image::SharedPtr msg1;

  VideoCapture cap1(0);
  cap1.set(CAP_PROP_FRAME_WIDTH, 640);
  cap1.set(CAP_PROP_FRAME_HEIGHT, 480);

  Mat merged;

  rclcpp::WallRate loop_rate(20.0);

  while(rclcpp::ok())
  {
    cap1 >> frame1;

    // sensor_msgs::msg::Image::SharedPtr msg1;
    // msg1 = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame1).toImageMsg();
    // publisher1->publish(*msg1.get()); 

    sensor_msgs::msg::Image::UniquePtr image_msg = std::make_unique<sensor_msgs::msg::Image>();
    image_msg->height = frame1.rows;
    image_msg->width = frame1.cols;
    image_msg->encoding = "bgr8";
    image_msg->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame1.step);
    size_t size = frame1.step * frame1.rows;
    image_msg->data.resize(size);
    memcpy(&image_msg->data[0], frame1.data, size);

    publisher1->publish(std::move(image_msg));

    rclcpp::spin_some(node);
    loop_rate.sleep();

    imshow("merged", frame1);
    namedWindow("merged");

    cv::waitKey(10);

    char ch = cv::waitKey(10);
    if(ch == 27) break;
  }

}