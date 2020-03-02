#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
    // Abrir a camera
    VideoCapture cap;
    const int deviceID = 0;

    cap.open(deviceID);
    if(!cap.isOpened())
    {
        cerr << "Couldn't open camera." << endl;
        return 1;
    }

    // Objetos para img em BRG e em HSV
    Mat frameBRG, frameHSV, frameFilter;

    // Range da cor que vai ser detectada
    int RedLow[3]  = {170, 070, 000};
    int RedHigh[3] = {179, 255, 255}; 

    while(1)
    {
        // Jogar frames da camera para Mat frameBRG
        cap.read(frameBRG);

        if(frameBRG.empty())
        {
            cerr << "Empty frame." << endl;
            break;
        }

        // Exibir captura da camera sem filtros
        namedWindow("Cam input", WINDOW_AUTOSIZE);
        imshow("Cam input", frameBRG);

        // Converte a imagem lida em BRG para HSV
        cvtColor(frameBRG, frameHSV, COLOR_BGR2HSV);

        // Filtrar a imagem
        inRange(frameHSV, Scalar(RedLow[0], RedLow[1], RedLow[2]), Scalar(RedHigh[0], RedHigh[1], RedHigh[2]), frameFilter);

        // Exibir captura da camera com filtros
        namedWindow("Filtered", WINDOW_AUTOSIZE);
        imshow("Filtered", frameFilter);

        // Fecha o programa ao apertar qualquer tecla
        if(waitKey(30) != 255)
        {
            cap.release();
            break;
        }
    }

    std::cout << "Ending program." << endl;
    
    return 0;
}