#include "Node.h"

/* By Tony, Sep 20.
    Constructor for non-leave nodes.
*/
Node::Node(feature a, int d) {
	this->chosenFeature = a;
	this->depth = d;
	this->subNodes.clear();
}
/* By Tony, Sep 20.
    Constructor for leave nodes. 
*/
Node::Node(example averageLabelValue, int d) {
	this->label = averageLabelValue;
	this->depth = d;
	this->subNodes.clear();
}

Node::Node(vector<example> examples, int d) {
    this->estimateExamples = examples;
    this->depth = d;
    this->subNodes.clear();
}
/* By Tony, Sep 20.
    Add a new branch to this node, and the split poiont value also needed.
    We at most add two branch to the node.
*/
void Node::addNewBranch(Node* s, float sP) {
	this->subNodes.push_back(s);
	this->splitPoint = sP;
}
/* By Tony, Sep 20.
    When given a test example, let the Node make a decision to left or right sub-node.
*/
example Node::makeDecision(example &e) { 
    // If subNodes.size() == 0, then the node is a leaf.
	if (subNodes.size() == 0) return label;
    // Otherwise, goes down.
	float l = e.x[chosenFeature.index];
	if (l <= splitPoint && subNodes.size() > 0) return subNodes[0]->makeDecision(e);
	else if (subNodes.size() > 1) return subNodes[1]->makeDecision(e);
}
/* By Tony, Sep 20.
    Print out the the value of this node.
    For debug only.
*/
void Node::print() {
	if (subNodes.empty()) {
        cout << "label=";
        for (int i = 0; i < this->estimateExamples.size(); i++) 
        	cout << estimateExamples[i].name << ", ";
        cout << endl;
    } else {
        cout << this->chosenFeature.index << endl;
    }
}
// By Tony, Sep 20.
bool Node::isLeave() { return subNodes.empty(); }
// By Tony, Sep 20.
vector<Node*> Node::getSubNodes() { return subNodes; }
// By Tony, Sep 20.
float Node::getSplitPoint() { return splitPoint; }
// By Tony, Sep 20.
Node::~Node() {
	for (int i = 0; i < subNodes.size(); i++) {
		delete subNodes[i];
	}
}
