# CamWorker

This is a test application designed to recieve feed from a web camera and apply transformations to it using a set of frame processors.

While all OpenCV highgui code is run on the main thread, the retrieval of frames and the processing is done on separate threads in the background.
All this makes heavy use of concurrent circular queues (see file src/CircularQueue.h).

Built for x64 using C++14 with OpenCV 4.5.4 and CMake 3.5

Instructions to build & run:
1) Install OpenCV and CMake
2) Go to repository folder an run: cmake . && make
3) Run executable called CamWorker
