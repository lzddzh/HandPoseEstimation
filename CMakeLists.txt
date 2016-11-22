cmake_minimum_required(VERSION 2.8)
project(HandPoseEstimation)

find_package(OpenMP)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
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
        Tree.h
        CalSim.cpp
        CalSim.h
        Block.h
        Block.cpp
        main.cpp)

find_package(Armadillo REQUIRED)
find_package( OpenCV REQUIRED )
include_directories(${ARMADILLO_INCLUDE_DIRS})
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(HandPoseEstimation ${SOURCE_FILES})

target_link_libraries(HandPoseEstimation ${ARMADILLO_LIBRARIES})
target_link_libraries(HandPoseEstimation ${OpenCV_LIBS} )
