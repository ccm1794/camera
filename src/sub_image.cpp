#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace std;


class VideoSubscriber : public rclcpp::Node
{
public:
  VideoSubscriber()
  : Node("video_subscriber")
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
      "video1", 10, 
      [this](sensor_msgs::msg::Image::SharedPtr msg) -> void
      {
        image_callback(msg);
      });
  }

public:
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg) 
  {
    Mat frame;
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
      frame = cv_ptr->image;
    }
    catch(cv_bridge::Exception& e)
    {
      RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
      return;
    }
    imshow("video_subscriber", frame);
    char ch = cv::waitKey(10);
    if(ch == 27) 
    {
      rclcpp::shutdown();
    }
  }
  private:
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<VideoSubscriber>());
  rclcpp::shutdown();
  return 0;
}