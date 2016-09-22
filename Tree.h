/*
    This Class Tree contains the root node of the tree.
    We grow the tree and make prediction on a single tree at this class.
    This Class is using Class Node as its tree nodes.
*/
#ifndef DECISIONTREE_H
#define DECISIONTREE_H

#include "common.h"
#include "Node.h"
#include <armadillo>

class Tree {
public:
	Tree();
	void beginLearning();
	void setThreshold(float exampleSize = 5, float depth = 2, float infoGain = 0.2);
	void setTrainData(const vector<example>&);
	void setTestData(const vector<example>&);
	void setFeatures(const vector<feature> &a);
	vector<example> getResult();
	void print();
	~Tree();
private:
	Node* learning(vector<example> &E, const vector<feature> &feature, int depth);
	vector<example> trainData, testData;
	vector<feature> features; // training on a part of all feature.
	vector<example> result; // this size is the same as testData.
	Node *root; // the root node of the tree.
	// Conditions to stop growing tree, avoiding overfitting.
	float infoGainThreshold, exampleSizeThreshold, depthThreshold;
};

#endif
