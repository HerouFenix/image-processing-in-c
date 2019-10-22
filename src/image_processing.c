#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Colour;

typedef struct
{
    int width, height;
    Colour *pixel_array;
} RGBImage;

typedef struct
{
    unsigned char Gray;
} Grayscale;

typedef struct
{
    int width, height;
    Grayscale *pixel_array;
} GrayscaleImage;


/********************************************//**
 *  FUNCTION DEFINITIONS
 ***********************************************/
int save_grayscale_to_file(char *file_name, GrayscaleImage *image)
{
    FILE *fp;
    int width, height, colour_range;
    char *img_type;

    fp = fopen(file_name, "wb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "ERROR: An error occurred finding the file '%s'\n", file_name);
        return 1;
    }

    img_type = "P5";
    height = image->height, width = image->width, colour_range = 255;

    //Write the PPM's headers
    fprintf(fp, "%s\n", img_type);
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", colour_range);

    //Write the image's pixels
    fwrite(image->pixel_array, width, height, fp);

    fclose(fp);
    return 0;
};

GrayscaleImage *load_grayscale_file(char *file_name)
{
    char image_type[8];
    int comments, colour_range = 0;
    FILE *fp;
    GrayscaleImage *image;

    fp = fopen(file_name, "rb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "ERROR: An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if ((!fgets(image_type, sizeof(image_type), fp)) && (image_type[0] != 'P' && image_type[1] != '5'))
    { //Get PPM Type (Goes from P0-P6)
        fprintf(stderr, "ERROR: The PGM image file seems to be malformed or is incorrect - Should be P5\n");
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp); //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    image = (GrayscaleImage *)malloc(sizeof(GrayscaleImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "ERROR: The PGM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    if ((fscanf(fp, "%d", &colour_range) != 1) && (colour_range != 255))
    { //Get colour range (should be 255)
        fprintf(stderr, "ERROR: The PGM image seems to have an invalid grayscale range - Should be 255\n");
        exit(1);
    }

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    image->pixel_array = (Grayscale *)malloc(sizeof(Grayscale) * image->width * image->height); //Allocate memory for image's pixels

    if (fread(image->pixel_array, image->width, image->height, fp) != image->height)
    { //Read all remaining data from the image (Remember each grayscale pixel is just a value from 0-255)
        fprintf(stderr, "ERROR: At least one line couldn't be read from the image\n");
        exit(1);
    }

    fclose(fp);
    return image;
};

int save_rgb_to_file(char *file_name, RGBImage *image)
{
    FILE *fp;
    int width, height, colour_range;
    char *img_type;

    fp = fopen(file_name, "wb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "ERROR: An error occurred finding the file '%s'\n", file_name);
        return 1;
    }

    img_type = "P6";
    height = image->height, width = image->width, colour_range = 255;

    //Write the PPM's headers
    fprintf(fp, "%s\n", img_type);
    fprintf(fp, "%d %d\n", width, height);
    fprintf(fp, "%d\n", colour_range);

    //Write the image's pixels
    fwrite(image->pixel_array, 3 * width, height, fp);

    fclose(fp);
    return 0;
};

RGBImage *load_rgb_file(char *file_name)
{
    char image_type[8];
    int colour_range, comments;
    FILE *fp;
    RGBImage *image;

    fp = fopen(file_name, "rb"); //Open File
    if (!fp)
    {
        fprintf(stderr, "ERROR: An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if ((!fgets(image_type, sizeof(image_type), fp)) && (image_type[0] != 'P' && image_type[1] != '6'))
    { //Get PPM Type (Goes from P0-P6)
        fprintf(stderr, "ERROR: The PPM image file seems to be malformed\n");
        exit(1);
    }

    image = (RGBImage *)malloc(sizeof(RGBImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "ERROR: The PPM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    if ((fscanf(fp, "%d", &colour_range) != 1) && (colour_range != 255))
    { //Get colour range (should be 255)
        fprintf(stderr, "ERROR: The PPM image seems to have an invalid rgb range - Should be 255\n");
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp); //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    image->pixel_array = (Colour *)malloc(sizeof(Colour) * image->width * image->height); //Allocate memory for image's pixels

    if (fread(image->pixel_array, 3 * image->width, image->height, fp) != image->height)
    { //Read all remaining data from the image (we multiply the width by 3 because each pixel holds 3 chars - r,g,b)
        fprintf(stderr, "ERROR: At least one line couldn't be read from the image\n");
        exit(1);
    }

    fclose(fp);
    return image;
};

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

// DISCUTIR COM O PEDRO, MIGHT BE A BETTER WAY, isto tem mt codigo repetido, maybe usar uma função de suporte?
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

    return 0;
}