#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
//#include <raspicam/raspicam_cv.h>
//#include <pigpio.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <chrono>
//#include "Robot.h"

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
    VideoCapture cap;
    cap.open(0);

    if(!cap.isOpened())
    {
        cerr << "Couldn't open camera." << endl;
        return 1;
    }

    // Objetos para img em BRG e em HSV
    Mat frame, frameBRG, frameHSV, frameMask;    

    // Range da cor que vai ser detectada
    const int colorLower[3] = {130, 60, 100}, colorUpper[3] = {175, 255, 255}; /*Rosa*/

    // Parametros para deteccao do alvo
    const float min_radius = 20; // Raio minimo do objeto para ser considerado
    const float catch_radius[2] = {100, 110}; // Raio quando a distancia for a de ser pego pela garra
    const float lim_front[2] = {250, 350}; // Limites do meio da tela (quando o objeto esta aqui, esta em frente ao carrinho)
    
    // Objetos para deteccao do alvo
    vector<vector<Point>> contours;
    vector<Point> obj;
    Point2f center;
    float radius;

    // Objeto do robo
    /*Robot *robinho = new Robot(35, 36, 37, 38, 13, 15, 16, 18);
    try
    {
        robinho->isOn();
        cout << "gpio initialized" << endl;
    }
    catch(gpioInitException& e)
    {
        std::cerr << e.what() << '\n';
        return 2;
    }*/
    

    while(1)
    {
        // Jogar frames da camera para Mat frameBRG
        cap.read(frameBRG);
        frame = frameBRG;

        if(frameBRG.empty())
        {
            cerr << "Empty frame." << endl;
            break;
        }

        GaussianBlur(frameBRG, frameBRG, Size2i(11,11), 0); //Aplica um blur pro filtro ter menos ruido
        cvtColor(frameBRG, frameHSV, COLOR_BGR2HSV); //Converte a imagem lida em BRG para HSV

        // Filtrar a imagem
        inRange(frameHSV, Scalar(colorLower[0], colorLower[1], colorLower[2]), Scalar(colorUpper[0], colorUpper[1], colorUpper[2]), frameMask);
        erode(frameMask, frameMask, NULL);
        dilate(frameMask, frameMask, NULL);

        // Encontra o contorno do Objetos
        findContours(frameMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        bool seeingObjects = (contours.size() > 0) ? true : false; 

        if(seeingObjects)
        { // Se algum objeto foi encontrado
            obj = findBestContour(contours);
            minEnclosingCircle(obj, center, radius);

            string center_str = "Center: X = " + to_string(center.x) + " Y  = " + to_string(center.y);
            string radius_str = "Radius: " + to_string(radius);
            
            if(radius > min_radius)
            {
                circle(frame, center, radius, Scalar(0, 255, 255), 2);
                putText(frame, center_str, Point(30, 30), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0)); 
                putText(frame, radius_str, Point(30, 40), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

                if(catch_radius[0] <= radius && radius <= catch_radius[1])
                {
                    //robinho->grab(); // Tá na distancia para pegar com a garra
                    cout << "Ativar garra (sleep) \t robinho->grab()" << endl;
                }

                if(lim_front[0] <= center.x && center.x <= lim_front[1])
                {
                    //robinho->foward();
                    cout << "Andar para frente \t robinho->foward()" << endl;
                }
                else if(center.x < lim_front[0])
                {
                    //robinho->turn(LEFT); // Objeto esta a esquerda
                    cout << "Virar para a esquerda \t robinho->turn(LEFT)" << endl;
                }
                else if(lim_front[1] < center.x)
                {
                    //robinho->turn(RIGHT); // Objeto esta a direita
                    cout << "Virar para a direita \t robinho->turn(RIGT)" << endl;
                }  
            }
            else
            {
                seeingObjects = false;
            }
                        
        }
        if(!seeingObjects)
        {
           //robinho->search();
            cout << "Busca aleatoria \t robinho->search()" << endl;
        }

        // Exibir captura da camera
        namedWindow("Cam input", WINDOW_AUTOSIZE);
        imshow("Cam input", frame);

        // Fecha o programa ao apertar qualquer tecla
        if(waitKey(30) != 255)
        {
            cap.release();
            break;
        }
    }

    //delete robinho;
    std::cout << "Ending program." << endl;
    
    return 0;
}
