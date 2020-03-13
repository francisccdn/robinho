#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <raspicam/raspicam_cv.h>
#include <pigpio.h>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

vector<cv::Point> findBestContour(std::vector<std::vector<cv::Point>> v)
{
    std::vector<cv::Point> b = v[0];
    for(int i = 0; i < v.size(); i++)
    {
        if(cv::contourArea(b) < cv::contourArea(v[i]))    
            b = v[i];
    }
    return b;
}

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
    const int colorLower[3] = {135, 120, 80}, colorUpper[3] = {175, 255, 255}; /*Rosa*/

    // Parametros para deteccao do alvo
    const float min_radius = 0.01; // Raio minimo do objeto para ser considerado
    const float catch_radius[2] = {45, 60}; // Raio quando a distancia for a de ser pego pela garra
    const float lim_front[2] = {500, 600}; // Limites do meio da tela (quando o objeto esta aqui, esta em frente ao carrinho)
    
    // Objetos para deteccao do alvo
    vector<vector<Point>> contours;
    vector<Point> obj;
    Point2f center;
    float radius;

    // Pi GPIO pins
    const int w_motor[4] = {35, 36, 37, 38};
    const int c_motor[4] = {7, 11, 13, 15};
    const int rst = 22;

    if(gpioInitialise() < 0)
        cerr << "GPIO init failed." << endl;
    else
    { // Pi GPIO setup
        for(int i = 0; i < 4; i++)
        {
            gpioSetMode(w_motor[i], PI_OUTPUT);
            gpioSetMode(c_motor[i], PI_OUTPUT);
        }
        gpioSetMode(rst, PI_INPUT);

        for(int w : w_motor) gpioWrite(w, 1);
    }

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

        GaussianBlur(frameBRG, frameBRG, Size2i(11,11), 0); //Aplica um blur pro filtro ter menos ruido
        cvtColor(frameBRG, frameHSV, COLOR_BGR2HSV); //Converte a imagem lida em BRG para HSV

        // Filtrar a imagem
        inRange(frameHSV, Scalar(colorLower[0], colorLower[1], colorLower[2]), Scalar(colorUpper[0], colorUpper[1], redUpper[2]), frameMask);
        erode(frameMask, frameMask, NULL);
        dilate(frameMask, frameMask, NULL);

        // Encontra o contorno do Objetos
        findContours(frameMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); //pode dar problema pela versao

        if(contours.size() > 0)
        { // Se algum objeto foi encontrado
            obj = findBestContour(contours);
            minEnclosingCircle(obj, center, radius);
            
            if(radius > min_radius)
            {
                if(catch_radius[0] <= radius && radius >= catch_radius[1])
                {
                    gpioWrite(PIN_DISTANCE, 1); // Tá na distancia para pegar com a garra
                }
            }

            if(lim_front[0] <= center.x && center.x >= lim_front[1])
            {
                gpioWrite(PIN_SIGHT, 1); // Objeto está em frente
            }
            else
            {
                gpioWrite(PIN_SIGHT, 0); // Objeto não está em frente
            }          
        }
        else
        {
           gpioWrite(PIN_SIGHT, 0); // Objeto não está em frente
        }

        // Reinicia os pinos se detectar reset
        if(gpioRead(PIN_RST))
        {
            gpioWrite(PIN_ON, 1);
            gpioWrite(PIN_SIGHT, 0);
            gpioWrite(PIN_DISTANCE, 0);
        }

        // Fecha o programa ao apertar qualquer tecla
        if(waitKey(30) != -1)
        {
            cap.release();
            break;
        }
    }

    gpioWrite(PIN_ON, 0);
    gpioTerminate();
    std::cout << "Ending program." << endl;
    
    return 0;
}