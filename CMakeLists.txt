cmake_minimum_required (VERSION 3.8)

project ("CamWorker")

file(GLOB CamWorker_SRC "src/*.h" "src/*.cpp" "src/FrameProcessors/*.h" "src/FrameProcessors/*.cpp")
add_executable(CamWorker ${CamWorker_SRC})

FIND_PACKAGE( OpenCV REQUIRED )     
TARGET_LINK_LIBRARIES (CamWorker ${OpenCV_LIBS})    

# TODO: Add tests and install targets if needed.
