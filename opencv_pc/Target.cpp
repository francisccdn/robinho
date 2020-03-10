#include "Target.h"

Target::Target()
{
    contour = std::vector<cv::Point>();
    center = cv::Point2f(-100,-100);
    radius = 0;
}

void Target::updatePos(std::vector<cv::Point> contour)
{
    this->contour = contour;
    cv::minEnclosingCircle(contour, center, radius);
}

bool Target::findBestContour(std::vector<std::vector<cv::Point>> v)
{
    std::vector<cv::Point> b = v[0];
    for(int i = 0; i < v.size(); i++)
    {
        if(cv::contourArea(b) < cv::contourArea(v[i]))    
            b = v[i];
    }
    updatePos(b);

    if(radius != 0)
        return true;
    else
        return false;
    
}