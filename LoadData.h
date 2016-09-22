// This Class is to Load training or test data from a .csv file.
// Note as C/C++ is not suitable for dealing with reading data from files,
// We will use a .py script to convert the '.skdepth.cropping' and '.shand' files
// to a single .csv file, where each line of the .csv contains the 90*60 depth
// values and the 20*3 joints 3D position.

#ifndef LOADDATA_H
#define LOADDATA_H

#include "common.h"

class LoadData {
public:
	LoadData();
    /*
        The construction method of the class, takes two input variables:
        'fileName' is the input file name, for example 'train.csv'.
        'dataSet' has two possible values, 'train' and 'test', 
            which implies the data loaded is with label or not. 
    */
	LoadData(string fileName, string dataSet);
    /*
        Take the 'examples' into 'this->data'
    */
	LoadData(vector<example> examples);
    // TODO: Not implemented yet. Please refer to the paper.
    // This method convert the raw pixel values into example values. 
    // e.g. this->rawData ---> this->data
    // 
    // Possible steps:
    // 1. using the 'Depth feature' in the paper.
    // 2. maybe some centerize work needed.
    // 
    void convertRawDataToExamples();
    /*
	    This method will split the 'this->data' into two part, 'this->trainData' and 'this->testData',
            one for training and another for testing.
        A float num 'rate' in range of  [0,1], standing for the percentage of trainDataSet
	*/
    void splitData(float rate);
    /*
        Return this->data.
    */
    vector<rawDataEachLine> getRawData();
    /*
        Return this->data.
    */
	vector<example> getData();
    /*
        Return this->TrainData.
    */
	vector<example> getTrainData();
    /*
        Return this->TestData.
    */
	vector<example> getTestData();
private:
    // Stores the input data from '.csv' files
    vector<rawDataEachLine> rawData;
    // If this class is loading training data, then 'data' stores training data,
    // Else if is loading test data, then 'data' stores test data.
    // Stores the converted data from rawData.
	vector<example> data;
    // Store the two part by using splitData(), only in 'debug mode'.
	vector<example> trainData, testData;
};

#endif
