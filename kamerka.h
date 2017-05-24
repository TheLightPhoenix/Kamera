#ifndef KAMERKA_H
#define KAMERKA_H

#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"

class Kamerka
{
    private:
        bool mysz, wyswietlanie, klikniecie;
        cv::Mat frame, img, hsv_img, binary, cont, kulka, src_gray, src;
        std::vector<cv::Mat> hsv_split;
        std::vector<std::string> window_name;
        int lowerb, upperb, areaMax, areaMin, thres, minR, maxR;
        float area;
        cv::Point srodek;
        cv::VideoCapture capture;
        char m;
    public:
        Kamerka();
        ~Kamerka();

        void update();
};

#endif // KAMERKA_H
