//============================================================================
// Name        : TestCaffe.cpp
// Author      : TzuTaLin
// Version     :
// Copyright   : 2015
// Description : TestCaffe in C++, Ansi-style
//============================================================================
#include <iostream>
#include <caffe/caffe.hpp>
#include <vector>
#include <sstream>
using std::string;
using caffe::Blob;
using caffe::Caffe;
using caffe::Datum;
using caffe::Net;
using caffe::shared_ptr;
using caffe::vector;
using caffe::MemoryDataLayer;

void Log(std::string msg)
{
    std::cout<< msg << std::endl;
}
// CPU Only : Need to define symbols in preprocessor CPU_ONLY=1
int main() {
    std::string model_path = "/home/darrenl/developments/caffe-android-lib/caffe-mobile/jni/caffe/models/bvlc_reference_caffenet/deploy_mobile.prototxt";
    std::string weights_path = "/home/darrenl/developments/caffe-android-lib/caffe-mobile/jni/caffe/models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel";
    std::string image_path = "/home/darrenl/cat.jpg";
    // Use CPU only.
    Caffe::set_mode(Caffe::CPU);
    // Initial
    Net<float> *caffe_net;
    caffe_net = new Net<float>(model_path, caffe::TEST);
    caffe_net->CopyTrainedLayersFrom(weights_path);
    // Assign datum
    Datum datum;
    if (!ReadImageToDatum(image_path, 1, 227, 227, &datum)) {
        Log("Read Image fail");
        return -1;
    }
    // Use MemoryDataLayer
    const boost::shared_ptr< MemoryDataLayer<float> > memory_data_layer = boost::static_pointer_cast< MemoryDataLayer<float> >(caffe_net->layer_by_name("data"));
    std::vector<Datum> datum_vector;
    datum_vector.push_back(datum);
    memory_data_layer->AddDatumVector(datum_vector);
    std::vector<Blob<float>* > dummy_bottom_vec;
    float loss;
    const std::vector<Blob<float>*>& result = caffe_net->Forward(dummy_bottom_vec, &loss);
    const std::vector<float> probs = std::vector<float>(result[1]->cpu_data(), result[1]->cpu_data() + result[1]->count());
    // Find the index with max prob
    int max_index = -1;
    float max_value;
    for (int index = 0; index != probs.size() ; index++) {
        if (index == 0) {
            max_index = 0;
            max_value = probs[max_index];
            continue;
        }
        // Compare
        if (max_value < probs[index]) {
            max_value = probs[index];
            max_index = index;
        }
    }
    /**
     * Result : Toilet is 861 Cat is 281, etc..
     **/
    std::stringstream ss;
    ss << "max index: " <<  max_index;
    Log(ss.str());
    return 0;
}
