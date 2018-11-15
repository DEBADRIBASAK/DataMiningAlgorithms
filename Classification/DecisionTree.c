#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
#include "DecisionTree.h"
using namespace std;

int main()
{
    Node tree;
    DataSet D = read_file("Train.txt");
    tree.construct(D);
    cout<<"completed :)"<<endl;
    tree.print(0);
    map<string,string> test;
    test["AGE"] = "senior";
    test["INCOME"] = "low";
    test["STUDENT"] = "yes";
    test["CREDIT_RATING"] = "fair";
    tree.classify(test);
    return 0;
}
