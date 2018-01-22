#Feature variables
GPU=1
CUDNN=0
OPENCV=1
DEBUG=1
OPENMP=1
LIBSO=1

#? Just leave it
ARCH= -gencode arch=compute_30,code=sm_30 \
      -gencode arch=compute_35,code=sm_35 \
      -gencode arch=compute_50,code=[sm_50,compute_50] \
      -gencode arch=compute_52,code=[sm_52,compute_52] \
      -gencode arch=compute_61,code=[sm_61,compute_61]

# This is what I use, uncomment if you know your arch and want to specify
# ARCH=  -gencode arch=compute_52,code=compute_52

#Source file paths
VPATH=./nn_src/
EXEC=darknet
OBJDIR=./obj/

#Compile shared object library (Haven't figured this out yet)
ifeq ($(LIBSO), 1)
LIBNAMESO=darknet.so
APPNAMESO=uselib
endif

#Eclipse Name
ECLIPSENAME = eclipse

#Compiler variables
CC=gcc
CPP=g++
NVCC=nvcc 
OPTS=-Ofast
LDFLAGS= -lm -pthread 
COMMON= 
CFLAGS=-Wall -Wfatal-errors

ifeq ($(DEBUG), 1) 
OPTS=-O4 -g -Wall #Note: original code uses -O0. This does not work.
endif

CFLAGS+=$(OPTS)

#If OpenCV is used. Add libraries here.
ifeq ($(OPENCV), 1) 
COMMON+= -DOPENCV
CFLAGS+= -DOPENCV
LDFLAGS+= `pkg-config --libs opencv `

#COMMON += -I/usr/local/include/opencv 
#LDFLAGS+= -lopencv_core #-I may need to be in COMMON
COMMON+= `pkg-config --cflags opencv` 
endif

#Multithreaded processing
ifeq ($(OPENMP), 1)
CFLAGS+= -fopenmp
LDFLAGS+= -lgomp
endif

#GPU Acceleration
ifeq ($(GPU), 1) 
COMMON+= -DGPU -I/usr/local/cuda/include/
CFLAGS+= -DGPU
LDFLAGS+= -L/usr/local/cuda/lib64 -lcuda -lcudart -lcublas -lcurand
endif

#Boost filesystem library
LDFLAGS+= -L/usr/include -lboost_system -lboost_filesystem

ifeq ($(CUDNN), 1) 
COMMON+= -DCUDNN 
CFLAGS+= -DCUDNN -I/usr/local/cudnn/include
LDFLAGS+= -L/usr/local/cudnn/lib64 -lcudnn
endif

#Object files for darknet neural network
OBJ=gemm.o utils.o cuda.o convolutional_layer.o list.o image.o activations.o im2col.o col2im.o blas.o crop_layer.o dropout_layer.o maxpool_layer.o softmax_layer.o data.o matrix.o network.o connected_layer.o cost_layer.o parser.o option_list.o darknet.o detection_layer.o captcha.o route_layer.o writing.o box.o nightmare.o normalization_layer.o avgpool_layer.o coco.o dice.o yolo.o detector.o layer.o compare.o classifier.o local_layer.o swag.o shortcut_layer.o activation_layer.o rnn_layer.o gru_layer.o rnn.o rnn_vid.o crnn_layer.o demo.o tag.o cifar.o go.o batchnorm_layer.o art.o region_layer.o reorg_layer.o super.o voxel.o tree.o

#Additional object files if GPU is enabled
ifeq ($(GPU), 1) 
LDFLAGS+= -lstdc++ 
OBJ+=convolutional_kernels.o activation_kernels.o im2col_kernels.o col2im_kernels.o blas_kernels.o crop_layer_kernels.o dropout_layer_kernels.o maxpool_layer_kernels.o network_kernels.o avgpool_layer_kernels.o
endif

OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard nn_src/*.h) Makefile

all: obj backup results $(EXEC) $(LIBNAMESO) $(APPNAMESO) $(ECLIPSENAME)


#Compile shared library. Use this as a template for how to build custom program
ifeq ($(LIBSO), 1) 
CFLAGS+= -fPIC

$(LIBNAMESO): $(OBJS) nn_src/yolo_v2_class.hpp nn_src/yolo_v2_class.cpp
	$(CPP) -shared -std=c++11 -fvisibility=hidden -DYOLODLL_EXPORTS $(COMMON) $(CFLAGS) $(OBJS) nn_src/yolo_v2_class.cpp -o $@ $(LDFLAGS)
	
$(APPNAMESO): $(LIBNAMESO) nn_src/yolo_v2_class.hpp nn_src/yolo_console_dll.cpp
	$(CPP) -std=c++11 $(COMMON) $(CFLAGS) -o $@ nn_src/yolo_console_dll.cpp $(LDFLAGS) -L ./ -l:$(LIBNAMESO)
endif

#Compile Eclipse
$(ECLIPSENAME): $(LIBNAMESO) nn_src/yolo_v2_class.hpp src/eclipse.cpp
	$(CPP) -std=c++11 $(COMMON) $(CFLAGS) -o $@ src/eclipse.cpp $(LDFLAGS) -L ./ -l:$(LIBNAMESO)

#Used to compile default darknet program
$(EXEC): $(OBJS)
	$(CC) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS)

#Used to compile .o files?
$(OBJDIR)%.o: %.c $(DEPS)
	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@

#Compile cuda files
$(OBJDIR)%.o: %.cu $(DEPS)
	$(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c $< -o $@

obj:
	mkdir -p obj
backup:
	mkdir -p backup
results:
	mkdir -p results

.PHONY: clean

clean:
	rm -rf $(OBJS) $(EXEC) $(LIBNAMESO) $(APPNAMESO) $(ECLIPSENAMESO)

