
#set in and out data file names
IN_FILE = "Data.csv"
OUT_FILE = "Filtered.csv"

#set to True or False
AVG_SAME_LOCATIONS = True;
REMOVE_RETRACTED_POINTS = True;    # filters based on RETRACTED_THRES

RETRACTED_THRES = 16            #in inches



def main():
    
    inF = 0
    try :
        inF = open(IN_FILE, 'r')
    except:
        print("Input file " + IN_FILE + " not found\nTo use a difrent input file name edit IN_FILE at the top of this script")
    outF = open(OUT_FILE, 'w')
    
    









if __name__ == '__main__':
    main()