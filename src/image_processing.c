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
BinaryImage* convert_gray_to_bin(GrayscaleImage *image, unsigned char threshold) {
    BinaryImage *res;
    res = (BinaryImage *)malloc(sizeof(BinaryImage)); 
    
    if(image->width %8 != 0){
        printf("Image width not divisible by 8, due to bit stuffing issues, it wouldn't be converted correctly\n");
        return res;
    }

    res->height = image->height;
    res->width = image->width;

    res->bin_array = (unsigned char *)malloc(res->height*res->width);

    for (int i = 0; i < res->height*res->width; i++){
        res->bin_array[i] = (image->pixel_array[i].Gray < threshold) <<7;
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

void add_logo_to_image(RGBImage *rgb_image, RGBImage *logo, int* pixel, float intensity){
    // For loop variables
    int img_x, img_y, logo_x, logo_y;

    for (img_x = pixel[0], logo_x=0; img_x < pixel[0]+logo->width; img_x++, logo_x++){
        for (img_y = pixel[1], logo_y=0; img_y < pixel[1]+logo->height; img_y++, logo_y++){
            if (img_x < 0 || img_y < 0) continue;
            if (img_x >= rgb_image->width || img_y >= rgb_image->height) break;

            Colour logo_pixel = get_rgb_pixel(logo, logo_y, logo_x);
            Colour img_pixel = get_rgb_pixel(rgb_image, img_y, img_x);
            img_pixel.R = (unsigned char)((1-intensity)*img_pixel.R + intensity*logo_pixel.R);
            img_pixel.G = (unsigned char)((1-intensity)*img_pixel.G + intensity*logo_pixel.G);
            img_pixel.B = (unsigned char)((1-intensity)*img_pixel.B + intensity*logo_pixel.B);
            rgb_image->pixel_array[img_y * rgb_image->width + img_x] = img_pixel;
        }
    }
}

RGBImage* reduce_image(RGBImage* image, int new_height, int new_width){
    RGBImage *result;
    int width_ratio, height_ratio;

    width_ratio = image->width/new_width;
    height_ratio = image->height/new_height;

    result = (RGBImage *)malloc(sizeof(RGBImage));
    result->width = new_width;
    result->height = new_height;
    
    result->pixel_array = (Colour *)calloc(result->width*result->height, sizeof(Colour));

    for (int i =0; i < result->width*result->height; i++){
        int r = 0, g = 0, b = 0;
        int col = i%result->height;
        int row = i/result->width;
        for (int j = 0; j<height_ratio; j++){
            for (int k = 0; k<width_ratio;k++){
                Colour curr_pixel = get_rgb_pixel(image,row*width_ratio+k, col*height_ratio+j);
                r += curr_pixel.R;
                g += curr_pixel.G;
                b += curr_pixel.B;
            }
        }
        result->pixel_array[i].R = r/(height_ratio*width_ratio);
        result->pixel_array[i].G = g/(height_ratio*width_ratio);
        result->pixel_array[i].B = b/(height_ratio*width_ratio);
        
    }

    return result;
}

BinaryImage* convert_gray_to_bin_otsu(GrayscaleImage *image){
    int *histogram;

    histogram = (int*) calloc(1<<8, sizeof(int));
    for (int i = 0; i < image->width; i++){
        for (int j = 0; j < image->height; j++){
            int hist_index = get_grayscale_pixel(image, i, j).Gray;
            histogram[hist_index]++;
        }
    }

    int top = 256, wb = 0, wf=0, level=0;
    int total = image->height*image->width;
    float sumB = 0.0, sum = 0.0;
    float max = 0.0;

    for (int i = 0; i < top; i++){
        sum += i * histogram[i];
    }

    for (int i = 0; i<top; i++){
        
        wb += histogram[i];
        if (wb == 0) continue;
        wf = total - wb;
        if (wf == 0) break;
        sumB +=(i*histogram[i]);

        float mB = sumB/wb;
        float mF = (sum - sumB) / wf;
        float val =(float) wb*(float)wf*(mB - mF)*(mB- mF);
        if (val >= max){
            level = i;
            max = val;
        }
    }

    return convert_gray_to_bin(image, level);
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

    save_to_bin_file("lena.pbm", convert_gray_to_bin(gray_img, 128));
    
    int filter_dimension[2] = {3, 3};
    RGBImage* logo = load_rgb_file("../logo.ppm");
    RGBImage* new_logo = reduce_image(logo, 115,115);
    save_rgb_to_file("new_small_logo.ppm", new_logo);
    save_rgb_to_file("logo.ppm", logo);
    
    int pos[2] = {233,150};
    add_logo_to_image(image, logo, pos, 0.7);

    return 0;
}