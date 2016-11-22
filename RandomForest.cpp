#include "RandomForest.h"
#include <omp.h>

RandomForest::RandomForest(){}

void RandomForest::run(float a, float b, float c, int num) {
	resultRaw.clear();
    int startT;
    // Now it use only 1 thread whatever you set.
    // Later when the data becomes big, we debug this.
    // We shutdown the parrallel at present.
    int i;
    #pragma omp parallel for num_threads(NUMTHREADS)
    //#pragma omp parallel for num_threads(1)
	for (i = 0; i < num; i++) {
		Tree tree;
		    tree.setThreshold(a, b, c);
            cout << "Number of threads parallel running: " << omp_get_num_threads() << endl;
	        startT = time(NULL);
        #pragma omp critical
        {
		    tree.setTrainData(generateRandData(0.6));
		    tree.setTestData(testDataRaw);
			cout << "test data size:" << testDataRaw.size() << endl;
		    tree.setFeatures(chooseFeatures());
            /*
            for (int j = 0; j < tree.getTrainData().size(); j++) {
                for (int k = 0; k < labelNum; k++) {
                    cout << tree.getTrainData()[j].y[k] << ",";
                }
                cout << endl;
            }*/
            cout << "trianing on " << tree.getTrainDataSize() << " examples." << endl;
        }
		tree.beginLearning();
        #pragma omp critical
        {
			// FOR DEBUG ONLY.
            tree.print();
            tree.getResult();
		    resultRaw.push_back(tree.getResult());

		    cout << "Tree " << i << " cost: " << time(0) - startT << "s" << endl;
        }
	}
}

void RandomForest::setResultRaw(vector<vector<example> > r) {
	this->resultRaw = r;
}

void RandomForest::setTrainData(vector<example> &trainDataRaw) {
	this->trainDataRaw = trainDataRaw;
}

void RandomForest::setTestData(vector<example> &testDataRaw) {
	this->testDataRaw = testDataRaw;
}

// By Tony. Sep 20.
// Just a naive vote: calculating the average value.
// TODO: implemente this function correctly.
void RandomForest::vote() {
	if (resultRaw.size() == 0) {
		cout << "No results for vote." << endl;
		return;
	}
	result.clear();
    // resultRaw[0].size() == number of test items.
    for (int i = 0; i < resultRaw[0].size(); i++) {
        example label;
        for (int j = 0; j < labelNum; j++) {
            double sum = 0;
            // resultRaw.size() == number of trees in forest.
            for (int k = 0; k < resultRaw.size(); k++) {
                // the k th tree, and the j th label
                sum += resultRaw[k][i].y[j];
            }
            label.y[j] = sum / resultRaw.size();
        }
        result.push_back(label);
    }
}

// By Tony. Sep 20.
vector<example> RandomForest::generateRandData(float rate) {
    // Must clear it first, because multiple threads are using it.
/*
    bool used[trainDataRaw.size()];
    memset(used, 0, sizeof(used));
	int size = (int)trainDataRaw.size() * rate;
	for (int i = 0; i < size; i++) {
		int n = rand() % trainDataRaw.size();
        while (used[n]) n = rand() % trainDataRaw.size();
		//cout << n << endl;
		trainDataRand.push_back(trainDataRaw[n]);
        used[n] = true;
	}
*/
    vector<example> trainDataRand;
    trainDataRand = trainDataRaw;
    random_shuffle(trainDataRand.begin(), trainDataRand.end());
    trainDataRand.resize(rate * trainDataRaw.size());
    
    return trainDataRand;
}

// TODO: not implemented.
float RandomForest::CorrectRate() {
    // Not implemented
    return -1;
}

// By Tony. Sep 20.
// Output the trees' results to a file.
void RandomForest::outputResultRaw(string fileName) {
	if (resultRaw.size() == 0) return;
	fstream out;
	out.open((OUTPUTDIR + fileName).c_str(), ifstream::out);
	out << "id";
	for (int i = 0; i < resultRaw[0].size(); i++) {
		out << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,label" << i;
	}
	out << endl;
	for (int i = 0; i < resultRaw[0].size(); i++) {
		out << i;
		for (int j = 0; j < resultRaw.size(); j++) {
            for (int k = 0; k < labelNum; k++){
			    out << "," << resultRaw[j][i].y[k];
            }
		}
		out << endl;
	}
	out.close();
}

// By Tony. Sep 20.
// Output the final result to a file.
void RandomForest::outputResult(string fileName) {
	fstream out;
	out.open((OUTPUTDIR + fileName).c_str(), ifstream::out);
	//out << "id,label" << endl;
	for (int i = 0; i < result.size(); i++) {
        //out << i ;
        out << result[i].y[0];
        for (int j = 1; j < labelNum; j++)
		    out << "," << result[i].y[j];
        out << endl;
	}
	out.close();
}

// By Tony. Sep 20.
vector<example> RandomForest::getResult() { return result; }

// By Tony. Sep 20.
vector<vector<example> > RandomForest::getResultRaw() { return resultRaw; }

