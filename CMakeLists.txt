cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("CamWorker")

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB CamWorker_SRC "src/*.h" "src/*.cpp" "src/FrameProcessors/*.h" "src/FrameProcessors/*.cpp")
add_executable(CamWorker ${CamWorker_SRC})

FIND_PACKAGE(OpenCV REQUIRED)     

TARGET_LINK_LIBRARIES (CamWorker ${OpenCV_LIBS})    
