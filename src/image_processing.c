#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process_rgb.h"
#include "process_grayscale.h"
#include "image_processing.h"

/********************************************/ /**
 *  FUNCTION DEFINITIONS
 ***********************************************/
GrayscaleImage *convert_rgb_to_grayscale(RGBImage *rgb_image)
{
    GrayscaleImage *gray_img;
    Grayscale gray;
    Colour colour;
    gray_img = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct

    gray_img->height = rgb_image->height;
    gray_img->width = rgb_image->width;
    gray_img->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * gray_img->width * gray_img->height); //Allocate memory for image's pixels

    for (int i = 0; i < rgb_image->height * rgb_image->width; i++)
    {
        //Grayscale = (R + G + B / 3)

        colour = rgb_image->pixel_array[i];

        gray.Gray = (colour.R + colour.G + colour.B) / 3;

        gray_img->pixel_array[i] = gray;
    }

    return gray_img;
}

GrayscaleImage **convert_rgb_to_three_grayscale(RGBImage *rgb_image)
{
    GrayscaleImage **gray_images;
    GrayscaleImage *gray_img_r, *gray_img_g, *gray_img_b;
    Grayscale gray;
    Colour colour;

    gray_img_r = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_img_g = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_img_b = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct
    gray_images = (GrayscaleImage **)malloc(sizeof(GrayscaleImage) * 3);

    gray_img_r->height = rgb_image->height;
    gray_img_g->height = rgb_image->height;
    gray_img_b->height = rgb_image->height;

    gray_img_r->width = rgb_image->width;
    gray_img_g->width = rgb_image->width;
    gray_img_b->width = rgb_image->width;

    gray_img_r->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels
    gray_img_g->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels
    gray_img_b->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * rgb_image->width * rgb_image->height); //Allocate memory for image's pixels

    for (int i = 0; i < rgb_image->height * rgb_image->width; i++)
    {
        colour = rgb_image->pixel_array[i];

        gray.Gray = colour.R;
        gray_img_r->pixel_array[i] = gray;

        gray.Gray = colour.G;
        gray_img_g->pixel_array[i] = gray;

        gray.Gray = colour.B;
        gray_img_b->pixel_array[i] = gray;
    }

    gray_images[0] = gray_img_r;
    gray_images[1] = gray_img_g;
    gray_images[2] = gray_img_b;

    return gray_images;
}

int apply_rgb_filter(RGBImage *image, double *kernel, int filter_dimension[2])
{
    if (filter_dimension[0] % 2 == 0 || filter_dimension[1] % 2 == 0)
    {
        fprintf(stderr, "ERROR: The filter kernel's dimensions must be odd numbers!\n");
        return 1;
    }

    int row_offset, col_offset, current_pixel_index, image_row, subsection_row, kernel_row, image_col, subsection_col, kernel_col;
    Colour new_pixel, current_pixel;
    Colour *new_pixel_array;

    row_offset = filter_dimension[0] / 2; //How many pixels to our left/right do we move from the central pixel
    col_offset = filter_dimension[1] / 2; //How many pixels to our up/down do we move from the central pixel

    new_pixel_array = (Colour *)malloc(sizeof(Colour) * image->width * image->height);

    current_pixel_index = 0;
    //We apply a filter by multiplying all pixels around a center pixel with its correspondant in the filter's kernel and summing all values up
    //Iterate over all of the image's pixels
    for (image_row = 0; image_row < image->height; image_row++)
    {
        for (image_col = 0; image_col < image->width; image_col++)
        {
            new_pixel.R = 0;
            new_pixel.G = 0;
            new_pixel.B = 0;

            //Iterate over a subsection of the image around the current (central) pixel
            for (subsection_row = image_row - row_offset; subsection_row < image_row + row_offset; subsection_row++)
            {
                kernel_row = 0;
                for (subsection_col = image_col - col_offset; subsection_col < image_col + col_offset; subsection_col++)
                {
                    kernel_col = 0;
                    current_pixel = get_rgb_pixel(image, subsection_row, subsection_col); //Our pixel in the original

                    new_pixel.R += current_pixel.R * kernel[kernel_row * image->width + kernel_col];
                    new_pixel.G += current_pixel.G * kernel[kernel_row * image->width + kernel_col];
                    new_pixel.B += current_pixel.B * kernel[kernel_row * image->width + kernel_col];

                    kernel_col++;
                }
                kernel_row++;
            }

            new_pixel_array[current_pixel_index++] = new_pixel;
        }
    }

    image->pixel_array = new_pixel_array;

    return 0;
}

int apply_grayscale_filter(GrayscaleImage *image, double *kernel, int filter_dimension[2])
{
    if (filter_dimension[0] % 2 == 0 || filter_dimension[1] % 2 == 0)
    {
        fprintf(stderr, "ERROR: The filter kernel's dimensions must be odd numbers!\n");
        return 1;
    }

    int row_offset, col_offset, current_pixel_index, image_row, subsection_row, kernel_row, image_col, subsection_col, kernel_col;
    Grayscale new_pixel, current_pixel;
    Grayscale *new_pixel_array;

    row_offset = filter_dimension[0] / 2; //How many pixels to our left/right do we move from the central pixel
    col_offset = filter_dimension[1] / 2; //How many pixels to our up/down do we move from the central pixel

    new_pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * image->width * image->height);

    current_pixel_index = 0;
    //We apply a filter by multiplying all pixels around a center pixel with its correspondant in the filter's kernel and summing all values up
    //Iterate over all of the image's pixels
    for (image_row = 0; image_row < image->height; image_row++)
    {
        for (image_col = 0; image_col < image->width; image_col++)
        {
            new_pixel.Gray = 0;

            //Iterate over a subsection of the image around the current (central) pixel
            for (subsection_row = image_row - row_offset; subsection_row < image_row + row_offset; subsection_row++)
            {
                kernel_row = 0;
                for (subsection_col = image_col - col_offset; subsection_col < image_col + col_offset; subsection_col++)
                {
                    kernel_col = 0;
                    current_pixel = get_grayscale_pixel(image, subsection_row, subsection_col); //Our pixel in the original

                    new_pixel.Gray += current_pixel.Gray * kernel[kernel_row * image->width + kernel_col];

                    kernel_col++;
                }
                kernel_row++;
            }

            new_pixel_array[current_pixel_index++] = new_pixel;
        }
    }

    image->pixel_array = new_pixel_array;

    return 0;
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

    double kernel[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

    int filter_dimension[2] = {3, 3};
    apply_rgb_filter(image, kernel, filter_dimension);
    save_rgb_to_file("lenaFiltered.ppm", image);

    apply_grayscale_filter(gray_img, kernel, filter_dimension);
    save_grayscale_to_file("lenaGrayFiltered.pgm", gray_img);

    return 0;
}