#include <iostream>
#include <opencv2\opencv.hpp>
#include "math.h"
#define _USE_MATH_DEFINES
#include <math.h>

#include "ClockDetector.h"
#include "ShapesDetector.h"

using namespace std;
using namespace cv;

	ClockDetector clockDetector;	
	ShapesDetector shapesDetector;
	VideoCapture cap;

	Mat img;
	Mat maskedClockImg;

	Point circleCenter;
	Point triangleCenter;
	Point minCenter;
	Point hourCenter;

	double triangleDegree;
	double minDegree;
	double hourDegree;
	
	int hours;
	int minutes;

void calcAngles() {
    

	double yTriDiff = triangleCenter.y- circleCenter.y;
	double xTriDiff = triangleCenter.x- circleCenter.x;

    // The triangles angle in radians
    double tRad = atan2(yTriDiff, xTriDiff);
    
    // The triangles angle in degree
    triangleDegree = (tRad * 180)/ M_PI;
    
	double yMinDiff = minCenter.y - circleCenter.y;
	double xMinDiff = minCenter.x - circleCenter.x;
	
    // The minute hand angle in radians
    double mRad = atan2(yMinDiff, xMinDiff);
    
    // The minute hand angle in degree
    minDegree = (mRad * 180) / M_PI;
    
    // Subtracting the Clocks rotation out of the minute hand angle
    minDegree = minDegree - triangleDegree;
    
    // Fetch angles out of -180 to 180 range
    if (minDegree < -180) {
        minDegree = 360 + minDegree;
    } else if (minDegree > 180) {
        minDegree = minDegree - 360;
    }
    
	double yHourDiff = hourCenter.y - circleCenter.y;
	double xHourDiff = hourCenter.x - circleCenter.x;

    // The hour hand angle in radians
    double hRad = atan2(yHourDiff, xHourDiff);
    
    // The minute hand angle in degree
    hourDegree = (hRad * 180) / M_PI;
    
    // Subtracting the Clocks rotation out of the hour hand angle
    hourDegree = hourDegree - triangleDegree;
    
    // Fetch angles out of -180 to 180 range
    if (hourDegree < -180) {
        hourDegree = 360 + hourDegree;
    } else if (hourDegree > 180) {
        hourDegree = hourDegree - 360;
    }
    
}

void convertAnglesToTime() {
    
    // For positive hour hand angles, the hour value lies in a range of 0-5 h
    if (hourDegree > 0) {
        // Possible angles: 1 to 180 degree. Every hour correlates to a range of 30 degree.
	// Conversions of floating-point numbers to integer are always rounded off
        hours = hourDegree/30;
    }
    // For negative hour hand angles, the hour value lies in a range of 11-6 h
    else {
        // Possible angles: 0 to -180 degree.
        hours = 12+(hourDegree/30);
    }

    // For positive minute hand angles, the minute value lies in a range of 0-29 min
    if (minDegree > 0) {	
        // Possible angles: 1 to 180 degree. Every minute correlates to a range of 6 degree
        minutes = minDegree/6;
    }
    // For negative minute hand angles, the minute value lies in a range of 59-30 min
    else {
        // Possible angles: 0 to -180 degree.
        minutes = 60+(minDegree/6);
		if (minutes == 60) {
			minutes = 0;
		}
    }
}


String toString (int number) {

	String s;
	stringstream out;
	out << number;
	s = out.str();
	return s;

}

void showTime (int minutes, int hours, Mat img) {
	
	String hoursString;
	if (hours < 10) {
		hoursString = "0" + toString (hours);
	} 
	else {
		hoursString = toString (hours);
	}

	String minutesString;
	if (minutes < 10) {
		minutesString = "0" + toString (minutes);
	} 
	else {
		minutesString = toString (minutes);
	}

	String label = hoursString + ":" + minutesString;

	// rectangle for GUI
	Point RectUpLeft = Point(50, 0);
	Point RectDownRight = Point(195, 220);
	rectangle(img, RectUpLeft, RectDownRight, Scalar(40, 40, 40), CV_FILLED, 8, 0);

	//text properties
	int fontface = FONT_HERSHEY_SIMPLEX; //FONT_HERSHEY_PLAIN
    double scale = 1.1;
    int thickness = 1;
    int baseline = 0;

	// center label horizontally
	Size text = getTextSize(label, fontface, scale, thickness, &baseline);
	int textX = (((RectDownRight.x - RectUpLeft.x)/2) + RectUpLeft.x) - (text.width /2);
	// vertical position
	int textY = (RectDownRight.y - (RectDownRight.y / 5));
	//display time
	putText(img, label, Point(textX, textY), fontface, scale, Scalar(215, 215, 215), thickness, 8);

}

int main(){		

	//Camera
	cap = VideoCapture(1); // capture the video from web cam
	
	while (true) {
		if(!cap.isOpened()){ // if not success, exit program
			cout << "Cannot open the web cam" << endl;
		}
		bool bSuccess = cap.read(img); // read a new frame from video	

		// find circleCenter
		circleCenter = clockDetector.detectClock(img);
		maskedClockImg = clockDetector.getMaskedImg();
		
		if(clockDetector.clockDetected){
			//find triangle
			triangleCenter = shapesDetector.getTriangleCenter(maskedClockImg);
			if(shapesDetector.triangleDetected){
				//find rectangle
				shapesDetector.detectRectangles(maskedClockImg);
				if (shapesDetector.everythingDetected) {
					minCenter = shapesDetector.getRectangleCenterMinutes();
					hourCenter = shapesDetector.getRectangleCenterHours();

					calcAngles();
					convertAnglesToTime();
				}
			}					
		}
		
		showTime(minutes, hours, img);
		imshow("Original", img); //show the original image

		if (waitKey(30) == 27){ //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}
}
