
cc_input = "";
while True:
    print("Number: ", end="")
    cc_input = input()
    if cc_input.isdigit() == True:
        break

lsDigits = []
fst_sum = 0;
snd_sum = 0;

for digit in cc_input:
    lsDigits.append(int(digit))

for i in range(len(lsDigits)-2, -1, -2):#Iterating and multipling evry other digit by2 starting w/ the number's secnd-to-last digit
    if lsDigits[i] * 2 < 9:
        fst_sum += lsDigits[i] * 2
    else: #if the resulting product is greater than 9 add both digits to the sum (The product of a single digit by2 is always < 19)
        fst_sum += ((lsDigits[i] * 2) % 10 ) + 1

for i in range(len(lsDigits)-1, -1, -2):#Iterating and multipling every other digit by2 starting with the last number's digit
    snd_sum += lsDigits[i]

if (fst_sum + snd_sum) % 10 == 0: #Check if the credit card number is valid and, if True, check which company issues it
    if lsDigits[0] == 5:
        print("MASTERCARD")
    elif lsDigits[0] == 4:
        print("VISA")
    else:
        print("AMEX")
else:
    print("INVALID")

