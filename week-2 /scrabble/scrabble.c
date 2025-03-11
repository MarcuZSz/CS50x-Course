#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

//score for the letters used in the game
int let_score[] ={1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int calculate_score(string word);

int main (void){

    //getting input from player1 and player2
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    //calculating the score for both players
    int score1 = calculate_score(word1);
    int score2 = calculate_score(word2);

    //printing the winner
    if (score1 > score2){
        printf("Player 1 wins!\n");
    }
    else if(score2 > score1){
        printf("Player 2 wins!\n");
    }
    else{
        printf("Tie!\n");
    }
}

int calculate_score(string word){
    //calculating the score for the word
    //returning the score

    int score = 0;

    for (int i = 0; i < strlen(word); i++){
        if(isupper(word[i])){
            score += let_score[word[i] - 'A'];
        } else if (islower(word[i])){
            score += let_score[word[i] - 'a'];
        }
    }
    return score;
}