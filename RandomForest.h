/*
    This class is the forest that perform a number of trees training
        and prediction, then the forest collect all the results and
        let the trees vote the final result.

    In this class, OpenMP was used to enable multi core running.
*/
#ifndef RANDOMFOREST_H
#define RANDOMFOREST_H

#include "common.h"
#include "LoadData.h"
#include "Tree.h"

class RandomForest {
public:
	RandomForest();

	void run(float a, float b, float c, int num); 

	void setResultRaw(vector<vector<example> > r); 

	void setTrainData(vector<example> &trainDataRaw);

	void setTestData(vector<example> &testDataRaw); 

    // Just a naive vote: calculating the average value.
    // TODO: implemente this function correctly.
	void vote();

	void generateRandData(float rate); 

    // TODO: not implemented.
	float CorrectRate(); 

    // Output the trees' results to a file.
	void outputResultRaw(string fileName);

    // Output the final result to a file.
	void outputResult(string fileName);

	vector<example> getResult();

	vector<vector<example> > getResultRaw();

private:
	vector<example> trainDataRaw, testDataRaw;

    // Randomly choose a part of eaxmples for each tree.
	vector<example> trainDataRand;
    
    // Store all the trees prediction results.
    // In the first vector each line is a Tree's result.
    // In the second vecotr each line is a line in the test data. 
	vector<vector<example> > resultRaw;

    // Result after votes.
    // Each line is the result of a test item.
    // result.size() == testDataRaw.size()
	vector<example> result;

    // Return a full or sub set of features.
	vector<feature> chooseFeatures() {
		vector<feature> features;
		for (int i = 0; i < feaNum; i++) {
			features.push_back({i});
		}
		return features;
	}
};

#endif
