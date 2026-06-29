find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(arena_tracker_node src/arena_tracker_node.cpp)
target_include_directories(arena_tracker_node PUBLIC ${OpenCV_INCLUDE_DIRS})
ament_target_dependencies(arena_tracker_node rclcpp geometry_msgs)
target_link_libraries(arena_tracker_node ${OpenCV_LIBRARIES})

install(TARGETS
  arena_tracker_node
  DESTINATION lib/${PROJECT_NAME}
)
