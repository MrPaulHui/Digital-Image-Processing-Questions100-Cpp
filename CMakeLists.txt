cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
project(DIP100)
find_package(OpenCV REQUIRED)
add_executable(t44-46_more_circle t44-46_more_circle.cpp)
target_link_libraries(t44-46_more_circle ${OpenCV_LIBS})
