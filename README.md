# CamWorker

This is a test application designed to recieve feed from a web camera and apply transformations to it using a set of frame processors.

While all OpenCV highgui code is run on the main thread, the retrieval of frames and the processing is done on separate threads in the background.
All this makes heavy use of concurrent circular queues (see class FrameQueue).

Built for x64 using Visual Studio 2019 with Windows SDK 10 and OpenCV 4.5.4

Build instructions:
1) Install Visual Studio 2019 or newer with build tools v142
2) Define an env variable called COMMONPACKAGESROOT, this variable should point to the location where an OpenCV installation is located (for instance, C:\Dev\libs). The provided code looks for OpenCV 4.5.4 in the folder called "opencv454" under COMMONPACKAGESROOT. Change those values in the project settings if necessary.
