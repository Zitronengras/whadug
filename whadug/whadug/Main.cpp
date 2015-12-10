#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

double smallArea;
double bigArea;

void compareAreas(double areaOne, double areaTwo){

		if (areaOne < areaTwo){
			smallArea = areaOne;
			bigArea = areaTwo;
			cout << "Großer Zeiger: " << bigArea << " \n Kleiner Zeiger: " << smallArea << endl;
		}
		else if(areaOne > areaTwo){
			smallArea = areaTwo;
			bigArea = areaOne;
			cout << "Grosser Zeiger: " << bigArea << " \n Kleiner Zeiger: " << smallArea << endl;
		}
		else{
			cout << "Fehler bei Vergleich der Flächeninhalte" << endl;
		}
}

void showTime(String text){

	Mat img = imread("D:/_Daten_Carolin/_HAW/7_IMP/whadug/whadug/Blume.jpg", CV_LOAD_IMAGE_COLOR);


	//addText(const Mat& img, text, Point org, CvFont font);
	putText(img, text, Point(40, 80), FONT_HERSHEY_PLAIN, 2, Scalar(36, 22, 21), 2, 8, false);
	namedWindow("Uhrzeit");
	imshow("Uhrzeit", img);

	
}

void main(){

	double areaOne = 21; // = contourArea(*shape);
	double areaTwo = 22; 
	compareAreas(areaOne, areaTwo);

	//ostringstream time;
	String time = "Uhrzeit";
	//time = "Uhrzeit";
	showTime(time);

	waitKey(0);
}