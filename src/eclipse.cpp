
//Neural network include libraries
#include "../nn_src/yolo_v2_class.hpp"

//System libraries
#include <iostream>
#include <string>

//Filesystem library
#include "boost/filesystem.hpp"

using namespace std;

int main()
{

	string  names_file = "data/coco.names";
	string  cfg_file = "cfg/yolo.cfg";
	string  weights_file = "yolo.weights";
	
	Detector detector(cfg_file, weights_file);

	cout << "Hi World!" << endl;
	return 0;
}
