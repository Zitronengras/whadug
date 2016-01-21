#include "ShapesDetector.h"

using namespace std;
using namespace cv;

ShapesDetector::ShapesDetector(void)
{
	everythingDetected = false;
	triangleDetected = false;
}


ShapesDetector::~ShapesDetector(void)
{
}

Point ShapesDetector::getTriangleCenter(Mat image) {

    	Mat binaryImage;
    	Point triangleCenter;
    
    	// create binary image with range of the green hsv-value
    	inRange(image, Scalar(30,0,0), Scalar(80,255,255), binaryImage);

	imshow("binaryImg", binaryImage);
    
    	// combination of erosion with dilation
    	morphologyEx(binaryImage, binaryImage, MORPH_OPEN, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    	// finding contours
    	vector<vector<Point>> contours;
    	findContours(binaryImage.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    	vector<Point> approx;

    	for (int i=0; i<contours.size(); i++) {
        
        	// reducing number of points
        	approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true) * 0.05, true);
        
        	// exclude small areas and opening contours
        	if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx)) {
            		continue;
        	}
        
        	cout << "Kanten: " << approx.size() << endl;
        
        	if (approx.size() == 3) {
			Moments M = moments(contours[i]);
			triangleCenter = cv::Point(int(M.m10/M.m00), int(M.m01/M.m00));
			cout << "triangleCenter: " << triangleCenter << endl;
			triangleDetected = true;
			break;
        	}
	}
	
	// triangle ist not detected, take the biggest white area 
	if (!triangleDetected && contours.size() != 0) {
		double biggestArea = contourArea(contours[0]);
		triangleDetected = true;

		for(int i = 1; i < contours.size(); i++){
			if(contourArea(contours[i]) > biggestArea){
				biggestArea = contourArea(contours[i]);
				Moments M = moments(contours[i]);
				triangleCenter = cv::Point(int(M.m10/M.m00), int(M.m01/M.m00));
			}
		}
	} 

    	return triangleCenter;
}

void ShapesDetector::detectRectangles(Mat image){
 
	Mat imgThresholded;

	inRange(image, Scalar(0,100,0), Scalar(20, 255, 255), imgThresholded); //Threshold the image

	// remove small objects from the foreground
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
	dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	
	// find contours
	vector< vector< Point> > contours;
	findContours(imgThresholded.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	vector<Point> approx;
	vector<double> detectedAreas;
	vector<Point> centerPointArray;

	for (int i = 0; i < contours.size(); i++){
		// approximate contour with accuracy proportional to the contour perimeter
		approxPolyDP( Mat(contours[i]), approx, arcLength( Mat(contours[i]), true)*0.02, true);

		if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx)) {
			continue;
		}

		cout << "Rechteckkanten: " << approx.size() << endl;

		if (approx.size() == 4) {
			Moments M = moments (contours[i]);
			detectedAreas.push_back(fabs(contourArea(contours[i])));
			centerPointArray.push_back(Point((M.m10/M.m00),(M.m01/M.m00)));
			cout << detectedAreas.size() <<endl;
			if(detectedAreas.size() == 2) {	
					assignCenterPoints(detectedAreas[0], detectedAreas[1], centerPointArray[0], centerPointArray[1]);
					centerPointArray.clear();
					detectedAreas.clear();
					everythingDetected = true;
					break;
			} else {
				everythingDetected = false;
			}
		} else {
			everythingDetected = false;
			continue;
		}
	}	

	imshow("Thresholded Image", imgThresholded); // show the thresholded image
	
 }

void ShapesDetector::assignCenterPoints(double areaOne, double areaTwo, Point areaOneCenter, Point areaTwoCenter){

	double smallArea;
	double bigArea;
	Point centerMinutes, centerHours;

		if (areaOne < areaTwo) {
			smallArea = areaOne;
			centerHours = areaOneCenter;
			setRectangleCenterHours(areaOneCenter);

			bigArea = areaTwo;
			centerMinutes = areaTwoCenter;
			setRectangleCenterMinutes(areaTwoCenter);

			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			cout << "areaOne < areaTwo" << getRectangleCenterMinutes() << " -- "<< getRectangleCenterHours() << endl;
		}
		else if (areaOne > areaTwo) {
			smallArea = areaTwo;
			centerHours = areaTwoCenter;
			setRectangleCenterHours(areaTwoCenter);

			bigArea = areaOne;
			centerMinutes = areaOneCenter;
			setRectangleCenterMinutes(areaOneCenter);
	
			cout << "Grosser Zeiger: " << bigArea << " \nKleiner Zeiger: " << smallArea << endl;
			cout << "Grosser Zeiger Center: " << centerMinutes << " \nKleiner Zeiger Center: " << centerHours << endl;
			cout << "areaOne > areaTwo" << getRectangleCenterMinutes() << " -- "<< getRectangleCenterHours() << endl;
		}
		else {
			cout << "Fehler bei Vergleich der Flächeninhalte" << endl;
		}
}

void ShapesDetector::setRectangleCenterHours(Point area){
	rectangleCenterHours = area;
}

Point ShapesDetector::getRectangleCenterHours(){
	return rectangleCenterHours;
}

void ShapesDetector::setRectangleCenterMinutes(Point area){
	rectangleCenterMinutes = area;
}

Point ShapesDetector::getRectangleCenterMinutes(){
	return rectangleCenterMinutes;
}

