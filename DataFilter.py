
#set in and out data file names
IN_FILE = "Data.csv"
OUT_FILE = "DataFiltered.csv"

#set to True or False
AVG_SAME_LOCATIONS = True;
REMOVE_RETRACTED_POINTS = True;    # removes data points that are below RETRACTED_GAUGE_THRES or RETRACTED_FORCE_THRES
PRINT_FILE_OUTPUT = True
PLOT_OUTPUT = True          #not implemented need matplotlib on windows

RETRACTED_GAUGE_THRES = 16          #in inches
RETRACTED_FORCE_THRES = 10         # in lb 


def outputLine(outF, D, F, G):
    outF.write(str(D)+","+str(F)+","+str(G)+"\n")
    if(PRINT_FILE_OUTPUT):
        print(str(D)+","+str(F)+","+str(G))
    
    
def filterData(outF, D, F, G, avgVals):       
    if(REMOVE_RETRACTED_POINTS):
        if(F < RETRACTED_FORCE_THRES or G < RETRACTED_GAUGE_THRES):
            return avgData(outF, D, F, G, avgVals)
    else:
        return avgData(outF, D, F, G, avgVals)
        
def avgData(outF, D, F, G, avgVals):
    lastD, numPts, sumF ,sumG = avgVals
    
    if(AVG_SAME_LOCATIONS):
        if(lastD == D):
            numPts+=1
            sumF+=F
            sumG+=G
        else:
            #check first avg case
            if(lastD != None):
                outputLine(outF, lastD, sumF/numPts, sumG/numPts)
            lastD = D
            numPts = 1
            sumF = F
            sumG = G
    else:
        outputLine(outF, D, F, G)
    return [lastD, numPts, sumF, sumG]
    
    
def parseFile(inF, outF):
    #for AVG_SAME_LOCATIONS
    #last D val, numPts, sum F, sum G
    avgVals = [None, 0, 0, 0]
    
    for line in inF:
        goodLine = False
        D=0
        F=0
        G=0
        try:
            #parse Data (Distance, Force, Gauge)
            D, F, G = [float(val) for val in line.split(",")]
            goodLine = True
        except:
            pass
        if(goodLine):
            avgVals = filterData(outF, D, F, G, avgVals)      
    
    #avg the last line if nessary
    if(AVG_SAME_LOCATIONS):
        outputLine(outF, avgVals[0], avgVals[2]/avgVals[1], avgVals[3]/avgVals[1])

        
def main():
    inF = None
    try :
        inF = open(IN_FILE, 'r')
    except:
        print("Input file " + IN_FILE + " not found\nTo use a difrent input file name edit IN_FILE at the top of this script")
        exit()
    outF = open(OUT_FILE, 'w')
    
    #keep the first line the same
    headerLine = inF.readline()
    outF.write(headerLine)
    if(PRINT_FILE_OUTPUT):
        print(headerLine.split("\n")[0])
        
        
    parseFile(inF, outF)
    
    inF.close()
    outF.close()










if __name__ == '__main__':
    main()