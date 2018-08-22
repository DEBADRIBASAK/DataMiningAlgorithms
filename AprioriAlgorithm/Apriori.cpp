#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#define MIN_SUPPORT 2
#define SET_ITR set<int>::iterator
#define MAP_ITR map<set<int>,int>::iterator
#define MAP_ITR2  map<int,set<int> >::iterator
using namespace std;

map<int,set<int> > tran;
void print_set(ostream& out,set<int> s);

/**
the following function is used for reading the initial data from the transaction database and prepare L1 and C1
**/

map<set<int>,int> read_set(ofstream &out)
{
    ifstream in;
    in.open("Transactions.txt");
    string s="";
    int t,tr;
    map<set<int>,int> rtn,rtn1;
    set<int> v,it;
    while(!in.eof())
    {
        getline(in,s);
        stringstream str(s);
        str>>tr;
        while(str>>t)
        {
            v.insert(t);
            it.insert(t);
            rtn[v]++;
            v.clear();
        }
        tran[tr] = it;
        it.clear();
    }
    in.close();
    out<<"C1 :"<<endl;
    for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
    {
        print_set(out,itr->first);
        if(itr->second>=MIN_SUPPORT)
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
        if(itr1->second>=MIN_SUPPORT)
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
int main()
{
    map<set<int>,int> rtn,prev;
    ofstream out;
    out.open("output.txt");
    rtn = read_set(out);
    for(int k=1;rtn.size()!=0;k++)
    {
        out<<"L"<<k<<" :"<<endl;
        cout<<"k = "<<k<<" size = "<<rtn.size()<<endl;
        for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
        {
            print_set(out,itr->first);
            out<<" = "<<itr->second<<endl;
        }
        prev = rtn;
        out<<"C"<<k+1<<" : "<<endl;
        rtn = join(rtn,k,out);
    }
    out<<"frequent itemsets are"<<endl;
    rtn = prev;
    for(MAP_ITR itr = rtn.begin();itr!=rtn.end();itr++)
        {
            print_set(out,itr->first);
            print_set(cout,itr->first);
            out<<" = "<<itr->second<<endl;
            cout<<" = "<<itr->second<<endl;
        }
    out.close();
    return 0;
}
