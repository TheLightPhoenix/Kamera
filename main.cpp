#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture capture = VideoCapture(1);
    string window_name [] = { "Kamera", "Binary", "Blur" , "Erode" };
    Mat frame, img, hsv_img, binary, imgblur, imgerode;
    vector<Mat> hsv_split;
    for ( int i = 0; i < 3; i++ ) namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);
    int lowerb = 255, upperb = 255;
    createTrackbar( "lb", window_name[1], &lowerb, 255, NULL );
    createTrackbar( "ub", window_name[1], &upperb, 255, NULL );
    while ( waitKey(20) != 27 )
    {
        capture >> frame;
        //frame.copyTo(img);
        flip(frame, img, 1);
        cvtColor(img, hsv_img, CV_BGR2HSV);
        split(hsv_img, hsv_split);
        /*imshow("Kamera", hsv_split[0]);
        imshow("Binary", hsv_split[1]);
        imshow("Blur", hsv_split[2]);*/
        inRange(hsv_split[2], lowerb, upperb, binary);
        cv::Mat element(3,3,CV_8U,cv::Scalar(1));
        imshow(window_name[1], binary);
        blur(binary, imgblur, cv::Size(3,3) );
        imshow(window_name[2], imgblur);
        erode(imgblur, imgerode, element,Point(-1,-1), 2);
        imshow(window_name[0], img );
        imshow(window_name[3], imgerode);
    }
    capture.release();
    return 0;
}
