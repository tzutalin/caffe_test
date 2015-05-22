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
using caffe::BlobProto;

//#define USING_MEMORY_LAYER = 1;
void Log(std::string msg)
{
    std::cout << msg << std::endl;
}

void predict_Using_MemoryDataLayer(Net<float>* const, const Datum&);
void predict_Using_Blob(Net<float>* const, const Datum&);

const int IMAGE_WIDTH = 227;
const int IMAGE_HEIGHT = 227;
// CPU Only : Need to define symbols in preprocessor CPU_ONLY=1
int main()
{
    std::string model_path = "/home/darrenl/models/mit/places205CNN_deploy.prototxt";
    std::string weights_path = "/home/darrenl/models/mit/places205CNN_iter_300000.caffemodel";
    std::string image_path = "/home/darrenl/kitchen.jpg";

#ifdef USING_MEMORY_LAYER
    model_path = "/home/darrenl/workspace/eclipse_clusplus_workspace/TestCaffe/bvlc_reference_deploy_memorylayer.prototxt";
    weights_path = "/home/darrenl/workspace/eclipse_clusplus_workspace/TestCaffe/bvlc_reference_caffenet.caffemodel";
#endif

    // Use CPU only.
    Caffe::set_mode(Caffe::CPU);
    // Initial
    Net<float> *caffe_net = new Net<float>(model_path, caffe::TEST);
    caffe_net->CopyTrainedLayersFrom(weights_path);
    // Assign datum
    Datum datum;
    if (!ReadImageToDatum(image_path, 1, IMAGE_HEIGHT, IMAGE_WIDTH, &datum))
    {
        Log("Read Image fail");
        return -1;
    }

#ifdef USING_MEMORY_LAYER
    predict_Using_MemoryDataLayer(caffe_net, datum);
#else
    predict_Using_Blob(caffe_net, datum);
#endif

    delete caffe_net;
    return 0;
}

void predict_Using_MemoryDataLayer(Net<float>* const caffe_net, const Datum& datum)
{
    // Use MemoryDataLayer
    const boost::shared_ptr<MemoryDataLayer<float> > memory_data_layer =
            boost::static_pointer_cast<MemoryDataLayer<float> >(caffe_net->layer_by_name("data"));
    std::vector<Datum> datum_vector;
    datum_vector.push_back(datum);
    memory_data_layer->AddDatumVector(datum_vector);
    std::vector<Blob<float>*> dummy_bottom_vec;
    float loss;
    const std::vector<Blob<float>*>& result = caffe_net->Forward(
            dummy_bottom_vec, &loss);
    const std::vector<float> probs = std::vector<float>(result[1]->cpu_data(), result[1]->cpu_data() + result[1]->count());
    // Find the index with max prob
    int max_index = -1;
    float max_value;
    for (int index = 0; index != probs.size(); index++)
    {
        if (index == 0)
        {
            max_index = 0;
            max_value = probs[max_index];
            continue;
        }
        // Compare
        if (max_value < probs[index])
        {
            max_value = probs[index];
            max_index = index;
        }
    }
    /**
     * Result : Toilet is 861 Cat is 281, etc..
     **/
    std::stringstream ss;
    ss << "max index: " << max_index;
    Log(ss.str());
}

/**
 * Remember num should be consistent in deploy.protxt
 */
void predict_Using_Blob(Net<float>* const caffe_net, const Datum& datum)
{
    const int NUM = 1;
    // Get the blob
    Blob<float>* blob = new Blob<float>(NUM, datum.channels(), datum.height(), datum.width());
    // Get the blobproto
    BlobProto blob_proto;
    blob_proto.set_num(NUM);
    blob_proto.set_channels(datum.channels());
    blob_proto.set_height(datum.height());
    blob_proto.set_width(datum.width());
    const int data_size = datum.channels() * datum.height() * datum.width();
    int size_in_datum = std::max<int>(datum.data().size(), datum.float_data_size());
    for (int i = 0; i < size_in_datum; ++i) {
        blob_proto.add_data(0.);
    }
    const string& data = datum.data();
    if (data.size() != 0) {
        for (int i = 0; i < size_in_datum; ++i) {
            blob_proto.set_data(i, blob_proto.data(i) + (uint8_t)data[i]);
        }
    }
    // Set data into blob
    blob->FromProto(blob_proto);
    // Fill the vector
    vector<Blob<float>*> bottom;
    bottom.push_back(blob);
    float loss = 0.0;
    const vector<Blob<float>*>& result =  caffe_net->Forward(bottom, &loss);
    float max = 0;
    float max_i = 0;
    for (int i = 0; i < result[0]->count(); ++i) {
        float value = result[0]->cpu_data()[i];
        if (max < value){
            max = value;
            max_i = i;
        }
    }

    std::stringstream ss;
    ss << "Prob: " << max << " index: " << max_i;
    Log(ss.str());
}
