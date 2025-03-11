#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool is_numeric(string s);

int main(int argc, string argv[])
{
    // Check if the user provided exactly one command-line argument
    if (argc != 2 || !is_numeric(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    // Convert the key from a string to an integer
    int key = atoi(argv[1]);

    // Get the plaintext from the user
    string plaintext = get_string("plaintext: ");

    // Encrypt the plaintext
    printf("ciphertext: ");
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char c = plaintext[i];

        // Check if the character is an uppercase letter
        if (isupper(c))
        {
            printf("%c", (c - 'A' + key) % 26 + 'A');
        }
        // Check if the character is a lowercase letter
        else if (islower(c))
        {
            printf("%c", (c - 'a' + key) % 26 + 'a');
        }
        // If it's not a letter, print it as is
        else
        {
            printf("%c", c);
        }
    }
    printf("\n");

    return 0;
}

// Function to check if a string is numeric
bool is_numeric(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i]))
        {
            return false;
        }
    }
    return true;
}