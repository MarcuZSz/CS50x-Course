#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/*
Programm should print:
before Grade 1
Grade X equivalent to X
and Grade 16+
*/

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void){
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    double L = (letters / (double) words) * 100;
    double S = (sentences / (double) words) * 100;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // Print the grade level
    if (index < 1){
        printf("Before Grade 1\n");
    }
    else if (index > 16){
        printf("Grade 16+\n");
    }
    else{
        printf("Grade %i\n", index);
    }
}

int count_letters(string text){
    // Count the number of letters in the text
    int total_letters = 0;
    for (int i = 0; i < strlen(text); i++){
        if (isalpha(text[i])){
            total_letters++;
        }
    }
    return total_letters;
}

int count_words(string text){
    // Count the number of words in the text
    int total_words = 0;

    for (int i = 0; i < strlen(text); i++){
        if (isspace(text[i])){
            total_words++;
        }
    }
    return total_words + 1;
}

int count_sentences(string text){
    // Count the number of sentences in the text
    int total_sentences = 0;

    for (int i = 0; i < strlen(text); i++){
        if (text[i] == '.' || text[i] == '!' || text[i] == '?'){
            total_sentences++;
        }
    }
    return total_sentences;
}