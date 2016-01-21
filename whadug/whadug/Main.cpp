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

    // Winkel des Dreiecks in Radians
    double tRad = atan2(yTriDiff, xTriDiff);
    
    // Winkel des Dreiecks in Grad
    triangleDegree = (tRad * 180)/ M_PI;
    
    cout << "tDeg: " << triangleDegree << endl;
    

	double yMinDiff = minCenter.y - circleCenter.y;
	double xMinDiff = minCenter.x - circleCenter.x;
    // Winkel des Minutenzeigers in Radians
    double mRad = atan2(yMinDiff, xMinDiff);
    
    // Winkel des Minutenzeigers in Grad
    minDegree = (mRad * 180) / M_PI;
    
    cout << "mDeg: " << minDegree << endl;
    
    // Drehung der Uhr im Bild rausrechnen über den Winkel des Dreiecks
    minDegree = minDegree - triangleDegree;
    
    cout << "mDeg: " << minDegree << endl;
    
    if (minDegree < -180) {
        minDegree = 360 + minDegree;
    } else if (minDegree > 180) {
        minDegree = minDegree - 360;
    }
    
    cout << "mDeg: " << minDegree << endl;
    
	double yHourDiff = hourCenter.y - circleCenter.y;
	double xHourDiff = hourCenter.x - circleCenter.x;

    // Winkel des Stundenzeigers in Radians
    double hRad = atan2(yHourDiff, xHourDiff);
    
    // Winkel des Stundenzeigers in Grad
    hourDegree = (hRad * 180) / M_PI;
    
    cout << "hDeg: " << hourDegree << endl;
    
    // Drehung der Uhr im Bild rausrechnen über den Winkel des Dreiecks
    hourDegree = hourDegree - triangleDegree;
    
    cout << "hDeg: " << hourDegree << endl;
    
    if (hourDegree < -180) {
        hourDegree = 360 + hourDegree;
    } else if (hourDegree > 180) {
        hourDegree = hourDegree - 360;
    }
    
    cout << "hDeg: " << hourDegree << endl;
    
}

void convertAnglesToTime() {
    
    cout << "Hours: " << hourDegree/30 << endl;
    cout << "Minutes: " << (minDegree/-6) << endl;
    
    // Ist der Winkel des Stundenzeigers positiv, liegt er im Bereich 0-5 h
    if (hourDegree > 0) {
        // Moegliche Winkel: 1-180. Jede Stunde entspricht einem Bereich von 30 Grad.
        // Bei einer einfachen Konvertierung einer Fliesskommazahl zu Integer wird IMMER Abgerundet. Dies machen wir uns hier zunutze.
        hours = hourDegree/30;
    }
    // Ist der Winkel des Stundenzeigers hingegen negativ, liegt er im Bereich 11-6 h
    else {
        // Moegliche Winkel: 0-(-180). Jede Stunde entspricht einem Bereich von 30 Grad.
        // Über den negativen Wert wird das Vorzeichen umgekehrt. Es muessen immer 6 Stunden addiert werden, da
        hours = 12-(hourDegree/-30);
    }
    cout << "Hours: " << hours << endl;
    
    // Ist der Winkel des Minutenzeigers positiv, liegt er im Bereich 0-29 Min
    if (minDegree > 0) {	
        // Moegliche Winkel: 1-180. Jede Minute entspricht einem Bereich von 6 Grad.
        minutes = minDegree/6;
    }
    // Ist der Winkel des Minutenzeigers hingegen negativ, liegt er im Bereich 59-30 Min
    else {
        // Moegliche Winkel: 0-(-180). Jede Minute entspricht einem Bereich von 6 Grad.
        minutes = 60-(minDegree/-6);
		if (minutes == 60) {
			minutes = 0;
		}
    }
    cout << "Minutes: " << minutes << endl;    
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