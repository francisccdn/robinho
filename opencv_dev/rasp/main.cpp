#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
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
    VideoCapture cap = VideoCapture(0);

    if(!cap.isOpened())
    {
        cerr << "Couldn't open camera." << endl;
        return 1;
    }

    // Objetos para img em BRG e em HSV
    Mat frameBRG, frameHSV, frameMask;    
    Mat frame;//Deletar na versao final (so serve pra mostrar no monitor)

    // Range da cor que vai ser detectada
    const int redLower[3] = {0, 150, 50};
    const int redUpper[3] = {95, 220, 225};
    
    const int whiteLower[3] = {0, 0, 200};
    const int whiteUpper[3] = {0, 50, 255};

    const int pinkLower[3] = {135, 120, 80};
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

    // Parametros para deteccao do alvo
    const float min_radius = 0.01; // Raio minimo do objeto para ser considerado
    const float catch_radius[2] = {45, 60}; // Raio quando a distancia for a de ser pego pela garra
    const float lim_front[2] = {500, 600}; // Limites do meio da tela (quando o objeto esta aqui, esta em frente ao carrinho)
    
    // Objetos para deteccao do alvo
    vector<vector<Point>> contours;
    vector<Point> obj;
    Point2f center;
    float radius;

    while(1)
    {
        // Jogar frames da camera para Mat frameBRG
        cap.read(frameBRG);
        frame = frameBRG; //Deletar na versao final

        if(frameBRG.empty())
        {
            cerr << "Empty frame." << endl;
            break;
        }

        GaussianBlur(frameBRG, frameBRG, Size2i(11,11), 0); //Aplica um blur pro filtro ter menos ruido
        cvtColor(frameBRG, frameHSV, COLOR_BGR2HSV); //Converte a imagem lida em BRG para HSV

        // Filtrar a imagem
        inRange(frameHSV, Scalar(colorLower[0], colorLower[1], colorLower[2]), Scalar(colorUpper[0], colorUpper[1], colorUpper[2]), frameMask);
        InputArray kernel = getStructuringElement(MORPH_RECT, Size2i(3,3));
        erode(frameMask, frameMask, kernel);
        dilate(frameMask, frameMask, kernel);

        // Exibir captura da camera com filtros -- Deletar na versao final
        namedWindow("Mask", WINDOW_AUTOSIZE);
        imshow("Mask", frameMask);

        // Encontra o contorno do Objetos
        findContours(frameMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //pode dar problema pela versao

        if(contours.size() > 0)
        { // Se algum objeto foi encontrado
            obj = findBestContour(contours);
            minEnclosingCircle(obj, center, radius);
            
            string center_str = "Center: X = " + to_string(center.x) + " Y  = " + to_string(center.y); // Deletar na versao final

            if(radius > min_radius)
            {
                circle(frame, center, radius, Scalar(0, 255, 255), 2); // Deletar na versao final
                putText(frame, center_str, Point(30, 30), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0)); // Deletar na versao final

                /*if(catch_radius[0] <= radius && radius >= catch_radius[1])
                {
                    // TODO - Mandar pra fpga q ta perto
                }*/
            }
/*
            if(lim_front[0] <= center.x && center.x >= lim_front[1])
            {
                // TODO - Mandar pra fpga q o objeto ta em frente 
            }
            else
            {
                // TODO - Mandar pra fpga q n tem objeto em vista 
            }
*/          
        }
        else
        {
           // TODO - Mandar pra fpga q n tem objeto em vista 
        }
        
        // Exibir captura da camera com circulo ao redor da bola -- Deletar na versao final
        namedWindow("Cam input", WINDOW_AUTOSIZE);
        imshow("Cam input", frame);

        // Fecha o programa ao apertar qualquer tecla
        if(waitKey(30) != -1)
        {
            cap.release();
            break;
        }
    }

    std::cout << "Ending program." << endl;
    
    return 0;
}
