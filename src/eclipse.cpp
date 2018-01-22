
//Neural network include libraries
#include "../nn_src/yolo_v2_class.hpp"

//System libraries
#include <chrono>
#include <iostream>
#include <string>

//Filesystem library
#include "boost/filesystem.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;

int main(int argc, char** argv)
{

	//Get parent directory
	path parent_dir = argv[0];
	parent_dir = parent_dir.remove_filename();

	//Directory for input images
	path input_image_dir = parent_dir.append("/Input_Images");
	path output_image_dir = parent_dir.append("/Output_Images");

	input_image_dir = input_image_dir.append("/Selected_Images");

	recursive_directory_iterator end_itr;

	/*----- PROCESS ALL IMAGES IN FOLDER -----*/
	for (recursive_directory_iterator itr(input_image_dir); itr != end_itr; ++itr)
	{
    	//Path strings
		string currentFilePath = itr->path().string();
		string currentFileName = itr->path().filename().string();


		//If directory, make folder and continue
		if (is_directory(itr->path()))
		{
			size_t index = 0;
			string outputDirectoryPath = currentFilePath;
			index = outputDirectoryPath.find("Input", index);
			outputDirectoryPath.replace(index,5,"Output"); //Replace "Input" with "Output

			create_directory(outputDirectoryPath);

			continue;

		}

		/*----- READ & CHECK -----*/

		//Import image. imread imports in BGR format.

		if(currentFileName.find(".jpg") == string::npos)
			continue;


		cout << "Reading " << currentFilePath << endl;
		Mat cameraImgBGR = imread(currentFilePath, CV_LOAD_IMAGE_COLOR);

		//Start timer
		TickMeter stepTime;
		TickMeter totalTime;
		stepTime.start();
		totalTime.start();

		//Get image dimensions for preallocation. Can eventually replace with constants
		int rows = cameraImgBGR.rows;
		int cols = cameraImgBGR.cols;
		int imgType = cameraImgBGR.type();


	}

//	string  names_file = "data/coco.names";
//	string  cfg_file = "cfg/yolo.cfg";
//	string  weights_file = "yolo.weights";
//
//	Detector detector(cfg_file, weights_file);

	return 0;
}
