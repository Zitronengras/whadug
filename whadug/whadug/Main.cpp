#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

double smallArea;
double bigArea;

void compareAreas(float areaOne, float areaTwo){

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

String toString(int number){

	String s;
	stringstream out;
	out << number;
	s = out.str();
	return s;

}

void showTime(int minutes, int hours){
	//just for test
	Mat img = imread("D:/_Daten_Carolin/_HAW/7_IMP/whadug/whadug/Blume.jpg", CV_LOAD_IMAGE_COLOR);

	String label = toString(hours) + ":" + toString(minutes);

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
	
	namedWindow("Uhrzeit");
	imshow("Uhrzeit", img);	
}

void main(){

	float areaOne = 21.5;
	float areaTwo = 22.0; 
	compareAreas(areaOne, areaTwo);

	int minutes = 58;
	int hours = 23;
	showTime(minutes, hours);

	waitKey(0);
}