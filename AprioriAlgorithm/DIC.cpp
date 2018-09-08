#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#define MAP_ITR map<vector<int>,int>::iterator
#define SET_ITR set<vector<int> >::iterator
#define VEC_ITR vector<int>::iterator
#define MIN_SUPPORT 2
#define CHECKPOINTS 3
#define CHUNK 17
using namespace std;
map<vector<int>,int> prob_freq,prob_infreq,check,helper;
set<vector<int> > conf_freq;
set<int> finalised;
int cnt;
bool first_time = true;
ofstream out("OutOfDIC.txt");

void print_set(ostream &o,vector<int> v,int n)
{
    for(int i=0;i<v.size();i++)
    {
        o<<v[i]<<" ";
    }
    o<<" = "<<n<<endl;
}

void DIC()
{
    ifstream in;
    in.open("Transactions.txt");
    int c = 0,t,i,j;
    string s="";
    vector<int> v,v1;
    MAP_ITR itr1;
    SET_ITR itr2;
    while(!in.eof())
    {
        c = 0;
        while(!in.eof()&&c<CHUNK)
        {
            getline(in,s);
            v.clear();
            stringstream str(s);
            str>>t;
            while(str>>t)
            {
                v.push_back(t);
                v1.clear();
                if(first_time&&finalised.find(t)==finalised.end())
                {
                    v1.push_back(t);
                    if(prob_freq.find(v1)!=prob_freq.end())
                        prob_freq[v1]++;
                    else
                    {
                        prob_infreq[v1]++;
                        if(check.find(v1)==check.end())
                            check[v1] = cnt;
                    }
                    v1.pop_back();
                }
            }
                first_time = false;
                itr1 = prob_infreq.begin();
                while(itr1!=prob_infreq.end())
                {
                    i = 0;j = 0;
                  if((itr1->first).size()==1)
                    {
                        first_time = true;
                        itr1++;
                        continue;
                    }
                    while(i<(itr1->first).size()&&j<v.size())
                    {
                        if((itr1->first)[i]==v[j])
                        {
                            i++;j++;
                        }
                        else if((itr1->first)[i]>v[j])
                        {
                            j++;
                        }
                        else
                            break;
                    }
                    if(i==(itr1->first).size())
                        itr1->second++;
                    itr1++;
                }
                itr1 = prob_freq.begin();
                while(itr1!=prob_freq.end())
                {
                  i = 0;j = 0;
                  if((itr1->first).size()==1)
                  {
                      first_time = true;
                      itr1++;
                      continue;
                  }
                    while(i<(itr1->first).size()&&j<v.size())
                    {
                        if((itr1->first)[i]==v[j])
                        {
                            i++;j++;
                        }
                        else if((itr1->first)[i]>v[j])
                        {
                            j++;
                        }
                        else
                            break;
                    }
                    if(i==(itr1->first).size())
                        itr1->second++;
                    itr1++;
                }
                c++;
        }
        cnt = (cnt+1)%CHECKPOINTS;
        itr1 = prob_infreq.begin();
        helper.clear();

        while(itr1!=prob_infreq.end())
        {
           // cout<<"+";
            if(itr1->second>=MIN_SUPPORT)
            {
                prob_freq[itr1->first] = itr1->second;
                /// adding new itemsets

                v1.clear();
                itr2 = conf_freq.begin();
                while(itr2!=conf_freq.end())
                {
                    v1.clear();
                    i = 0;j = 0;
                    while(i<(*itr2).size()&&j<(itr1->first).size())
                    {
                        if((*itr2)[i]==(itr1->first)[j])
                        {
                            v1.push_back((*itr2)[i]);i++;j++;
                        }
                        else if((*itr2)[i]<(itr1->first)[j])
                        {
                            v1.push_back((*itr2)[i]);i++;
                        }
                        else
                        {
                            v1.push_back((itr1->first)[j]);j++;
                        }
                    }
                    while(i<(*itr2).size())
                    {
                        v1.push_back((*itr2)[i]);i++;
                    }
                    while(j<(itr1->first).size())
                    {
                        v1.push_back((itr1->first)[j]);j++;
                    }
                    if(prob_freq.find(v1)==prob_freq.end())
                    helper[v1] = 0;
                    check[v1] = cnt;
                    itr2++;
                   // cout<<"*";
                }
                conf_freq.insert(itr1->first);
            }
            else if(check[itr1->first]!=cnt)
            {
                helper[itr1->first] = itr1->second;
            }
            itr1++;
        }
        prob_infreq = helper;
        if(prob_infreq.size()==0)
        return;
        itr1 = prob_freq.begin();
        helper.clear();
        while(itr1!=prob_freq.end())
        {
            if(check[itr1->first]==cnt)
            {
                if((itr1->first).size()==1)
                    finalised.insert((itr1->first)[0]);
                print_set(out,itr1->first,itr1->second);
            }
            else
            {
                helper[itr1->first] = itr1->second;
            }
            itr1++;
        }
        prob_freq = helper;

    }
}

int main()
{
    cnt = 0;
    DIC();
   int k = 1;
   cout<<k<<endl;
    while(prob_infreq.size()!=0)
    {
        DIC();
        k++;
        cout<<k<<endl;
    }
    out.close();
    return 0;
}
