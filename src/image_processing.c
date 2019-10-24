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

    for (int i = 0; i < res->height*res->width; i++){
        res->bin_array[i]= (unsigned char) ((image->pixel_array + i) > threshold)? 255:0; 
    }

    return res;
}

GrayscaleImage* convert_rgb_to_grayscale(RGBImage *rgb_image){
    GrayscaleImage *gray_img;
    Grayscale gray;
    Colour colour;
    gray_img = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct


    gray_img->height = rgb_image->height;
    gray_img->width = rgb_image->width;
    gray_img->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * gray_img->width * gray_img->height); //Allocate memory for image's pixels


    for(int i = 0 ; i < rgb_image->height * rgb_image->width ; i++ ){
        //Grayscale = (R + G + B / 3)

        colour = rgb_image->pixel_array[i];

        gray.Gray = (colour.R + colour.G + colour.B)/3;

        gray_img->pixel_array[i] = gray;
    }

    return gray_img;
}


GrayscaleImage** convert_rgb_to_three_grayscale(RGBImage *rgb_image){
    GrayscaleImage **gray_images;
    GrayscaleImage *gray_img_r, *gray_img_g, *gray_img_b;
    Grayscale gray;
    Colour colour;

    gray_img_r = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_img_g = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_img_b = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_images = (GrayscaleImage **)malloc(sizeof(GrayscaleImage)*3);

    gray_img_r->height = rgb_image->height;
    gray_img_g->height = rgb_image->height;
    gray_img_b->height = rgb_image->height;

    gray_img_r->width = rgb_image->width;
    gray_img_g->width = rgb_image->width;
    gray_img_b->width = rgb_image->width;

    gray_img_r->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels
    gray_img_g->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels
    gray_img_b->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels

    for(int i = 0 ; i < rgb_image->height * rgb_image->width ; i++ ){
        colour = rgb_image->pixel_array[i];

        gray.Gray = colour.R;
        gray_img_r->pixel_array[i] = gray ;

        gray.Gray = colour.G;
        gray_img_g->pixel_array[i] = gray ;

        gray.Gray = colour.B;
        gray_img_b->pixel_array[i] = gray ;
    }

    gray_images[0] = gray_img_r;
    gray_images[1] = gray_img_g;
    gray_images[2] = gray_img_b;


    return gray_images;
}


int main()
{
    RGBImage *image = load_rgb_file("../lena.ppm");

    GrayscaleImage *gray_img = convert_rgb_to_grayscale(image);

    save_grayscale_to_file("lenaR.pgm", gray_img);


    GrayscaleImage **gray_images;
    gray_images = convert_rgb_to_three_grayscale(image);

    save_grayscale_to_file("lenaR.pgm", gray_images[0]);
    save_grayscale_to_file("lenaG.pgm", gray_images[1]);
    save_grayscale_to_file("lenaB.pgm", gray_images[2]);


    GrayscaleImage *image_og = load_grayscale_file("../galaxy.ascii.pbm");

    save_to_bin_file("galaxy_bin_threshold.pbm", convert_gray_to_bin(image_og, 128));

    return 0;
}