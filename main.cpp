#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture capture = VideoCapture(1);
    Mat frame;
    Mat hsv_img;
    vector<Mat> hsv_split;
    namedWindow("Kamerka", CV_WINDOW_AUTOSIZE);
    namedWindow("Hsv", CV_WINDOW_AUTOSIZE);
    namedWindow("Hue", CV_WINDOW_AUTOSIZE);
    namedWindow("Saturation", CV_WINDOW_AUTOSIZE);
    namedWindow("Value", CV_WINDOW_AUTOSIZE);
    while ( waitKey(20) != 27 )
    {
        capture >> frame;
        cvtColor(frame, hsv_img, CV_BGR2HSV);
        split(hsv_img, hsv_split);
        imshow("Kamerka", frame);
        imshow("Hsv", hsv_img);
        imshow("Hue", hsv_split[0]);
        imshow("Saturation", hsv_split[1]);
        imshow("Value", hsv_split[2]);
    }
    capture.release();
    return 0;
}
