#include <iostream>
#include <opencv2\opencv.hpp>

#include "ClockDetector.h"

using namespace std;
using namespace cv;

ClockDetector clockDetector;
Mat img;
Point circleCenter;

int main(){

	while(true){
		circleCenter = clockDetector.detectClock(img);
		if (waitKey(30) == 27){			//wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
			cout << "esc key is pressed by user" << endl;
			break; 
		}
	}	
}