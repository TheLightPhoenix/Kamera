#include "kamerka.h"

#include <iostream>

#include <Windows.h>
#include <Winuser.h>
#include <Winable.h>

using namespace cv;
using namespace std;

Kamerka::Kamerka()
{
    capture = VideoCapture(0);

    window_name.push_back("Kamera");
    window_name.push_back("Contour");
    window_name.push_back("Binary");
    window_name.push_back("Kulka");
    window_name.push_back("Okregi");

    mysz = false;
    wyswietlanie = true;
    klikniecie = false;

    m = 'l';

    lowerb = 255;
    upperb = 255;
    areaMax = 6500;
    areaMin = 500;
    area = 0;
    thres = 20;
    minR = 10;
    maxR = 36;
}

Kamerka::~Kamerka()
{
    capture.release();
}

void Kamerka::update()
{
    if(wyswietlanie)
    {
        for ( int i = 0; i < 5; i++ )
            namedWindow(window_name[i], CV_WINDOW_AUTOSIZE);

        createTrackbar("AreaMax", "Kulka", &areaMax, 10000);
        createTrackbar("AreaMin", "Kulka", &areaMin, 10000);
        createTrackbar("canny", "Okregi", &thres, 100);
        createTrackbar("minR", "Okregi", &minR, 100);
        createTrackbar("maxR", "Okregi", &maxR, 100);
    }
    while(waitKey(1) != 27)
    {
    capture >> frame;
    flip(frame, img,1);
    img.copyTo(src);
    cvtColor(img, hsv_img, CV_BGR2HSV);
    split(hsv_img, hsv_split);
    inRange(hsv_split[2], lowerb, upperb, binary);
    blur(binary, binary, cv::Size(3,3) );
    erode(binary, binary, cv::Mat() );

    vector<vector<Point> > contours;
    vector<Point> contours_poly;
    Rect boundRect;
    binary.copyTo(cont);
    findContours( cont, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    int max = 0, i_cont = -1;
    Mat drawing = Mat::zeros( cont.size(), CV_8UC3 );
    if(mysz)
    {
        if(contours.size()==0 && !klikniecie)
        {
            klikniecie = true;
            INPUT    Input={0};

            Input.type      = INPUT_MOUSE;
            if(m=='l')
                Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
            else
                Input.mi.dwFlags  = MOUSEEVENTF_RIGHTDOWN;
            SendInput(1,&Input,sizeof(INPUT));
        }

        if(contours.size() > 0 && klikniecie)
        {
            klikniecie = false;
            INPUT    Input={0};

            Input.type      = INPUT_MOUSE;
            if(m=='l')
                Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
            else
                Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
            SendInput(1,&Input,sizeof(INPUT));
        }
    }

    for( int i = 0; i< contours.size(); i++ )
    {
        area = abs(contourArea(Mat(contours[i])));
        if ( area > max && area > areaMin && area < areaMax)
        {
            max = area;
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
    cvtColor( src, src_gray, CV_BGR2GRAY );

    GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );

    vector<Vec3f> circles;

    if(thres > 0)
        HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, thres, minR, maxR );

    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
        // circle outline
        circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
    }

    kulka = Mat::zeros( img.size(), CV_8UC3 );
    circle(kulka, srodek, 3, Scalar(255, 0, 0));
    if(mysz)
        SetCursorPos(srodek.x*2, srodek.y*2);

    if(wyswietlanie)
    {
        imshow(window_name[1], drawing);
        imshow(window_name[0], img );
        imshow(window_name[2], src_gray);
        imshow(window_name[3], kulka);
        imshow(window_name[4], src);
    }
    }
}
