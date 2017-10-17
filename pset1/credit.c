#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("Number: \n");
    long long cc_number = get_long_long();
    int fst_sum = 0;
    int snd_sum = cc_number % 10;
    
    bool toggle = true;
    for (long long i=100; i< cc_number*10; i = i * 10) {
        int multip_digits = ((cc_number % i) / (i/10))*2;
        int other_numbers = (cc_number%i) /(i/10);
        
        if (toggle == true) {
            if (multip_digits > 9) {
                fst_sum += (multip_digits/10) + (multip_digits%10);
            } else {
                fst_sum += multip_digits;
            }
            toggle = false;
        } else if (toggle == false) {
            snd_sum += other_numbers;
            toggle = true;
        }
    }
    
    int test_number = fst_sum + snd_sum;
    if (test_number%10 == 0) {
        if (cc_number >= 5100000000000000 && cc_number < 5600000000000000) {
            printf("MASTERCARD\n");
        } else if (cc_number >= 340000000000000 && cc_number < 380000000000000) {
            printf("AMEX\n");
        } else {
            printf("VISA\n");
        }
    } else {
        printf("INVALID\n");
    }
}