#include "opencv2/opencv.hpp"
#include <opencv2/core/types_c.h>
#include <iostream>

using namespace cv;
using namespace std;

#define OFFSET 450
#define IMAGE_H 220
#define IMAGE_W 1280

enum TRANSFORM_TYPE {AFFINE, PERSPECTIVE};

Mat getTransform(Point2f *src, Point2f *dst, TRANSFORM_TYPE type);
void transform(Mat& src, Mat &dst, Mat transform_matrix, TRANSFORM_TYPE type);

int main(int argc, char *argv[])
{
    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        cout << "Unable to open file: " << argv[1] << endl;
        return -1;
    }
    
    Point2f src_points[4];
    // src_points[0] = Point(0,IMAGE_H + OFFSET);
    // src_points[1] = Point(1207, IMAGE_H + OFFSET);
    // src_points[2] = Point(0, OFFSET);
    // src_points[3] = Point(IMAGE_W, OFFSET);

    src_points[0] = Point(450, OFFSET);
    src_points[1] = Point(IMAGE_W-300, OFFSET);
    src_points[2] = Point(IMAGE_W, IMAGE_H + OFFSET);
    src_points[3] = Point(0,IMAGE_H + OFFSET);

    Point2f dst_points[4];
    // dst_points[0] = Point(570, IMAGE_H);
    // dst_points[1] = Point(710, IMAGE_H);
    // dst_points[2] = Point(0, 0);
    // dst_points[3] = Point(IMAGE_W, 0);

    dst_points[0] = Point(0, 0);
    dst_points[1] = Point(IMAGE_W, 0);
    dst_points[2] = Point(IMAGE_W, IMAGE_H);
    dst_points[3] = Point(0, IMAGE_H);

    //Mat M = getTransform(src_points, dst_points, PERSPECTIVE);
    Mat M = getTransform(src_points, dst_points, AFFINE);
    Mat dst(IMAGE_H, IMAGE_W, CV_8UC3);

    while (1) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) break;

        //transform(frame, dst, M, PERSPECTIVE);
        transform(frame, dst, M, AFFINE);

        imshow("src", frame);
        //waitKey(0);
        imshow("bird's eye view", dst);
        if ((char)waitKey(25) == 27) break;
    }
    cap.release();
    destroyAllWindows();
}

Mat getTransform(Point2f *src, Point2f *dst, TRANSFORM_TYPE type)
{
    if (type == AFFINE) {
        return getAffineTransform(src, dst);
    }
    else if (type == PERSPECTIVE) {
        return getPerspectiveTransform(src, dst);
    }
}

void transform(Mat& src, Mat &dst, Mat transform_matrix, TRANSFORM_TYPE type)
{
    if (type == AFFINE) {
        warpAffine(src, dst, transform_matrix, dst.size());
    }
    else if (type == PERSPECTIVE) {
        warpPerspective(src, dst, transform_matrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
    }
}