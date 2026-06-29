#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/point.hpp>
#include <geometry_msgs/msg/polygon.hpp> // Добавлено
#include <algorithm>
#include <vector> // Добавлено

class StateMachineNode : public rclcpp::Node {
public:
    StateMachineNode() : Node("state_machine_node") {
        RCLCPP_INFO(this->get_logger(), "State Machine Node started. Hunting for ArUco...");

        cmd_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

        target_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
            "/robot_position", 10, // Изменено на топик вашего трекера
            std::bind(&StateMachineNode::target_cb, this, std::placeholders::_1)
        );

        // Подписка на препятствия
        obstacles_sub_ = this->create_subscription<geometry_msgs::msg::Polygon>(
            "/obstacles_positions", 10,
            [this](const geometry_msgs::msg::Polygon::SharedPtr msg) {
                obstacles_ = msg->points;
            }
        );

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(50),
            std::bind(&StateMachineNode::control_loop, this)
        );
    }

private:
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr target_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Polygon>::SharedPtr obstacles_sub_; // Добавлено
    rclcpp::TimerBase::SharedPtr timer_;

    std::vector<geometry_msgs::msg::Point32> obstacles_; // Добавлено

    rclcpp::Time last_target_time_;
    double target_x_ = 0.0;
    double target_area_ = 0.0;
    bool target_active_ = false;

    const double KP_ANGULAR = 1.2;
    const double MAX_LINEAR = 0.15;
    const double STOP_AREA = 50000.0;
    const double TARGET_TIMEOUT = 0.5;

    void target_cb(const geometry_msgs::msg::Point::SharedPtr msg) {
        if (msg->z > 0.5) { // Проверка флага активности
            target_x_ = msg->x;
            target_area_ = 1000.0; // Условная площадь
            last_target_time_ = this->get_clock()->now();
            target_active_ = true;
        } else {
            target_active_ = false;
        }
    }

    void control_loop() {
        auto cmd_msg = geometry_msgs::msg::Twist();
        auto now = this->get_clock()->now();

        // 0. ПРОВЕРКА НА ПРЕПЯТСТВИЯ (Высший приоритет)
        if (!obstacles_.empty()) {
            cmd_msg.linear.x = 0.0;
            cmd_msg.angular.z = 0.0;
            cmd_pub_->publish(cmd_msg);
            return; 
        }

        // 1. ЛОГИКА ДВИЖЕНИЯ
        if (target_active_ && (now - last_target_time_).seconds() < TARGET_TIMEOUT) {
            cmd_msg.angular.z = -KP_ANGULAR * target_x_;
            cmd_msg.angular.z = std::clamp(cmd_msg.angular.z, -1.5, 1.5);
            
            if (target_area_ < STOP_AREA) {
                double speed_factor = 1.0 - (target_area_ / STOP_AREA);
                cmd_msg.linear.x = MAX_LINEAR * std::max(0.2, speed_factor);
            } else {
                cmd_msg.linear.x = 0.0;
            }
        } else {
            target_active_ = false;
            cmd_msg.linear.x = 0.0;
            cmd_msg.angular.z = 0.0;
        }

        cmd_pub_->publish(cmd_msg);
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StateMachineNode>());
    rclcpp::shutdown();
    return 0;
}
