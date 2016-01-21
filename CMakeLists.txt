PROJECT(CaRetine)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)

add_definitions(-std=c++0x -stdlib=libc++)


add_executable(TestPlus2000 src/TestPlus2000.cpp src/Cell.cpp src/ImageMode.cpp src/Reseau.cpp src/VideoStream.cpp)
target_link_libraries( TestPlus2000 ${OpenCV_LIBS} )







