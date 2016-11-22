/*
    This program is a KNN implementation for estimating hand pose from depth image.
    By Yancy.
    Nov 10.
*/
#include "LoadData.h"
#include "Tree.h"
#include "RandomForest.h"


typedef pair<string, int> PAIR;

bool compare(const pair<example, double> &i, const pair<example, double> &j) {
    return i.second > j.second;
}

int main() {
   string result="";
   LoadData trainData("trainData.csv", "train");
   LoadData testData("testData.csv", "test");
   trainData.convertRawDataToExamples();
   testData.convertRawDataToExamples();
   vector<example> t1 = trainData.getData();
   vector<example> t2 = testData.getData();
   for (int i = 0; i < t2.size(); i++) {
       cout<<i<<endl;
       example e = t2[i];
       vector<pair<example, double>> simPair;
       for (int j = 0; j < t1.size(); j++) {
           example t_e = t1[j];
           double similarity = CalSim::CalSim(e.x, t_e.x);
           simPair.push_back(pair<example, double>(t_e, similarity));
       }
       sort(simPair.begin(), simPair.end(), compare);
       for (int j = 0; j < 10; j++) {
           example t_e = simPair[j].first;
           for (int k = 0; k < 60; k++) {
               e.y[k] +=t_e.y[k];
           }
       }

       result+=to_string(e.y[0]/10);
       for(int j=1;j<60;j++)
       {
           result+=","+to_string(e.y[j]/10);
       }
       result+="\n";

   }
   ofstream out;
   out.open("/Users/mac/Documents/knn_without_f_e.csv", ofstream::out);
   out << result;
   out.close();
}
