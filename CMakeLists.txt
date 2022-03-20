cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project ("CamWorker")

find_package(OpenCV 4 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB CamWorker_SRC "src/*.h" "src/*.cpp" "src/FrameProcessors/*.h" "src/FrameProcessors/*.cpp")
add_executable(CamWorker ${CamWorker_SRC})

target_link_libraries(CamWorker ${OpenCV_LIBS} pthread)    
