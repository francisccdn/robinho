#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <raspicam/raspicam_cv.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    // Abrir a camera
    raspicam::RaspiCam_Cv cap;

    if(!cap.open())
    {
        cerr << "Couldn't open camera." << endl;
        return 1;
    }

    // Objetos para img em BRG e em HSV
    Mat frameBRG, frameHSV, frameMask;

    // Range da cor que vai ser detectada
    const int redLower[3] = {170, 070, 000};
    const int redUpper[3] = {179, 255, 255};
    
    const int whiteLower[3] = {000, 000, 200};
    const int whiteUpper[3] = {000, 050, 255};

    const int pinkLower[3] = {135, 120, 080};
    const int pinkUpper[3] = {175, 255, 255};

    int colorLower[3] = {0, 0, 0}, colorUpper[3] = {179, 255, 255};

    // Barrinhas para escolher uma cor
    namedWindow("Control", WINDOW_AUTOSIZE);
    createTrackbar("LowH", "Control", &colorLower[0], 179); //Hue (0 - 179)
    createTrackbar("HighH", "Control", &colorUpper[0], 179);
    createTrackbar("LowS", "Control", &colorLower[1], 255); //Saturation (0 - 255)
    createTrackbar("HighS", "Control", &colorUpper[1], 255);
    createTrackbar("LowV", "Control", &colorLower[2], 255);//Value (0 - 255)
    createTrackbar("HighV", "Control", &colorUpper[2], 255);

    while(1)
    {
        // Jogar frames da camera para Mat frameBRG
        cap.grab();
        cap.retrieve(frameBRG);

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
        inRange(frameHSV, Scalar(colorLower[0], colorLower[1], colorLower[2]), Scalar(colorUpper[0], colorUpper[1], colorUpper[2]), frameMask);
        erode(frameMask, frameMask, NULL);
        dilate(frameMask, frameMask, NULL);

        // Exibir captura da camera com filtros
        namedWindow("Mask", WINDOW_AUTOSIZE);
        imshow("Mask", frameMask);

        // Fecha o programa ao apertar qualquer tecla
        if(waitKey(30) > 0)
        {
            cap.release();
            break;
        }
    }

    std::cout << "Ending program." << endl;
    
    return 0;
}
