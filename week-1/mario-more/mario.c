#include <stdio.h>
#include <cs50.h>

int main(void){
    int height;

    do{
        height = get_int("Height: ");
    }
    while(height < 1 || height > 8);

    for (int row = 0; row < height; row++){
        for (int col = 1; col < height - row; col++){
            printf(" "); //print spaces
        }

        for (int x = -1; x < row; x++){
            printf("#"); //print hashes
        }

        printf("  "); //print gap

        for (int y = -1; y < row; y++){
            printf("#"); //print hashes
        }

        printf("\n");
    }
}