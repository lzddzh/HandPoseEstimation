#ifndef COMMON_H
#define COMMON_H

#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#include<string>
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
#define INPUTDIR "/home/jayd/Desktop/NUS/UncertaintyModel/project/HandPoseEstimation/InputData/" // You can change this value to 'InputData/'
#define OUTPUTDIR "/home/jayd/Desktop/NUS/UncertaintyModel/project/HandPoseEstimation/OutputData/"

// Define the parameters of the input data.
#define feaNum 15*10//30*20 //90*60 // The number of feature in each example.
#define labelNum 20*3 // 20 3D positions of the joints.
#define trainExampleNum 430 // For conveinience of dubugging, we first use only 10 examples.
#define testExampleNum 435
#define splitPointsNum 10 // number of split points in each try of feautures.

#define cropingHeigh 15
#define cropingWidth 10

#define INF 2e9

using namespace std;

// When we read data from a .csv file, a rawDataEachLine stores a line of the data.
// A data line contains 'id, feature0, feature1, ..., feature n, label0, label1, ..., label m',
// Where n is the number of feature, m is the number of labels.
// In our case, n = 90 * 60, m = 20 * 3
struct rawDataEachLine {
    string name;
    float terms[feaNum + labelNum];
}; 
// The input of forest at training time is 'vector<example>'. 
// TODO: edit 'example' as armadillo type.
struct example { 
    string name;
    float x[feaNum];
    float y[labelNum];
};
// feature, we have 90 * 60 feature. We use them by their index 0, 1, 2, ...., 90*60-1.
struct feature {
    int index;
};

#endif
