#include <iostream>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

double smallArea;
double bigArea;

void compareAreas(double areaOne, double areaTwo){

	//try{
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

int main(){

	cout << "START" << endl;

	double areaOne = 21; // = contourArea(*shape);
	double areaTwo = 22; 
	compareAreas(areaOne, areaTwo);

	system("pause");
	return 0;

}