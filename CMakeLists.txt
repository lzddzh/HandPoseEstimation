cmake_minimum_required(VERSION 2.8)
project(HandPoseEstimation)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++11")
set(SOURCE_FILES
        common.h
        LoadData.cpp
        LoadData.h
        main.cpp
        Node.cpp
        Node.h
        RandomForest.cpp
        RandomForest.h
        Tree.cpp
        Tree.h calSim.cpp
        main.cpp)

find_package(Armadillo REQUIRED)
find_package( OpenCV REQUIRED )
include_directories(${ARMADILLO_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(HandPoseEstimation ${SOURCE_FILES})

target_link_libraries(HandPoseEstimation ${ARMADILLO_LIBRARIES})
target_link_libraries(HandPoseEstimation ${OpenCV_LIBS} )
