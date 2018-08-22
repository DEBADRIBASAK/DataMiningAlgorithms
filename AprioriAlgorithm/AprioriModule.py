
import copy
import time

class Apriori:
    #this is the implementation of Apriori Algorithm in python 3.6
    
    transactions = {}
    def __init__(self,s='C:\\Users\\USER\\Desktop\\Company Prep\\Data Mining\\Transactions1.txt',n=2):
        self.min_support_count = n
        self.input_file_path = s

    """ A funtion to read from file"""
    def read_from_file(self):
        fobj = open(self.input_file_path,'r')
        cnt = {}
        rtn = set({})
        v = []
        for each in fobj:
            l = list(map(int,each.strip().split(' ')))
            tno = l[0]
            l.pop(0)
            self.transactions[tno] = set({})
            for any in l:
                v.append(any)
                if tuple(v) in cnt:
                    cnt[tuple(v)]+=1
                else:
                    cnt[tuple(v)] = 1
                v.pop(0)
                self.transactions[tno].add(any)
        for items in cnt:
            if cnt[items]>=self.min_support_count:
                rtn.add(items)
        return rtn,cnt

    """A funtion to check whether two sets can be joined or not"""
    def isFeasible(self,s1,s2,s,k,rtn):
        r = True
        s = copy.deepcopy(s1)
        s = s.union(s2)
        if len(s)!=k+1:
            r = False
        else:
            tip = copy.deepcopy(s)
            for i in s:
                tip.remove(i)
                l = list(tip)
                l.sort()
                if tuple(l) not in rtn:
                    r = False
                    break
                else:
                    tip.add(i)
        return r,s

    """A utility function to check whether support count is more than min_support_count"""
    def checkSupportCount(self,rtn):
        s = set({})
        for a in self.transactions:
            for b in rtn:
                d = set(b)
                if d.issubset(self.transactions[a]):
                    rtn[b]+=1
        for c in rtn:
            if rtn[c]>=self.min_support_count:
                s.add(c)
            else:
                pass
        return s,rtn
                
        
    """ A funtion to join two dicts"""
    def join(self,c,k):
        itr = iter(c)
        rtn = {}
        s = set({})
        while itr:
            try:
                pair1 = next(itr)
                itr1 = copy.deepcopy(itr)
                while itr1:
                    try:
                        pair2 = next(itr1)
                        flag,s = self.isFeasible(set(pair1),set(pair2),s,k,c)
                        if flag:
                            s1 = copy.deepcopy(s)
                            l = list(s1)
                            l.sort()
                            rtn[tuple(l)] = 0
                        s.clear()
                    except StopIteration:
                        break
            except StopIteration:
                break
        #print("rtn = ",rtn)
        return self.checkSupportCount(rtn)
    def solve(self,Performance_analysis=True):
        start_time = time.time()
        c,cnt = self.read_from_file()
        print(c)
        p = {}
        k = 1
        while(bool(c)):
            p = c
            print("L",k," :")
            print(len(c))
            c,cnt = self.join(c,k)
            k+=1
        print("final values = ",p)
        end_time = time.time()
        if(Performance_analysis):
            print("Total time taken = ",end_time-start_time)

#sample driver program, remove comment to use
#c = Apriori()
#c.solve()

    
    
        
