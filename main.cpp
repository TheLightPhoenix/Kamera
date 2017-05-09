#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <Winable.h>

using namespace cv;
using namespace std;

int main()
{
    VideoCapture capture = VideoCapture(0);
    string window_name [] = { "Kamera", "Contour", "Binary" };
    Mat frame, img, hsv_img, binary;

    //***
    Mat cont;
    //***

    vector<Mat> hsv_split;
    for ( int i = 0; i < 3; i++ ) namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);
    int lowerb = 255, upperb = 255;
    createTrackbar( "Thresh lb", window_name[2], &lowerb, 255, NULL );
    createTrackbar( "Thresh ub", window_name[2], &upperb, 255, NULL );
    namedWindow("Kulka", CV_WINDOW_AUTOSIZE);
    Mat kulka;
    Point srodek;
    while ( waitKey(20) != 27 )
    {
        capture >> frame;
        flip(frame, img,1);
        cvtColor(img, hsv_img, CV_BGR2HSV);
        split(hsv_img, hsv_split);
        inRange(hsv_split[2], lowerb, upperb, binary);
        blur(binary, binary, cv::Size(3,3) );
        erode(binary, binary, cv::Mat() );

        //***
        vector<vector<Point> > contours;
        vector<Point> contours_poly;
        Rect boundRect;
        binary.copyTo(cont);
        findContours( cont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        int max = 0, i_cont = -1;
        Mat drawing = Mat::zeros( cont.size(), CV_8UC3 );
        for( int i = 0; i< contours.size(); i++ )
        {
            if ( abs(contourArea(Mat(contours[i]))) > max )
            {
                max = abs(contourArea(Mat(contours[i])));
                i_cont = i;
            }
        }
        if ( i_cont >= 0 )
        {
            approxPolyDP( Mat(contours[i_cont]), contours_poly, 3, true );
            boundRect = boundingRect( Mat(contours_poly) );
            fillConvexPoly(img, contours_poly, contours_poly.size() );
            rectangle( img, boundRect.tl(), boundRect.br(), Scalar(125, 250, 125), 2, 8, 0 );
            line( img, boundRect.tl(), boundRect.br(), Scalar(250, 125, 125), 2, 8, 0);
            line( img, Point(boundRect.x + boundRect.width, boundRect.y), Point(boundRect.x, boundRect.y + boundRect.height), Scalar(250, 125, 125), 2, 8, 0);
            string s;
            stringstream out;
            srodek = Point(boundRect.x + boundRect.width/2, boundRect.y + boundRect.height/2);
            out << boundRect.x + boundRect.width/2 << "x" << boundRect.y + boundRect.height/2;
            s = out.str();
            putText( img, s, Point(50, 50), CV_FONT_HERSHEY_COMPLEX, 1, Scalar(20, 40, 80), 3, 8 );
            drawContours( drawing,  contours, i_cont, Scalar(125, 125, 250), 2 );
        }
        imshow(window_name[1], drawing);
        //***

        imshow(window_name[0], img );
        imshow(window_name[2], binary);

        kulka = Mat::zeros( img.size(), CV_8UC3 );
        circle(kulka, srodek, 3, Scalar(255, 0, 0));
        SetCursorPos(srodek.x*2, srodek.y*2);
        imshow("Kulka", kulka);
        if(waitKey(10) == 'm')
        {
            INPUT    Input={0};
            // left down
            Input.type      = INPUT_MOUSE;
            Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
            SendInput(1,&Input,sizeof(INPUT));

            // left up
            ZeroMemory(&Input,sizeof(INPUT));
            Input.type      = INPUT_MOUSE;
            Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
            SendInput(1,&Input,sizeof(INPUT));
        }
    }
    capture.release();
    return 0;
}
