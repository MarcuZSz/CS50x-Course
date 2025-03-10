#include <stdio.h>
#include <cs50.h>

int main (void){

    /*
    Visa: 13 or 16 digits, start with 4
    American Express: 15 digits, start with 34 or 37
    MasterCard: 16 digits, start with 51, 52, 53, 54, or 55
    */

    long credit_card_num = get_long("Number: ");
    int first_digit = 0;
    int second_digit = 0;
    int num_digits = 0;
    int sum_oddsX2 = 0;
    int sum_even = 0;

    while(credit_card_num > 0){
        second_digit = first_digit;
        first_digit = credit_card_num % 10;

        if (num_digits % 2 == 0){
            sum_even += first_digit;
        }
        else{
            int multiplied = first_digit * 2;
            sum_oddsX2 += (multiplied / 10) + (multiplied % 10);
        }

        credit_card_num /= 10;
        num_digits++;
    }

    bool valid_card = (sum_even + sum_oddsX2) % 10 == 0;
    int first_two_digits = (first_digit * 10) + second_digit; //CS50.ai for opinion on this if this would work

    if (first_digit == 4 && (num_digits == 13 || num_digits == 16) && valid_card){
        printf("VISA\n");
    }    
    else if ((first_two_digits == 34 || first_two_digits == 37) && num_digits == 15 && valid_card){
        printf("AMEX\n");
    }
    else if ((first_two_digits >= 51 && first_two_digits <= 55) && num_digits == 16 && valid_card){
        printf("MASTERCARD\n");
    }
    else{
        printf("INVALID\n");
    }
}