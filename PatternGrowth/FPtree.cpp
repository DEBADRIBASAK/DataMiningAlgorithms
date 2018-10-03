#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <climits>
#define VRAND vector<int>::RandomAccessIterator
#define MIN_SUPPORT 2
using namespace std;

struct FPTree
{
    int itemNo,frequency;
    vector<FPTree*> children;
    FPTree* next,*parent;
};

map<int,int> m;

struct Header
{
    int itemno,freq;
    FPTree* point;
};

bool comp(Header a,Header b)
{
    return a.freq>b.freq;
}

Header* hh = NULL;
int num;

void addToHeader(int item,FPTree* f,Header* head,int n)
{
    int i;
    for(i=0; i<n; i++)
    {
     //   cout<<"i = "<<i+1<<endl;
        if(head[i].itemno==item)
            break;
    }
    if(head[i].point==NULL)
    {
        head[i].point = f;
      //  cout<<"-----";
    }
    else
    {

        FPTree* q;
        q = head[i].point;
        while(q->next!=NULL)
            q = q->next;
        q->next = f;
    }
}

void Insert(FPTree* &t,vector<int> v,int ind,bool cond=false,int frq=0,Header* h=hh,int n=num)
{
    if(ind==v.size())
        return;
    bool f = false;
    FPTree* ch;
    for(int i=0; i<(t->children).size(); i++)
    {
        if(v[ind]==(t->children)[i]->itemNo)
        {
            ch = (t->children)[i];
            if(!cond)
                ch->frequency++;
            else
                ch->frequency+=frq;
            f = true;
            break;
        }
    }
    if(!f)
    {
        ch = NULL;
        ch = new FPTree;
        ch->itemNo = v[ind];
        if(!cond)
            ch->frequency = 1;
        else
            ch->frequency = frq;
        ch->next = NULL;
        ch->parent = t;
     //   cout<<"adding"<<endl;
        addToHeader(ch->itemNo,ch,h,n);
        (t->children).push_back(ch);
    }
    Insert(ch,v,ind+1,cond,frq,h,n);
}

void init(Header* &h,int& n)
{
    ifstream in;
    in.open("Transactions.txt");
    string s="";
    int t;
    while(!in.eof())
    {
        getline(in,s);
        stringstream str(s);
        str>>t;
        while(str>>t)
        {
            m[t]++;
        }
    }
    in.close();
    int sz = 0;

    map<int,int>::iterator itr = m.begin();
    while(itr!=m.end())
    {
        if(itr->second>=MIN_SUPPORT)
            sz++;
        itr++;
    }

    h = new Header[sz];
    n = sz;

    int i=0;
    itr = m.begin();

    while(itr!=m.end())
    {
        if(itr->second>=MIN_SUPPORT)
        {
            h[i].itemno = itr->first;
            h[i].freq = itr->second;
            h[i].point = NULL;
            i++;
        }
        itr++;
    }

    sort(h,h+sz,comp);
}

bool Comp1(int a,int b)
{
    return m[a]>m[b];
}

FPTree* makeFPTree()
{
    FPTree *tree = NULL;
    tree = new FPTree;
    tree->itemNo = -1;
    tree->frequency = 0;
    tree->parent = NULL;
    tree->next = NULL;
    ifstream in;
    in.open("Transactions.txt");
    string s="";
    int t;
    vector<int> v;
    while(!in.eof())
    {
        getline(in,s);
        stringstream str(s);
        str>>t;
        while(str>>t)
        {
            if(m[t]>=MIN_SUPPORT)
            {
                v.push_back(t);
            }
        }
        sort(v.begin(),v.end(),Comp1);

        Insert(tree,v,0);
        v.clear();
    }
    in.close();
    return tree;
}

void print(FPTree* t,int s)
{
    if(t!=NULL)
    {
        for(int i=0; i<s; i++)
        {
            cout<<"  ";
        }
        cout<<t->itemNo<<":"<<t->frequency<<endl;
        for(int i=0; i<(t->children).size(); i++)
        {
            print((t->children)[i],s+2);
        }
    }
}

int noOfBranches(FPTree* t)
{
    if((t->children).size()==0)
        return 1;
    int n=0;
    for(int i=0; i<(t->children).size(); i++)
    {
        n+=(noOfBranches((t->children)[i]));
    }
    return n;
}

// the following function is only used when there is a single path

vector<int> TreeToArray(FPTree* t)
{
    FPTree* q = (t->children)[0];
    vector<int> v;
    while(q!=NULL)
    {
        v.push_back(q->itemNo);
        if((q->children).size()==0)
            q = NULL;
        else
            q = (q->children)[0];
    }
    return v;
}

FPTree* makeConditionalFPTree(Header hd,Header* &dum,int& nn)
{
    FPTree* q,*rtn,*p;
    q = hd.point;
    int k;
    rtn = NULL;
    rtn = new FPTree;
    rtn->itemNo = -1;
    rtn->parent = NULL;
    rtn->next = NULL;
    rtn->frequency = 0;
    m.clear();
    while(q!=NULL)
    {
        p = q->parent;
        k = q->frequency;
        while(p->itemNo!=-1)
        {
            m[p->itemNo]+=k;
            p = p->parent;
        }
        q = q->next;
    }
    Header *head = NULL;
    map<int,int>::iterator itr = m.begin();
    int sz = 0,j=0;
    while(itr!=m.end())
    {
        if(itr->second>=MIN_SUPPORT)
            sz++;
        itr++;
    }
    if(sz==0)
    {
        dum = NULL;
        nn = 0;
        return rtn;
    }
    head = new Header[sz];
    nn = sz;
    itr = m.begin();
    while(itr!=m.end())
    {
        if(itr->second>=MIN_SUPPORT)
        {
            head[j].itemno = itr->first;
            head[j].freq = itr->second;
            head[j].point = NULL;
            j++;
        }
        itr++;
    }
    sort(head,head+sz,comp);
    dum = head;
    q = hd.point;
    while(q!=NULL)
    {
        vector<int> st;
        k = q->frequency;
        p = q->parent;
        while(p->itemNo!=-1)
        {
            if(m[p->itemNo]>=MIN_SUPPORT)
            {
                st.insert(st.begin(),p->itemNo);
            }
            p = p->parent;
        }
        sort(st.begin(),st.end(),Comp1);
        Insert(rtn,st,0,true,k,head,sz);
        q = q->next;
    }
    return rtn;
}


ofstream out("OutOfFPTree.txt");

void MineFPTree(FPTree* t,Header *h,int n,vector<int> pattern,int ff)
{
    if(noOfBranches(t)==1)
    {
        // generate all combinations
        if(n==0)
        {
            vector<int> v1;
           // mn = INT_MAX;
            for(int i=0; i<pattern.size(); i++)
            {
                	v1.push_back(pattern[i]);
            }
            sort(v1.begin(),v1.end());
            for(int ll=0;ll<v1.size();ll++)
                out<<v1[ll]<<" ";
            if(pattern.size()>0)
            {
                out<<" = "<<ff<<endl;
            }
            return;
        }

        vector<int> v = TreeToArray(t),v1;
        int nn = v.size(),mn = INT_MAX;
        int mask = (1<<nn)-1;
        for(int i=((pattern.size()==1)?0:1); i<=mask; i++)
        {
            	v1.clear();mn = INT_MAX;
            for(int j=0; j<nn; j++)
            {
                if(i&(1<<j))
                {
                    	v1.push_back(v[j]);
                    	if(mn>m[v[j]])
                            mn = m[v[j]];
                }
            }
            for(int i=0; i<pattern.size(); i++)
            {
                	v1.push_back(pattern[i]);
            }
            mn = min(ff,mn);
            sort(v1.begin(),v1.end());
            for(int ll = 0;ll<v1.size();ll++)
                out<<v1[ll]<<" ";
            out<<" = "<<mn<<endl;
        }
    }
    else
    {
        int mn = INT_MAX;
        vector<int> v1;
        for(int i=n-1; i>=0; i--)
        {
            Header *conHead = NULL;
            int conN;
            FPTree *cont = NULL;
            cont = makeConditionalFPTree(h[i],conHead,conN);
            pattern.insert(pattern.begin(),h[i].itemno);
            MineFPTree(cont,conHead,conN,pattern,h[i].freq);
            pattern.erase(pattern.begin());
            delete[] conHead;
            conHead = NULL;
        }
    FPTree* rtn = NULL;
    rtn = new FPTree;
    rtn->itemNo = -1;
    rtn->parent = NULL;
    rtn->next = NULL;
    rtn->frequency = 0;
    MineFPTree(rtn,NULL,0,pattern,ff);
    }
}

int main()
{

    init(hh,num);
    for(int i=0; i<num; i++)
    {
        cout<<hh[i].itemno<<" : "<<hh[i].freq<<endl;
    }
    FPTree* t;
    t = makeFPTree();
    print(t,0);
    vector<int> pattern;
    MineFPTree(t,hh,num,pattern,num);
    out.close();
    return 0;
}



