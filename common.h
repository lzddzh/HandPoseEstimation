#ifndef COMMON_H
#define COMMON_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<ctime>
#include<stdlib.h>

// Define the number of threads simultanuously running.
#define NUMTHREADS 4

// Define the file read and write directory.
#define INPUTDIR "InputData/" // You can change this value to 'InputData/'
#define OUTPUTDIR "OutputData/"

// Define the parameters of the input data.
#define feaNum 90*60 // The number of feature in each example.
#define labelNum 20*3 // 20 3D positions of the joints.
#define trainExampleNum 10 // For conveinience of dubugging, we first use only 10 examples.
#define testExampleNum 3
#define splitPointsNum 4 // number of split points in each try of feautures.

#define INF 2e9

using namespace std;

// When we read data from a .csv file, a rawDataEachLine stores a line of the data.
// A data line contains 'id, feature0, feature1, ..., feature n, label0, label1, ..., label m',
// Where n is the number of feature, m is the number of labels.
// In our case, n = 90 * 60, m = 20 * 3
struct rawDataEachLine {
    float terms[feaNum + labelNum];
}; 
// The input of forest at training time is 'vector<example>'. 
// TODO: edit 'example' as armadillo type.
struct example { 
    float x[feaNum];
    float y[labelNum];
};
// feature, we have 90 * 60 feature. We use them by their index 0, 1, 2, ...., 90*60-1.
struct feature {
    int index;
};

#endif
