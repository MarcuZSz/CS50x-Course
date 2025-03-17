#include <stdio.h>
#include <stdlib.h>
 
int main(int argc, char *argv[])
{
    //Accept single command-line arg
    if (argc != 2){
        printf("Usage: ./recover image\n");
        return 1;
    }

    //Open memory card file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL){
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    //Read-mode
    unsigned char buffer[512];
    int file_count = 0;
    FILE *img = NULL;
    char filename[8];

    while (fread(buffer, 512, 1, file)){
        //Check first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0){
            if (img != NULL){
                fclose(img);
            }
            sprintf(filename, "%03i.jpg", file_count);
            img = fopen(filename, "w");
            file_count++;
        }
        if (img != NULL){
            fwrite(buffer, 512, 1, img);
        }
    }
    if (img != NULL){
        fclose(img);
    }
    fclose(file);
    return 0;
}