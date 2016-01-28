PROJECT(CaRetine)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

FIND_PACKAGE(OpenCV REQUIRED)

add_definitions(-std=c++0x -stdlib=libc++)


SET(Boost_USE_STATIC_LIBS   ON)
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
SET(Boost_FOUND FALSE)
FIND_PACKAGE(Boost 1.46.0 REQUIRED COMPONENTS program_options)
if ( Boost_FOUND )
  message(STATUS "Boost and boost_program_options found.")
  include_directories( ${Boost_INCLUDE_DIRS} )
endif( Boost_FOUND )


add_executable(TestPlus2000 src/TestPlus2000.cpp src/Cell.cpp src/ImageMode.cpp src/Reseau.cpp src/VideoStream.cpp)
target_link_libraries( TestPlus2000 ${OpenCV_LIBS}  ${Boost_LIBRAIRIES} ${Boost_PROGRAM_OPTIONS_LIBRARY}  )







