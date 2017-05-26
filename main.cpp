#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>

#include "kamerka.h"

using namespace cv;
using namespace std;

int main()
{
    char key = 'p';
    Kamerka cam;

    thread tr = cam.spawn();
    tr.detach();
    //std::thread t([] { std::cout << "My first thread..." << std::endl; });
    //t.detach();
    cout << "taaaaaaak" << endl;

    /*while ( key != 27 )
    {
        cam.update();
        key = waitKey(1);
    }*/
    return 0;
}
