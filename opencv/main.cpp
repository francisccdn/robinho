#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
    Mat frame;
    VideoCapture cap;
    
    const int deviceID = 0;

    cap.open(deviceID);
    if(!cap.isOpened())
    {
        cerr << "Couldn't open camera." << endl;
        return 1;
    }

    while(1)
    {
        cap.read(frame);

        if(frame.empty())
        {
            cerr << "Empty frame." << endl;
            break;
        }

        namedWindow("WebCam", WINDOW_AUTOSIZE);
        imshow("WebCam", frame);

        if(waitKey(30) != 255)
        {
            cap.release();
            break;
        }
    }

    std::cout << "Ending program." << endl;
    
    return 0;
}