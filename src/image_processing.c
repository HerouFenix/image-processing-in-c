#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process_rgb.h"
#include "process_grayscale.h"
#include "process_binary.h"
#include "image_processing.h"

/********************************************//**
 *  FUNCTION DEFINITIONS
 ***********************************************/
BinaryImage* convert_gray_to_bin(GrayscaleImage* image, int threshold) {
    BinaryImage *res;
    res = (BinaryImage *)malloc(sizeof(BinaryImage)); 

    res->height = image->height;
    res->width = image->width;

    res->bin_array = (unsigned char)malloc(res->height*res->width);

    return res;
}


int main(){
    GrayscaleImage *image_og = load_grayscale_file("../galaxy.ascii.pbm");

    return save_to_bin_file("galaxy_bin_threshold.pbm", convert_gray_to_bin(image_og, 128));
}