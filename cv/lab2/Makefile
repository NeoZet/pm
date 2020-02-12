CC              := g++ -std=c++11
CFLAGS          := -I/home/rem/direct/opencv/opencv-4.1.1/build/install/include/opencv4 -L/home/rem/direct/opencv/opencv-4.1.1/build/install/lib
OBJECTS         := birds_eye_view.o
LIBRARIES       := -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_imgcodecs -lopencv_videoio
SOURCE          := birds_eye_view.cpp
EXEC            := birds_eye_view

.PHONY: all clean

all: $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) -o $(EXEC) $(SOURCE) $(LIBRARIES)

clean:
	rm -f *.o $(EXEC)