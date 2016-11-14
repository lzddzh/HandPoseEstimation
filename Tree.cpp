#include "Tree.h"

/* Some declaration of function prototypies that will be 
     used in the member functions of the class.
*/
void printTree(Node *root, int depth);
feature chooseBestFeature(const vector<feature> &feats, vector<example> &E,
    vector<example> &upSet, vector<example> &downSet, float &sP); 
double H(const vector<example> &E);
inline double infoGain(const vector<example> &E, const vector<example> &upSet,
    const vector<example> &downSet); 
vector<feature> randomChooseFeatures();
example averageLabelValueOfExamples(const vector<example> &E);

/*
   Defination of the memeber functions.
*/

// By Tony, Sep. 20. The constructor of the class.
Tree::Tree() {
	root = NULL;
}
// By Tony, Sep. 20.  The deconstructor of the Tree, release the root node space.
// Before the root node is released, root node will automatically release its sub nodes.
Tree::~Tree() {
	//delete root;
	if (root != NULL)
		root->~Node();
}
// By Tony, Sep20. Below are some class parameter setting methods.
void Tree::setThreshold(float exampleSize, float depth, float infoGain) {
	infoGainThreshold = infoGain;
	exampleSizeThreshold = exampleSize;
	depthThreshold = depth;
}
void Tree::setTrainData(const vector<example> &a) {
	trainData = a;
}
void Tree::setTestData(const vector<example> &a) {
	testData = a;
}
void Tree::setFeatures(const vector<feature> &a) {
	features = a;
}
// By Tony, Sep 20. Begin split the tree from the root node.
void Tree::beginLearning() {
	if (trainData.size() == 0) cout << "Tree's trainData size is empty." << endl;
	if (features.size() == 0) cout << "Tree's training features is empty." << endl;
	else root = learning(this->trainData, this->features, 0);
}
// By Tony, Sep 20. Get the result of prediction of this tree.
vector<example> Tree::getResult() {
	result.clear();
	if (testData.size() == 0) {
		cout << "Tree's test data is empty. Can not get result." << endl;
	}
	for (int i = 0; i < testData.size(); i++) {
		result.push_back(root->makeDecision(testData[i]));
	}
	return result;
}
// By Tony, Sep 20. Print this tree to console by calling a receruit function.
void Tree::print() {
	printTree(root, 0);
}

// By Tony, Sep 20. Core algorithm of decision tree.
Node* Tree::learning(vector<example> &E, const vector<feature> &feats, int depth) {
	int n;
	if (E.size() <= exampleSizeThreshold || depth > depthThreshold || H(E) == -1 * numeric_limits<double>::infinity()) {
        Node *leaf = new Node(averageLabelValueOfExamples(E), depth); return leaf;
    }
	else {
		float splitPoint;
		vector<example> upSet, downSet;
	    vector<feature> feats = randomChooseFeatures();
		feature best = chooseBestFeature(feats, E, upSet, downSet, splitPoint);
		Node *newTree = new Node(best, depth);
		Node *newLeftTree = learning(upSet, feats, depth + 1);
		newTree->addNewBranch(newLeftTree, splitPoint);
		Node *newRightTree = learning(downSet, feats, depth + 1);
		newTree->addNewBranch(newRightTree, splitPoint);
		return newTree;
	}
}


/* Below are some static functions that not belows to the Tree class, 
    but they are used by the member functions of Tree class.*/

// By Tony, Sep 20. Print this tree to the console. For debug use.
void printTree(Node *root, int depth) {
	root->print();
	vector<Node*> subTrees = root->getSubNodes();
	for (int i = 0; i < subTrees.size(); i++) {
		for (int j = 0; j < depth + 1; j++) cout << "\t";
		if (i == 0) cout << "<" << root->getSplitPoint() << ": ";
		else cout << ">" << root->getSplitPoint() << ": ";
		printTree(subTrees[i], depth + 1);
	}
}
// By Tone, Sep 20. This function returns a average value of the labels 
//     in E.
// In the returned 'example', only '.y' will be used.
// Recall that struct example { float x[feaNum]; float y[labelNum]};
example averageLabelValueOfExamples(const vector<example> &E) {
    example ave;
    memset(ave.y, 0, sizeof(ave.y));
    for (int i = 0; i < E.size(); i++) {
        for (int j = 0; j < labelNum; j++) {
            ave.y[j] += E[i].y[j]; 
        }
    }
    for (int i = 0; i < labelNum; i++) ave.y[i] /= E.size();
    return ave;
}
// By Tony, Sep 20.
vector<feature> randomChooseFeatures() {
	int num = int(sqrt(feaNum));
	//int num = feaNum;
	vector<feature> feats;
    for (int i = 0; i < feaNum; i++) {
        feats.push_back({i});
    }
    random_shuffle(feats.begin(), feats.end());
    feats.resize(num);
	return feats;
}
// By Tony, Sep 21. 
// The H(S) function used by infoGain().
// Third party library Armadillo is used for matrix calculation.
// See at: http://arma.sourceforge.net/
double H(const vector<example> &E) {
    // Using Armadillo linear algebra lib.
    // Declare a double number matrix S with size of E.size() * labelNum.
    arma::mat S(E.size(), labelNum);
    // Convert our data structure to the armadillo matrix so we can calculate
    // the 'cov(S)' and the 'det(cov(S))' as in the paper page 5 formular (2).
    //cout << E.size() << endl;
    if (E.size() == 1) return 0;
    for (int i = 0; i < S.n_rows; i++) {
        for (int j = 0; j < S.n_cols; j++) {
            S(i, j) = E[i].y[j];
            // FOR DEBUG ONLY:
            //  cout << E[i].y[j] << ",";
        }
        // FOR DEBUG ONLY:
        // cout << endl;
    }
    //Calculate the covariance matrix of S.
    arma::mat covS = arma::cov(S);
        // FOR DEBUG ONLY:
    // cout << endl << "covS = " << covS << endl;
    for (int i = 0; i < covS.n_rows; i++) covS(i, i) += 0.1;
    //Calculate the determinant of covS.
    double detCS = arma::det(covS);
        // FOR DEBUG ONLY:
    // cout << "detcs = " << detCS << endl;
    // log() is the function 'ln()'
    // M_PI = 3.14159265358979323846
    double c = (labelNum / 2) * (1 + log(2 * M_PI));
    double lg = log(detCS);
    /* FOR DEBUG ONLY
    if (lg == -1 * numeric_limits<double>::infinity()) {
        // if detCS -> 0, then ln(detCS) -> -inf
        // which means the examples labels value are very close. (I guess)
        cout << "detCS = 0 " << endl;
    }
    */
        // FOR DEBUG ONLY:
    //cout << lg / 2+ c << endl;
    //exit (EXIT_FAILURE);
    return lg / 2 + c;
}
// By Tony. Sep 20.
inline double infoGain(const vector<example> &E, const vector<example> &upSet, const vector<example> &downSet) {
    if (H(upSet) == -1 * numeric_limits<double>::infinity() && H(downSet) == -1 * numeric_limits<double>::infinity())
        return numeric_limits<double>::infinity();
    // INF is a fake inf. INF = 2e9
    if (H(upSet) == -1 * numeric_limits<double>::infinity())
        return INF - downSet.size() * H(downSet);  
    if (H(downSet) == -1 * numeric_limits<double>::infinity())
        return INF - upSet.size() * H(upSet);  
    //cout << "size of UPset: " << upSet.size() << "size of DownSet: " << downSet.size() << endl;
    //cout << "H(E) of UpSet: " << H(upSet) << "  H(E) of DownSet: " << H(downSet) << endl;
    return H(E) - (((upSet.size() + 0.0) / E.size()) * H(upSet) + ((downSet.size() + 0.0 )/ E.size() * H(downSet)));
}
int label(const example &e) {
    int last = e.name.size() - 1;
    if ('1' <= e.name[last] <= '9')
        return e.name[last] - '1' + 1;
    if ('A' <= e.name[last] <= 'Z')
        return e.name[last] - 'A' + 11;
    if ('0' == e.name[last])
        return 10; 
    cout << "example name[last] not [1-9] or [A-Z]" << endl;
    exit(0);
}

double H2(const vector<example> &E) {
    int count[50] = {0};
    for (int i = 0; i < E.size(); i++) {
        count[label(E[i])]++;
    }
    double ret = 0;
    for (int i = 0; i < 50; i++) {
        double x = float(count[i])/E.size();
        if (count[i])
            ret += x * log(x);
    }
    return -ret;
}

double infoGain2(const vector<example> &E, const vector<example> &upSet, const vector<example> &downSet) {
    return H2(E) - (((upSet.size() + 0.0) / E.size()) * H2(upSet) + ((downSet.size() + 0.0 )/ E.size() * H2(downSet)));
}

// By Tony. Sep 20.
feature chooseBestFeature(const vector<feature> &feats, vector<example> &E, vector<example> &upSet, vector<example> &downSet, float &sP) {
    using namespace std::chrono;
    // Set maxInfoGain to -inf.
	double maxInfoGain = -1 * numeric_limits<double>::infinity();
	int bestFeature = -1;
	for (int i = 0; i < feats.size(); i++) {
    //auto start = high_resolution_clock::now();
		float min = INF, max = -INF;
		for (int j = 0; j < E.size(); j++) {
			if (E[j].x[feats[i].index] > max) max = E[j].x[feats[i].index];
			if (E[j].x[feats[i].index] < min) min = E[j].x[feats[i].index];
		}
        //cout << " i = " << i << "max = " << max << "min = " << min << endl;
        if (0.01 > max - min && 0.01 > min - max) continue;
        // Try a number of split points.
		double split[splitPointsNum], step = (max - min) / (splitPointsNum + 1);
		for (int j = 0; j < splitPointsNum; j++) split[j] = min + step * (j + 1); 
		for (int j = 0; j < splitPointsNum; j++) {
            //cout << "split[j] = " << split[j] << ", ";
			upSet.clear();
			downSet.clear();
			for (int k = 0; k < E.size(); k++) {
				if (E[k].x[feats[i].index] <= split[j]) {
					upSet.push_back(E[k]);
				}
				else {
					downSet.push_back(E[k]);
				}
			}
            //auto start1 = high_resolution_clock::now();
			double nu = infoGain(E, upSet, downSet);
            //cout <<  "feat id " <<  feats[i].index << " H = (" << H(upSet) << ", " << H(downSet) << ") Upsize = " << upSet.size() << " Downsize = " << downSet.size() << endl;
            //auto end1 = high_resolution_clock::now();
            //float elapsedSeconds1 = duration_cast<duration<float>>(end1-start1).count();
            //cout << "time1 " << elapsedSeconds1 << endl;
            //cout << "Info calculate time: " << infoGainEndTime - infoGainStartTime << endl;
            // For debug only.
            //cout << "nu = " << nu << endl;
            // Get the best feature index. and the split point value. 
			if (maxInfoGain < nu) {
				maxInfoGain = nu;
				sP = split[j];
				bestFeature = feats[i].index;
			}
		}
        //auto end = high_resolution_clock::now();
        //float elapsedSeconds = duration_cast<duration<float>>(end-start).count();
        //cout << "time2 " << elapsedSeconds << endl;
	}
    
	upSet.clear();
	downSet.clear();
    if (bestFeature < 0 || bestFeature >= feaNum) {
        cout << "choose feature erorr: best feature index = " << bestFeature << endl;
        cout << "Exit program.." << endl;
        exit(0);
    }
    cout << "bestFeature = " << bestFeature << endl;
	for (int k = 0; k < E.size(); k++) {
		if (E[k].x[bestFeature] <= sP) {
			upSet.push_back(E[k]);
		}
		else {
			downSet.push_back(E[k]);
		}
	}
    //cout << "H(best) = ";
    //cout << H(upSet) << " ";
    //cout << H(downSet) << endl;
	feature a;
	a.index = bestFeature;
	return a;
}


