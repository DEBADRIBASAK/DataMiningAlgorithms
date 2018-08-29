#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <string>
#include <cmath>
#define MIN_SUPPORT 2
#define TRAN_NUM 51
#define CHUNK 3
#define MIN_SUPPORT_PART (int)ceil(((double)MIN_SUPPORT/(double)TRAN_NUM)*cnt)
#define vector_ITR vector<int>::iterator
#define VEC_ITR vector<vector<int> >::iterator
#define SET_ITR set<int>::iterator
#define MAP_ITR map<vector<int>,int>::iterator
#define MAP_ITR2  map<int,vector<int> >::iterator
using namespace std;

map<int,vector<int> > tran;
map<vector<int>,int> final_vector;
ifstream in;
bool f;
int cnt;
void print_vector(ostream& out,vector<int> s);


void init()
{
    in.open("Transactions1.txt");
}

/**
the following function is used for reading the initial data from the transaction database and prepare L1 and C1
**/


vector <vector<int> > read_vector(ofstream &out)
{
    string s="";
    int t,tr;
    cnt = 0;
    set<int> v;
    while(!in.eof()&&cnt<CHUNK)
    {
        getline(in,s);
        stringstream str(s);
        str>>tr;
        while(str>>t)
        {
            v.insert(t);
            tran[tr].push_back(t);
        }
        cnt++;
    }
    if(in.eof())
    {
        f = false;
        in.close();
    }
    vector<vector<int> > rtn(v.size());
    int ind = 0;
    for(SET_ITR itr = v.begin();itr!=v.end();itr++)
    {
        rtn[ind++].push_back(*itr);
    }
    return rtn;
}

/**
Function for finding the number of occurrences of each item vector
**/

map<vector<int>,int> check_support_count(vector<vector<int> > rtn,ofstream &out)
{
    int t;
    int itr1,itr2,itr4;
    MAP_ITR2 itr3;
    itr3 = tran.begin();
    map<vector<int>,int> rtn1;
    for(itr1 = 0;itr1<rtn.size();itr1++)
    {
        itr3 = tran.begin();
        t = 0;
        while(itr3!=tran.end())
        {
            itr4 = 0;
            itr2 = 0;
            while(itr2!=(rtn[itr1]).size()&&itr4!=(itr3->second).size())
            {
                if(rtn[itr1][itr2]==(itr3->second)[itr4])
                {
                    itr2++;itr4++;
                }
                else if(rtn[itr1][itr2]>(itr3->second)[itr4])
                {
                    itr4++;
                }
                else
                    break;
            }
            if(itr2==(rtn[itr1]).size())
            {
                t++;
            }
            itr3++;
        }
        if(t>=MIN_SUPPORT_PART)
        {
            rtn1[rtn[itr1]] = t;
        }
        print_vector(out,rtn[itr1]);
        out<<" = "<<t<<endl;
    }
   return rtn1;
}

/**
utility function for joining Lk with itself to make ck and then L(k+1)
**/

map<vector<int>,int> join(map<vector<int>,int> lk,int k,ofstream &out)
{
    vector<vector<int> > rtn;
    vector<int> s,h;
    vector<int>::iterator i,j;
    MAP_ITR itr1,itr2;
    int ind1,ind2;bool f = true;
    for(itr1 = lk.begin();itr1!=lk.end();itr1++)
    {
   // cout<<"= ";
        itr2 = itr1;
        itr2++;

        while(itr2!=lk.end())
        {
           f = true;
            for(ind1=0,ind2=0;ind1<(itr1->first).size()-1,ind2<(itr2->first).size()-1;ind1++,ind2++)
            {
                if((itr1->first)[ind1]!=(itr2->first)[ind2])
                {
                    f = false;
                    break;
                }
                s.push_back((itr1->first)[ind1]);
            }
            if(f)
            {
                s.push_back((itr1->first)[ind1]);
                s.push_back((itr2->first)[ind2]);
                i = s.begin();
                int sto;
                h = s;
                j = h.begin();
                while(i!=s.end())
                {
                    sto = *i;
                    j = h.erase(j);
                    if(lk.find(h)==lk.end())
                    {
                        f = false;
                        break;
                    }
                    j = h.insert(j,sto);
                    i++;j++;
                }
                if(f)
                rtn.push_back(s);
            }
            s.clear();
            itr2++;
        }
    }
    return check_support_count(rtn,out);
}


/**
Utility function for printing a vector
**/


void print_vector(ostream &out,vector<int> s)
{
    for(vector_ITR itr = s.begin();itr!=s.end();itr++)
    {
        out<<*itr<<" ";
    }
}
/**
Driver program
**/
int main()
{
    vector<vector<int> > c;
    map<vector<int>,int> rtn,prev;
    ofstream out;
    init();
    out.open("out.txt");
    f = true;
    int track = 1;
    out<<"*********************   for "<<track<<"th part   ************************"<<endl;
    while(f)
    {
        c = read_vector(out);
        rtn = check_support_count(c,out);
        for(int k=1;rtn.size()!=0;k++)
        {
            out<<"L"<<k<<" :"<<endl;
            for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
            {
                print_vector(out,itr->first);
                out<<" = "<<itr->second<<endl;
                final_vector[itr->first]+=itr->second;
            }
            out<<"C"<<k+1<<" : "<<endl;
            rtn = join(rtn,k,out);
            if(rtn.empty()&&f)
                out<<"*********************   for "<<track+1<<"th part   ************************"<<endl;
        }
        tran.clear();
        track++;
    }
    out<<"******* final results *******"<<endl;
    for(MAP_ITR itr = final_vector.begin();itr!=final_vector.end();itr++)
        {
            if(itr->second>=MIN_SUPPORT)
            {
                print_vector(out,itr->first);
                out<<" = "<<itr->second<<endl;
            }
        }
    out.close();
    cout<<"size = "<<final_vector.size()<<endl;
    return 0;
}

