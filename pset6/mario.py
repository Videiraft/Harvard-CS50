
height = 0

while True:
    print("Height: ", end="")
    height = int(input())
    if height > 0 and height < 24:
        break

for i in range(1, height+1): # print "i"th row
    for j in range(height-i, 0, -1): # Spaces on the left
        print(" ", end="")
    
    for j in range(1, i+1): # Blocks on the left
        print("#", end="")
        
    print ("  ", end="") # Spaces at the center
    
    for j in range(1, i+1):
        print("#") if j == i else print("#", end="") # Blocks on the right side
