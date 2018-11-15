#ifndef __DECISIONTREEH
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <map>
using namespace std;
map<string,int> m;
int sze;

int uniqueValues(vector<string> v)
{
    m.clear();
    for(int i=0;i<v.size();i++)
        m[v[i]]++;
    return m.size();
}

double calculateInfo(bool isWeighted=false,int chunk=0,int tot=0)
{
    double TotInfo = 0;
    for(map<string,int>::iterator itr = m.begin();itr!=m.end();itr++)
    {
        if(!isWeighted)
            TotInfo+=((((double)(itr->second))/sze)*(log((((double)(itr->second))/sze)))*(-1));
        else
            TotInfo+=((((double)(itr->second))/sze)*(log((((double)(itr->second))/sze)))*(-1)*((double)chunk/tot));
    }
    return TotInfo;
}

class DataSet
{
public:
    vector<string> attrName;
    vector<vector<string> > values;
    vector<string> classes;
    void print()
    {
        for(int i=0;i<attrName.size();i++)
        {
            cout<<attrName[i]<<" ";
        }
        cout<<endl;
        for(int i=0;i<values.size();i++)
        {
            for(int j=0;j<values[i].size();j++)
            {
                cout<<values[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    bool isAttrEmpty()
    {
        return attrName.empty();
    }
    bool allSame()
    {
        if(!classes.empty())
        {
            for(int i=1;i<classes.size();i++)
            {
                if(classes[i]!=classes[0])
                    return false;
            }
            return true;
        }
        return false;
    }
    string majority()
    {
        uniqueValues(classes);
        string rtn = "";
        int mx = -1;
        for(map<string,int>::iterator itr = m.begin();itr!=m.end();itr++)
        {
            if(itr->second>mx)
            {
                mx = itr->second;
                rtn = itr->first;
            }
        }
        return rtn;
    }
    string bestSplit(DataSet *&d,int &siz,int& l,map<string,int>& indices)
    {
        int sz = classes.size();
        sze = sz;
        uniqueValues(classes);
        double TotInfo = calculateInfo();
        map<string,vector<string> > track;
        double tot,chunk,info,maxGain=-1;
        int ind,dist;
        string atr;
        for(int i=0;i<attrName.size();i++)
        {
            track.clear();
            info = 0;
            for(int j=0;j<values[i].size();j++)
            {
                track[values[i][j]].push_back(classes[j]);
            }
            for(map<string,vector<string> >::iterator itr=track.begin();itr!=track.end();itr++)
            {
                uniqueValues(itr->second);
                info+=calculateInfo(true,(itr->second).size(),classes.size());
            }
            if(TotInfo-info>maxGain)
            {
                maxGain = TotInfo-info;
                ind = i;
                dist = track.size();
                atr = attrName[i];
            }
        }
        siz = dist;
        d = new DataSet[dist];
        for(int i=0;i<dist;i++)
        {
            d[i].values.resize(attrName.size()-1);
        }
        for(int i=0;i<attrName.size();i++)
        {
            if(i!=ind)
            {
                for(int j=0;j<dist;j++)
                {
                    d[j].attrName.push_back(attrName[i]);
                }
            }
        }
        int num = 0;
        map<string,int> mp;
        for(int i=0;i<classes.size();i++)
        {
            if(mp.find(values[ind][i])==mp.end())
            {
                mp[values[ind][i]] = num++;
            }
            int curr = mp[values[ind][i]],k = 0;
            d[curr].classes.push_back(classes[i]);
            for(int j=0;j<attrName.size();j++)
            {
                if(j!=ind)
                {
                    d[curr].values[k++].push_back(values[j][i]);
                }
            }
        }
        indices = mp;
        l = ind;
        return attrName[ind];
    }
};

class Node
{
public:
    bool isLeaf;
    string label;
    int ind,childnum;
    Node *children;
    map<string,int> indices;
    Node()
    {
        isLeaf = false;
        children = NULL;
    }
    void construct(DataSet D)
    {
       // D.print();
        if(D.allSame())
        {
            isLeaf = true;
            label = D.classes[0];
        }
        else if(D.isAttrEmpty())
        {
            label = D.majority();
            isLeaf = true;
        }
        else
        {
            DataSet *d;
            d = NULL;
            int dist;
            label = D.bestSplit(d,dist,ind,indices);
            //cout<<"label = "<<label<<endl;
            childnum = dist;
            children = new Node[dist];
            for(int i=0;i<dist;i++)
            {
              // d[i].print();
                children[i].construct(d[i]);
            }
        }
    }
    void print(int spaces)
    {
        for(int i=0;i<spaces;i++)
            cout<<" ";
        if(isLeaf)
        {
            cout<<"leaf: "<<label<<endl;
        }
        else
        {
            cout<<"node: "<<label<<endl;

            for(int i=0;i<childnum;i++)
                children[i].print(spaces+5);
        }
    }
    void classify(map<string,string> data)
    {
        if(isLeaf)
            cout<<"Predicted class = "<<label<<endl;
        else
            children[indices[data[label]]].classify(data);
    }
};


DataSet read_file(const char* fname)
{
    DataSet D;
    ifstream in;
    in.open(fname);
    string s="",st="";
    int num = 0,cnt;
    getline(in,s);
    stringstream str(s);
    str>>st;
    while(str>>st)
    {
        D.attrName.push_back(st);
        num++;
    }
    num--;
    num--;
    D.attrName.pop_back();
    D.attrName.pop_back();
    D.values.resize(num);
    while(!in.eof())
    {
        getline(in,s);
        stringstream str1(s);
        str1>>st;
        cnt = 0;
        while(cnt<num)
        {
            str1>>st;
            D.values[cnt].push_back(st);
            cnt++;
        }
        str1>>st;
        D.classes.push_back(st);
    }
    return D;
}
#endif
