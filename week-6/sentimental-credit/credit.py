from cs50 import get_string

### Visa: 13 or 16 digits, first digit 4
#   American Express_ 15 digits, starts with 34 or 37 
#   Mastercard: 16 digits, starts with two digits in range of 51 to 55###

card_number = get_string("Number: ")

# check if the card number is numeric and not empty
if not card_number.isdigit():
    print("INVALID")
    exit()

sum_even = 0
sum_odds_x2 = 0
length = len(card_number)

# Process every other digit starting from the last
for i in range(length - 2, -1, -2):
    digit = int(card_number[i]) * 2
    sum_odds_x2 += digit // 10 + digit % 10

# Process the remaining digits
for i in range(length - 1, -1, -2):
    sum_even += int(card_number[i])

# check for valid card
if (sum_even + sum_odds_x2) % 10 != 0:
    print("INVALID")
    exit()

# check first two digits
first_digit = int(card_number[0])
first_two_digits = int(card_number[0:2])

# checking card type
if first_digit == 4 and (length == 13 or length == 16):
    print("VISA")
elif (first_two_digits == 34 or first_two_digits == 37) and length == 15:
    print("AMEX")
elif (51 <= first_two_digits <= 55) and length == 16:
    print("MASTERCARD")
else:
    print("INVALID")