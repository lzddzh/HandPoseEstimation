/*
    This program has a Random Forest containing number of Trees who can be trained 
    by training data and make prediction on test data. The Random Forest will then 
    collect the results of all the trees and make the final prediction.
    
    For detailed information, please refer to README.md
    
    By Tony,
    Sep 20.
*/
#include "LoadData.h"
#include "Tree.h"
#include "RandomForest.h"

int main() {
    // For generating random number during the later steps.
	srand(unsigned(time(NULL)));
    // The basic threshold of the trees:
    // Minimum number of examples in a tree node,
    // Maximum treeDepth,
    // Maximum information gain in a node.
    // **********Threshold infoGain not used at present!!!!!!
    float minExampleSetSize = 2, treeDepth = 5, infoGain = 0;
    // Just for shorter variable names.
	float &a = minExampleSetSize , &b = treeDepth, &c = infoGain;
    // Number of trees in the forest. For convenience of debugging, just 3 here.
	int treeNum = 100;
    // Ask user which mode to use.
    // If answer 'r', we will enter real run-time mode, program will 
    //     train on training data set and test on test data set.
    // Else if answer 'd', we will enter debug mode, program will 
    //     train on 70% of the training data and 30% of the training data.
	string input;
	cout << "debug mode or real time mode ? (d/r)";
	cin >> input;
    cout << "\nThreshold:" << endl << "minExampleSetSize = " << a << endl << 
        "treeDepth = " << b << endl << "infoGain = " 
        << c << endl << endl << "Number of Trees: " << treeNum << endl;
    // Our random forest rf as the algorithm suggested.
	RandomForest rf;
    // Record the start time.
	time_t startT = time(NULL);
	if (input == "d" || input == "D") {
		cout << "\nBegin debug mode..." << endl;
        // Load data from training data set and split it to two parts.
	    LoadData trainData("trainExample1.csv", "train");
        trainData.convertRawDataToExamples();
        // Take 70% of the data as training data, 30% as the test data.
	    trainData.splitData(0.7);
        vector<example> t1 = trainData.getTrainData();
        vector<example> t2 = trainData.getTestData();
        // FOR DEGBU only:
        /*for (int i = 0; i < t1.size(); i++) {
            for (int j = 0; j < feaNum; j++) cout << t1[i].x[j] << ",";
            for (int j = 0; j < labelNum; j++) cout << t1[i].y[j] << ",";
            cout << endl;
        }*/
        // Assign training data and test data for our forest.
		rf.setTrainData(t1);
		rf.setTestData(t2);
        // Display the size of training data size and the test set size.
		cout << "Training set size = " << trainData.getTrainData().size() << 
            " Test set size = " << trainData.getTestData().size() << endl;
	}
	else if (input == "r" || input == "R") {
		cout << "\nBegin real run-time mode..." << endl;
        // Load training data from training data set, Load test data from 
        //     test data set.
	    LoadData trainData("train.csv", "train");
		LoadData testData("test.csv", "test");
        trainData.convertRawDataToExamples();
        testData.convertRawDataToExamples();
        vector<example> t1 = trainData.getData();
        vector<example> t2 = testData.getData();
        // Note here we assign the real test data for our forest.
		rf.setTrainData(t1);
		rf.setTestData(t2);
        // Display the size of training data size and the test set size.
		cout << "Training set size = " << trainData.getData().size() << 
            " Test set size = " << testData.getData().size() << endl;
	}
    cout << "\n\n" << endl;
    // Training each tree in the forest by the training data,
    // then use each tree make prediction on the test data
    //    and store the prediction results of each tree. 
    //
    // a, b, c and num are thresholds and the number of trees.
    rf.run(a, b, c, treeNum);
    // resultDebugRaw.csv will have the result from all the trees.
    // This information is useful when debugging.
    string resultRawFileName = (input == "r" || input == "R") ? 
        "resultRawReal.csv" : "resultRawDebug.csv";
    rf.outputResultRaw(resultRawFileName);
    cout << endl << "Prediction results of trees are output into file: " << 
        (OUTPUTDIR + resultRawFileName) << endl;

    rf.vote();
    // result*.csv will have the final result given by the forest.
    string resultFileName = (input == "r" || input == "R") ? 
        "resultReal.csv" : "resultDebug.csv";
    rf.outputResult(resultFileName);
    cout << "Prediction result of the forest is output into file: " << 
        (OUTPUTDIR + resultFileName) << endl << endl;
    // To see our prediction is good or not.
    cout << "CorrectRate: Not Implemented." << rf.CorrectRate() << endl;
    cout << "Total cost: " << time(NULL) - startT << "s" << endl;
	return 0;
}
