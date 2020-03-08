#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

class Target
{
private:
    std::vector<cv::Point> contour;
    cv::Point2f center;
    float radius;

public:
    std::vector<cv::Point> getContour() { return contour; }
    cv::Point2f getCenter() { return center; }
    float getRadius() { return radius; }

    Target(std::vector<cv::Point> contour);
    Target(std::vector<std::vector<cv::Point>> v);
};