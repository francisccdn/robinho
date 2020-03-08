#include "Target.h"

Target::Target(std::vector<cv::Point> contour)
{
    this->contour = contour;
    cv::minEnclosingCircle(contour, center, radius);
}

Target::Target(std::vector<std::vector<cv::Point>> v)
{
    std::vector<cv::Point> b = v[0];
    for(int i = 0; i < v.size(); i++)
    {
        if(cv::contourArea(b) < cv::contourArea(v[i]))    
            b = v[i];
    }
    Target((std::vector<cv::Point>) b);
}