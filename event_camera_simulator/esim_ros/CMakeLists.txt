cmake_minimum_required(VERSION 2.8.3)
project(esim_ros)

find_package(catkin_simple REQUIRED)
find_package(OpenCV REQUIRED)
catkin_simple()

set(HEADERS
)

set(SOURCES
    src/esim_node.cpp
)

# NOTE: esim_node.cpp contains main() and the gflags DEFINE_*s. Compiling it
# into both a library and the executable registers each flag twice, which gflags
# rejects at startup on macOS ("linked both statically and dynamically"). Nothing
# depends on the esim_ros library, so only build the executable.
cs_add_executable(esim_node  src/esim_node.cpp)

# link the executable to the necessary libs
target_link_libraries(esim_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})

cs_install()
