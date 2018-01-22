UCD Eclipse Target Detection Neural Network.

Installation:

OpenCV and CUDA must be installed.

running "make" to compile the program will not work. Due to the way CUDA linking is handled,
"export PATH=/usr/local/cuda-9.1/bin${PATH:+:${PATH}}"
must be called before the "make" command.
This is implemented in the compile.sh script. To compile the program, go to the Makefile's directory and run:
./compile.sh.

Execution:

This program uses a shared library (.so) object. This object must be linked to when running the program.
To link this library, the run_eclipse.sh script automatically calls the necessary command
LD_LIBRARY_PATH=./:$LD_LIBRARY_PATH
before executing the program.

In general, none of the source files in the nn_src folder need to be edited, unless there are errors or optimization improvements.

Images:

The user must create the necessary Input_Images and Output_Images folders in the parent directory,
and must download all images from the Google Drive folder inside the Input_Images folder. These can be placed in subfolders.
Note: all extensions must be ".jpg"

Training:
Use the darknet executable to train the dataset.
