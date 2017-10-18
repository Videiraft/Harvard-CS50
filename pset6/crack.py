import sys
import crypt

salt = "50"

def main():
    # Check if there is just one argument - the password
    if not (len(sys.argv)) == 2:
        print("Usage: python crack.py hash")
        return 1
        
    inputHash = sys.argv[1]
    genPsw(inputHash)
    
    return 0


#Function to generate all possible passwords with 4 letters, case sensitive
def genPsw(hashTest):
    test1, test2, test3, test4 = [""], ["", ""], ["", "", ""], ["", "", "", ""]
    possChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
    
    for i in range(0, len(possChar)):
        test1[0] = test2[0] = test3[0] = test4[0] = possChar[i]
        testCmp(test1[0], hashTest) #Testing passwords with just one letter
        
        for j in range(0, len(possChar)):
            test2[1] = test3[1] = test4[1] = possChar[j]
            testCmp("".join(test2), hashTest) #Testing passwords with two letters
            
            for h in range(0, len(possChar)):
                test3[2] = test4[2] = possChar[h]
                testCmp("".join(test3), hashTest) #Testing password with three letters
                
                for k in range(0, len(possChar)):
                    test4[3] = possChar[k]
                    testCmp("".join(test4), hashTest) #Testing passwords with four letters
                    
    return 0
    

#Function to compare the given hash with the resulted hash from the possible password generated in genPsw()
def testCmp(strTest, hashT):
    if hashT == crypt.crypt(strTest, salt):
        print(strTest)
        exit()
    else:
        return 1


if __name__ == "__main__":
    main()