import sys

dictFormat={
    "M": 2,
    "L": 2,
    "V": 1,
    "C": 6,
    "Q": 4
}




class Path:
    def __init__(self, pathStr):
        self.pathData=[]
        #Convert format (M A,B to M A B)
        pathStr.replace(",", " ")
        pathStrList=pathStr.split()
        expectingChar=True
        numArgumentsExpected=0
        tempLst=[]
        tempKey=""
        for i in range(len(pathStrList)):
            if(numArgumentsExpected==0):
                if(pathStrList[i].isalpha()):
                    numArgumentsExpected=dictFormat.get(pathStrList[i])
                    tempKey=pathStrList[i]
                    tempLst=[]
                else:
                    print("error")
            else:
                if(pathStrList[i].isdigit()):
                    numArgumentsExpected-=1
                    tempLst.append(int(pathStrList[i]))
                    if(numArgumentsExpected==0):
                        self.pathData.append([tempKey, tempLst])
                else:
                    print("error")
    def printPath(self):
        print(self.pathData)

x=Path("M 10 80 Q 95 10 180 80")
print(x.printPath())
            
            

def QtoC(startingPoint, qData):
    
    pass



def morphEquivalentLength(path1, path2, fac):
    """
    Aims at converting paths of equivalent length by writing each operation in terms of the other. For example, an La,b in terms of Q is Qa,b a,b a,b and then interpolated.
    """
    i=1
    while(i!=len(path1)):
        if(path1[i]=="Q" and path2[i]=="C"):
            path1[i]=QtoC(path1[i-1][1][-9:], )
            pass
        i+=1


