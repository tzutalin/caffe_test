CAFFE_ROOT ?= /home/darrenl/developments/caffe
all:
	g++ -DCPU_ONLY=1 -I$(CAFFE_ROOT)/include -I/usr/local/include/opencv -I$(CAFFE_ROOT)/distribute/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"TestCaffe.d" -MT"TestCaffe.d" -o "TestCaffe.o" "./src/TestCaffe.cpp"
	g++ -L$(CAFFE_ROOT)/build/lib -L/usr/local/lib -o "TestCaffe"  TestCaffe.o   -lcaffe -lprotobuf -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_features2d

clean:
	rm TestCaffe*
