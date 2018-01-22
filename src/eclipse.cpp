
//Neural network include libraries
#include "../nn_src/yolo_v2_class.hpp"

//System libraries
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
	cout << parent_dir << endl;

	//Directory for input images
	path input_image_dir = parent_dir.append("/Input_Images");
	path output_image_dir = parent_dir.append("/Output_Images");

	cout << input_image_dir << endl;

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

		if(currentFileName.find(".jpg") != string::npos)
		{
			cout << "Reading " << currentFilePath << endl;
			Mat cameraImgBGR = imread(currentFilePath, CV_LOAD_IMAGE_COLOR);
		}
		else
		{
			continue;
		}
	}

//	string  names_file = "data/coco.names";
//	string  cfg_file = "cfg/yolo.cfg";
//	string  weights_file = "yolo.weights";
//
//	Detector detector(cfg_file, weights_file);

	return 0;
}
