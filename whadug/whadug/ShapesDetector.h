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
	//Point triangleCenter;
	
	ShapesDetector(void);
	~ShapesDetector(void);

	Point getTriangleCenter(Mat image);
	//void setLabel(Mat& im, const string label,  vector<Point>& contour);
	void detectRectangles(Mat image);
	void assignCenterPoints(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter);
	void ShapesDetector::setRectangleCenterHours(Point area);
		Point ShapesDetector::getRectangleCenterHours();
	void ShapesDetector::setRectangleCenterMinutes(Point area);
	Point ShapesDetector::getRectangleCenterMinutes();
	bool everythingDetected;

private:
	Point minPt;
	Point hourPt;
	Point centerPt;

	Point rectangleCenterHours;
	Point rectangleCenterMinutes;


	Mat originalImg;

	double tDeg;
	double mDeg;
	double hDeg;

};

