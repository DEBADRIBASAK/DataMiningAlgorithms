// a small program to generate random transactional database :)
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <set>
using namespace std;
int main()
{
    srand(time(0));
    int no_of_transaction = 1+(((int)rand())*175)%50;
    while(no_of_transaction<=10)
    {
        no_of_transaction = 1+((int)rand()*175)%50;
        //cout<<no_of_transaction<<endl;
    }
    cout<<no_of_transaction<<endl;
    int no_of_items = 1+((int)rand()*243)%25;
    while(no_of_items<=10)
    {
        no_of_items = 1+((int)rand()*243)%25;
    }
    cout<<no_of_items<<endl;
    int start = 1+((int)rand()*243)%100;
    int sz,st;
    set<int> items;
    while(items.size()!=no_of_items)
    {
        st = 1+((int)rand()*2159)%200;
        cout<<st<<endl;
        items.insert(st);
    }
    ofstream out;
    out.open("Transactions.txt");
    set<int> s;
    set<int>::iterator itr;
    for(int i=start;i<start+no_of_transaction;i++)
    {
        out<<i<<" ";
        //srand(time(0));
        st = 1+((int)rand()*1097)%100;
        itr = items.begin();
        while(st--)
        {
            itr++;
            if(itr==items.end())
                itr=  items.begin();
        }
        sz = 1+((int)rand()*1307)%50;
        while(sz--)
        {
            s.insert(*itr);
            itr++;
        }
        for(set<int>::iterator itr1 = s.begin();itr1!=s.end();itr1++)
        {
            out<<*itr1<<" ";
        }
        out<<endl;
    }
    out.close();
    return 0;
}
