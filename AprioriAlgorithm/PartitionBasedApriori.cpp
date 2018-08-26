#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <cmath>
#define MIN_SUPPORT 2
#define TRAN_NUM 51
#define CHUNK 3
#define MIN_SUPPORT_PART (int)ceil(((double)MIN_SUPPORT/(double)TRAN_NUM)*cnt)
#define SET_ITR set<int>::iterator
#define MAP_ITR map<set<int>,int>::iterator
#define MAP_ITR2  map<int,set<int> >::iterator
using namespace std;

map<int,set<int> > tran;
map<set<int>,int> final_set;
ifstream in;
bool f;
int cnt;
void print_set(ostream& out,set<int> s);

/**
the following function is used for reading the initial data from the transaction database and prepare L1 and C1
**/
void init()
{
    in.open("Transactions.txt");
}
map<set<int>,int> read_set(ofstream &out)
{
   // exit(0);
    string s="";
    int t,tr;
    cnt = 0;
    map<set<int>,int> rtn,rtn1;
    set<int> v;//,it;
    while(!in.eof()&&cnt<CHUNK)
    {
        getline(in,s);
        //cout<<"s = "<<s<<endl;
        stringstream str(s);
        str>>tr;
        while(str>>t)
        {
            v.insert(t);
            tran[tr].insert(t);
            rtn[v]++;
            v.clear();
        }
        cnt++;
       // cout<<cnt<<endl;
    }
    if(in.eof())
    {
        f = false;
        in.close();
    }
    out<<"C1 :"<<endl;
    for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
    {
        print_set(out,itr->first);
        if(itr->second>=MIN_SUPPORT_PART)
        rtn1[itr->first] = itr->second;
        out<<" = "<<itr->second<<endl;
    }
    return rtn1;
}

/**
utility function for joining and pruning item sets
**/
bool is_feasible(set<int> s1,set<int> s2,set<int> &s,map<set<int>,int> m,int k)
{
    SET_ITR i = s1.begin(),j = s2.begin();
    for(int l=0;l<k-1;l++)
    {
        if(*i!=*j)
            return false;
        s.insert(*i);
        i++;j++;
    }
    if(*i>=*j)
        return false;
    s.insert(*i);
    s.insert(*j);
    i = s.begin();
    set<int> h;
    h = s;
    while(i!=s.end())
    {
        h.erase(*i);
        if(m.find(h)==m.end())
            return false;
        h.insert(*i);
        i++;
    }

    return true;
}

/**
Function for finding the number of occurrences of each item set
**/
void check_support_count(map<set<int>,int> &rtn,ofstream &out)
{
    int t;
    MAP_ITR itr1;
    SET_ITR itr2;
    MAP_ITR2 itr3;
    SET_ITR itr4;
    itr3 = tran.begin();
    while(itr3!=tran.end())
    {
        itr1 = rtn.begin();
        while(itr1!=rtn.end())
        {
            itr2 = (itr1->first).begin();
            itr4 = (itr3->second).begin();
            while(itr2!=(itr1->first).end()&&itr4!=(itr3->second).end())
            {
                if(*itr2==*itr4)
                    {
                        itr2++;itr4++;
                    }
                else if(*itr4<*itr2)
                {
                    itr4++;
                }
                else
                    break;
            }
            if(itr2==(itr1->first).end())
            {
                rtn[itr1->first]++;
            }
            itr1++;
        }
        itr3++;
    }
    map<set<int>,int> rtn1;
    itr1 = rtn.begin();
    while(itr1!=rtn.end())
    {
        print_set(out,itr1->first);
        out<<" = "<<itr1->second<<endl;
        if(itr1->second>=MIN_SUPPORT_PART)
        {
            rtn1[itr1->first] = itr1->second;
        }
        itr1++;
    }
    rtn = rtn1;
}

/**
utility function for joining Lk with itself to make ck and then L(k+1)
**/

map<set<int>,int> join(map<set<int>,int> lk,int k,ofstream &out)
{
    map<set<int>,int> rtn;
    set<int> s;
    MAP_ITR itr1,itr2;
    for(itr1 = lk.begin();itr1!=lk.end();itr1++)
    {
        itr2 = itr1;
        itr2++;
        while(itr2!=lk.end())
        {
            if(is_feasible(itr1->first,itr2->first,s,lk,k))
            {
                rtn[s] = 0;
            }
            s.clear();
            itr2++;
        }
    }
    check_support_count(rtn,out);
    return rtn;
}
/**
Utility function for printing a set
**/
void print_set(ostream &out,set<int> s)
{
    for(SET_ITR itr = s.begin();itr!=s.end();itr++)
    {
        out<<*itr<<" ";
    }
}
/**
Driver program
**/
map<set<int>,int> finalise()
{
    map<set<int>,int> rtn;
    for(MAP_ITR itr = final_set.begin();itr!=final_set.end();itr++)
    {
        if(itr->second>=MIN_SUPPORT)
        rtn[itr->first] = itr->second;
    }
    return rtn;
}
int main()
{
    map<set<int>,int> rtn,prev;
    ofstream out;
    init();
    out.open("output1.txt");
    f = true;
    int track = 1;
    out<<"*********************   for "<<track<<"th part   ************************"<<endl;
    while(f)
    {
        rtn = read_set(out);
        for(int k=1;rtn.size()!=0;k++)
        {
            out<<"L"<<k<<" :"<<endl;
            cout<<"L"<<k<<" : "<<rtn.size()<<endl;
            for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
            {
                print_set(out,itr->first);
                out<<" = "<<itr->second<<endl;
                if(final_set.find(itr->first)==final_set.end())
                final_set[itr->first]=itr->second;
                else
                final_set[itr->first]+=itr->second;
            }
            prev = rtn;
            out<<"C"<<k+1<<" : "<<endl;
            rtn = join(rtn,k,out);
            if(rtn.empty())
                out<<"*********************   for "<<track<<"th part   ************************"<<endl;
        }
        for(MAP_ITR itr = prev.begin();itr!=prev.end();itr++)
        {
            print_set(out,itr->first);
            out<<" = "<<itr->second<<endl;
        }
        tran.clear();
        track++;
    }
    final_set = finalise();
    out<<"******* final results *******"<<endl;
    for(MAP_ITR itr = final_set.begin();itr!=final_set.end();itr++)
        {
            print_set(out,itr->first);
            print_set(cout,itr->first);
            out<<" = "<<itr->second<<endl;
            cout<<" = "<<itr->second<<endl;
        }
    out.close();
    cout<<"size = "<<final_set.size()<<endl;
    return 0;
}

