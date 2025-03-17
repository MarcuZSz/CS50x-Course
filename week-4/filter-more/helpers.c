#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int avg = round(((double)image[i][j].rgbtRed + (double)image[i][j].rgbtGreen + (double)image[i][j].rgbtBlue) / 3);

            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width / 2; j++){
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;
            float count = 0.00;

            for (int k = -1; k < 2; k++){
                for (int l = -1; l < 2; l++){
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width){
                        sumRed += image[i + k][j + l].rgbtRed;
                        sumGreen += image[i + k][j + l].rgbtGreen;
                        sumBlue += image[i + k][j + l].rgbtBlue;
                        count++;
                    }
                }
            }
            temp[i][j].rgbtRed = round(sumRed / count);
            temp[i][j].rgbtGreen = round(sumGreen / count);
            temp[i][j].rgbtBlue = round(sumBlue / count);
        }
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j] = temp[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            int GxRed = 0, GxGreen = 0, GxBlue = 0;
            int GyRed = 0, GyGreen = 0, GyBlue = 0;

            for (int k = -1; k < 2; k++){
                for (int l = -1; l < 2; l++){
                    if (i + k < 0 || i + k >= height || j + l < 0 || j + l >= width){
                        continue;
                    }
                    GxRed += image[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    GxGreen += image[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                    GxBlue += image[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];

                    GyRed += image[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                    GyGreen += image[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    GyBlue += image[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                }
            }
            temp[i][j].rgbtRed = fmin(255, round(sqrt(GxRed * GxRed + GyRed * GyRed)));
            temp[i][j].rgbtGreen = fmin(255, round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen)));
            temp[i][j].rgbtBlue = fmin(255, round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue)));
        }
    }

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            image[i][j] = temp[i][j];
        }
    }
}
