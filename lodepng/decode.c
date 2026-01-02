#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

int main(){
    unsigned char* image;
    unsigned width, height,i,j, error, red, green, blue, alpha;

    error = lodepng_decode32_file(&image, &width, &height, "6x4.png");
    if(error){
        printf("error : %d message: %s\n", error, lodepng_error_text(error));
    }


    for(i = 0; i < height; i ++){
        for(j = 0; j<width; j++){
            red = image[4*(i*width + j)];
            green = image[4*(i*width + j) + 1];
            blue = image[4*(i*width + j) + 2];
            alpha = image[4*(i*width + j) + 3];
            // printf("Pixel (%d, %d): R=%d G=%d B=%d A=%d\n", j, i/width, red, green, blue, alpha);
            printf("RGBA [%d %d %d %d]", red, green, blue, alpha);
        }
        printf("\n");
    }

    free(image);
    return 0;
}