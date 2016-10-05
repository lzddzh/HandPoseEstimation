// This Class is to Load training or test data from a .csv file.
// Note as C/C++ is not suitable for dealing with reading data from files,
// We will use a .py script to convert the '.skdepth.cropping' and '.shand' files
// to a single .csv file, where each line of the .csv contains the 90*60 depth
// values and the 20*3 joints 3D position.

#include "LoadData.h"

// Maximum buffer length of reading one line from file.
#define maxLenOneLine 100000

LoadData::LoadData() {}

// Load test data or training data.
// Note that train data set has features and labels while test data has only features. 
// How to use:
// LoadData ld('yourFileName.csv', 'train'); to load data into this->trainData 
// An example file is at trianExample.csv 
LoadData::LoadData(string fileName, string dataSet) {
	if (!(dataSet == "train" || dataSet == "test")) return;
	ifstream fin;
	char buffer[maxLenOneLine];
	float t;
	rawDataEachLine E;
	fin.open((INPUTDIR + fileName).c_str(), ifstream::in);
	fin.getline(buffer, maxLenOneLine);
	for (int i = 0; i < (dataSet == "test" ? testExampleNum : trainExampleNum); i++) {
		int j = 0, pos = 0;
		fin.getline(buffer, maxLenOneLine);
		sscanf(buffer, " %f", &t);
		for (j = 0; j < (dataSet == "test" ? feaNum : feaNum + labelNum); j++) {
			while (pos < maxLenOneLine && buffer[pos++] != ',') {}
			sscanf(buffer + pos, " %f", &E.terms[j]);
		}
		rawData.push_back(E);
	}
	fin.close();
    cout << "Load file successfully from directory: " << (INPUTDIR + fileName).c_str() << endl;
}

LoadData::LoadData(vector<example> examples) {
    data = examples;
}

void LoadData::convertRawDataToExamples() {
    //assume u equals(1000,0)    v equals(0,1000)

    for (int i = 0; i < rawData.size(); i++) {
        example t;
        for (int j = 0; j < feaNum; j++) {
            //current pixel depth
            float depth=rawData[i].terms[j];
            //pixel position
            int x = j / cropingWidth;
            int y = j % cropingWidth;
            //add_u position
            int y_u=y+1000/depth;
            //add_v position
            int x_v=x-1000/depth;
            //depth difference
            if(y_u<cropingWidth&&x_v>=0)
            {
                t.x[j]= rawData[i].terms[x*cropingWidth+y_u]-rawData[i].terms[x_v*cropingWidth+y];
            }
            //out of bound , during training we don't use this feature if any picture is out of bound case.
            else
            {
                t.x[j]=0;
            }
        }

        for (int j = 0; j < labelNum; j++) {
            t.y[j] = rawData[i].terms[j + feaNum];
        }
        data.push_back(t);
    }
}

// Split the input train data into training data and test data.
void LoadData::splitData(float rate) { // float num in range of [0,1]
    if (rate < 0 || rate > 1) return;
    int size = data.size();
    int size1 = size * rate;
    int i;
    for (i = 0; i < size1; i++) trainData.push_back(data[i]);
    for (; i < size; i++) testData.push_back(data[i]);
}

vector<rawDataEachLine> LoadData::getRawData() { return rawData; }

vector<example> LoadData::getData() { return data; }

vector<example> LoadData::getTrainData() { return trainData; }

vector<example> LoadData::getTestData() { return testData; }
