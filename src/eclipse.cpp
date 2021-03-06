
//Neural network include libraries
#include "../nn_src/yolo_v2_class.hpp"

//System libraries
#include <chrono>
#include <iostream>
#include <string>

//Filesystem library
#include "boost/filesystem.hpp"

//OpenCV Library
#include <opencv2/opencv.hpp> //OpenCV library

using namespace std;
using namespace cv;
using namespace boost::filesystem;

//Forward-declare functions
void printTime(String operation, TickMeter& tm);
void saveImage(const Mat& img, const string path);

int main(int argc, char** argv)
{

	/*----- COLOR VARIABLES -----*/

	// Ranges of colors to look for in HSV color space
	//TODO Get more precise values
	struct tarpData{
		int blue_ideal[3] = {0,0,0};
		int blue_low[3] = {200,45,40};
		int blue_high[3] = {240,100,100};

		int pink_ideal[3] = {0,0,0};
		int pink_low[3] = {290,20,50};
		int pink_high[3] = {350,60,100};

		int yellow_ideal[3] = {0,0,0};
		int yellow_low[3] = {45,20,40};
		int yellow_high[3] = {60,100,100};
	} color_vals;

	/*----- INITIALIZATION -----*/

	//Check if CUDA-enabled GPU can be accessed
	if(cuda::getCudaEnabledDeviceCount() < 1)
	{
		cerr << "No CUDA-enabled GPU detected" << endl;
		return 1;
	}

	//Initialize GPU
	cuda::setDevice(0);
	cuda::resetDevice();

	//Get parent directory
	path parent_dir = argv[0];
	parent_dir = parent_dir.remove_filename();

	//Directory for input images
	path input_image_dir = parent_dir.append("/Input_Images");
	path output_image_dir = parent_dir.append("/Output_Images");

	input_image_dir = input_image_dir.append("/Selected_Images"); //Use this to only test a subset of images

	recursive_directory_iterator end_itr;

	/*----- INITIALIZE NEURAL NETWORK -----*/
	string cfg_file = "./custom_cfg/eclipse_nn.cfg";
	string weight_file = "./backup/eclipse_nn_600.weights"; //TODO: update weights file location
	int gpu_id = 0;

	Detector detector(cfg_file, weight_file, gpu_id);

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

			//TODO: get lighting config file from directory

			continue;
		}

		/*----- READ & CHECK -----*/

		//Import image. imread imports in BGR format.

		//Read only if file is a jpg
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

		//Reduced image dimensions
		double scale = (1.0/4.0);
		int rrows = rows * scale;
		int rcols = cols * scale;

		//Check image exists
		if(cameraImgBGR.empty() == true)
		{
			cout << "No image detected" << endl;
			continue; //Error code that no data was gathered
		}

		printTime("Start Detection", stepTime);
		vector<bbox_t> bboxes = detector.detect(cameraImgBGR); //Run neural network on image
		printTime("Stop Detection", stepTime);

		Scalar color = Scalar(0,0,255);

		for(unsigned int i = 0; i < bboxes.size(); i++)
		{
			rectangle(cameraImgBGR, Rect(bboxes[i].x, bboxes[i].y, bboxes[i].w, bboxes[i].h), color, 20);

		}

		printTime("Draw Rectangles", stepTime);

		//Save image file

		resize(cameraImgBGR,cameraImgBGR,Size(),0.25,0.25,INTER_LINEAR);

		saveImage(cameraImgBGR, currentFilePath);

		printTime("Save Image", stepTime);

		cout << endl << endl;


	}

	/*----- EXIT PROGRAM -----*/

	cout << "Finished" << endl;

	return 0;
}

/*---------- CUSTOM FUNCTIONS ----------*/


void saveImage(const Mat& img, const string path)
{
	size_t index = 0;
	string writePath = path;
	index = writePath.find("Input", index);
	writePath.replace(index,5,"Output"); //Replace "Input" with "Output
	cout << writePath << endl;
	imwrite(writePath,img);

}

//Output elapsed time since last printTime() operation. Useful for determining runtime of given step.
void printTime(String operation, TickMeter& tm)
{
	tm.stop();
	cout << operation << ": "  << tm.getTimeMilli() << " ms" << endl;
	tm.reset();
	tm.start();
	return;
}
