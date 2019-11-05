#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	if (argc < 2) {
		cout << "Invalid number of arguments" << endl;
		return -1;
	}

    Mat image = imread(argv[1]);
    resize(image, image, Size(350, 350));

    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }
    String window_name = "Lena BGR";
    namedWindow(window_name);
    imshow(window_name, image);

	Mat image_hsv = image;
	cvtColor(image, image_hsv, COLOR_RGB2HSV);
    String window_name_hsv = "Lena HSV";
    namedWindow(window_name_hsv);
    imshow(window_name_hsv, image_hsv);

	Mat image_rgb = image;
	cvtColor(image, image_rgb, COLOR_BGR2RGB);
    String window_name_rgb = "Lena RGB";
    namedWindow(window_name_rgb);
    imshow(window_name_rgb, image_rgb);

	Mat image_mono = image;
	cvtColor(image, image_mono, COLOR_BGR2GRAY);
    String window_name_mono = "Lena MONO";
    namedWindow(window_name_mono);
    imshow(window_name_mono, image_mono);

	Mat image_bin_src = imread(argv[1], CV_8UC1);
	resize(image_bin_src, image_bin_src, Size(350, 350));
	Mat image_bin = image;
	adaptiveThreshold(image_bin_src, image_bin, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 5);
    String window_name_bin = "Lena BIN";
    namedWindow(window_name_bin);
    imshow(window_name_bin, image_bin);

    Mat image_canny = image;
    Mat image_blur = image_mono;
    int trashHold = 30;
    int ratio = 2;
    GaussianBlur(image_mono, image_blur, Size(3, 3), 0);
    Canny(image_blur, image_canny, trashHold, trashHold * ratio, 3);
    String window_name_canny = "Lena CANNY";
    namedWindow(window_name_canny);
    imshow(window_name_canny, image_canny);

    waitKey(0);
    destroyAllWindows();

    return 0;
}