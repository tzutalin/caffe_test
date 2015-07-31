 Use Caffe to predict in C++
===============
## Purpose
Include [caffe](https://github.com/BVLC/caffe) to do predict in C++

## Requirement
Enviornment: Ubuntu 14.04

Have [installed and build Caffe successfully](http://caffe.berkeleyvision.org/installation.html)

Download the model:

$ cd to /your/path/caffe

$ scripts/download_model_binary.py models/bvlc_reference_caffenet

## How to build the sample
Set your Caffe path

$ export CAFFE_ROOT=/your/path/caffe

$ make all

## You can build it using Eclipse:
http://tzutalin.blogspot.tw/2015/05/caffe-on-ubuntu-eclipse-cc.html

