
all: partA partB

partA: partA.cpp
	g++ partA.cpp -o partA -I/usr/include/opencv -I/usr/include/opencv2 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

partB: partB.cpp
	g++ partB.cpp -o partB -I/usr/include/opencv -I/usr/include/opencv2 -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs



clean:
	rm partA partB
