 Use Caffe to predict in C++
===============
## Purpose
Include [Caffe](https://github.com/BVLC/caffe) to do predict in C++

## Requirement
Have [installed Caffe depedencies successfully](http://caffe.berkeleyvision.org/installation.html)

* Grap the source

`git clone --recursive https://github.com/tzutalin/caffe_test.git`

* Download the model:

`$ cd data; sh download_caffe_model.sh`

* Build Caffe

`$ cd caffe`

`$ cp Makefile.config.example Makefile.config`

`$ vi Makefile.config`

Uncommnet CPU_ONLY := 1

`$ make all ; make distribute`

## How to build the sample

`$ cd $ros_test`

`$ make all`

Usage:

`./TestCaffe deploy.prototxt network.caffemodel mean.binaryproto labels.txt img.jpg`


`$ ./TestCaffe data/deploy.prototxt data/bvlc_reference_caffenet.caffemodel data/imagenet_mean.binaryproto data/synset_words.txt data/cat.jpg `


Output:

	---------- Prediction for data/cat.jpg ----------
	0.1384 - "n02123159 tiger cat"
	0.1302 - "n02123045 tabby, tabby cat"
	0.1138 - "n02085620 Chihuahua"
	0.1076 - "n02094433 Yorkshire terrier"
	0.0914 - "n02123394 Persian cat"


## You can build it using Eclipse
http://tzutalin.blogspot.tw/2015/05/caffe-on-ubuntu-eclipse-cc.html

