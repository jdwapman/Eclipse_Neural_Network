
//Neural network include libraries
#include "../nn_src/yolo_v2_class.hpp"

//System libraries
#include <iostream>
#include <string>

//Filesystem library
#include "boost/filesystem.hpp"

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{

	path parent_dir = argv[0];
	parent_dir = parent_dir.remove_filename();
	cout << parent_dir << endl;

//	string  names_file = "data/coco.names";
//	string  cfg_file = "cfg/yolo.cfg";
//	string  weights_file = "yolo.weights";
//
//	Detector detector(cfg_file, weights_file);

	return 0;
}
