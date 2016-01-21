#pragma once

#include <iostream>
#include <opencv2\opencv.hpp>
#include "math.h"
#include <math.h> 

using namespace std;
using namespace cv;

class ShapesDetector
{
public:
	
	ShapesDetector(void);
	~ShapesDetector(void);

	Point getTriangleCenter(Mat image);
	void detectRectangles(Mat image);
	void assignCenterPoints(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter);
	void setRectangleCenterHours(Point area);
	Point getRectangleCenterHours();
	void setRectangleCenterMinutes(Point area);
	Point getRectangleCenterMinutes();
	bool triangleDetected;
	bool everythingDetected;

private:

	Point minPt;
	Point hourPt;
	Point centerPt;
	Point rectangleCenterHours(0,0);
	Point rectangleCenterMinutes(0,0);

	Point rectangleCenterHours;
	Point rectangleCenterMinutes;

	Mat originalImg;

	double tDeg;
	double mDeg;
	double hDeg;

};

