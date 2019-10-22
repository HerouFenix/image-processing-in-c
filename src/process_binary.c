#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process_binary.h"

BinaryImage* get_subsection(BinaryImage *image, int *pos_start, int *pos_end){
    BinaryImage *new_image;
    new_image = (BinaryImage *)malloc(sizeof(BinaryImage)); 

    new_image->width = pos_end[1] - pos_start[1] + 1;
    new_image->height = pos_end[0] - pos_start[0] + 1;

    new_image -> bin_array = (unsigned char *)malloc(new_image->height * new_image-> width);
    int i;
    for (i = 0; i < new_image->height; i++){
        memmove(new_image->bin_array + i*new_image->width,
                image->bin_array + ((pos_start[0]+i)*image->width + pos_start[1]),
                new_image->width);
    }
    return new_image;
}

unsigned char access_pixel(BinaryImage *image, int line, int col){
    return *(image->bin_array + (line*image->width + col));
}

int save_to_file(char *file_name, BinaryImage *image){
    FILE *fp;

    fp = fopen(file_name, "wb");
    if (!fp)
    {
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        return 1;
    }

    fprintf(fp, "P4\n");
    fprintf(fp, "%d %d\n", image->width, image->height);

    fwrite(image->bin_array, image->width, image->height, fp);

    return 0;
}

BinaryImage* load_file(char *file_name){
    FILE *fp;
    int comments;
    char img_type[8];
    BinaryImage* image;

    fp = fopen(file_name, "rb");
    if (!fp){
        fprintf(stderr, "An error occurred finding the file '%s'\n", file_name);
        exit(1);
    }

    if (!fgets(img_type, sizeof(img_type), (FILE*) fp) && (img_type[0] != 'P')){
        fprintf(stderr, "An error has ocurred trying to read the type of '%s', it may be malformatted or curropted\n", file_name);
        exit(1);
    }

    while ((comments = getc(fp)) && (comments == '#')) //Remove comments (lines starting in #)
    {
        while (getc(fp) != '\n') //Get the entire line char by char until we find a breakline
            ;
    }
    ungetc(comments, fp); //If we break out of the last loop it's cus the current char isn't in a line starting with #, so we should unget it

    image = (BinaryImage *)malloc(sizeof(BinaryImage)); //Allocate memory for our image struct

    if (fscanf(fp, "%d %d", &image->width, &image->height) != 2)
    { //Get Size (WIDTHxHEIGHT)
        fprintf(stderr, "The PGM image seems to have an invalid image size (Should be Width x Height)\n");
        exit(1);
    }

    image->bin_array = (unsigned char *) malloc(image->width*image->height);

    while (fgetc(fp) != '\n')
        ; //Remove blank spaces

    fread(image->bin_array, 1, image->height*image->width, fp);

   /* int error;
    if ((error = fread(image->bin_array, image->width, image->height, fp)) != image->height)
    { //Read all remaining data from the image (Remember each grayscale pixel is just a value from 0-255)
        fprintf(stderr, "At least one line %d couldn't be read from the image\n", error);
        exit(1);
    }*/

    fclose(fp);
    return image;
}

int main()
{
	BinaryImage *image = load_file("../marbles.pbm");
    int i, j;
    /*for (i = 0; i < image->height; i++){
        for (j = 0; j < image->width; j++){
            printf("%u", access_pixel(image, i, j));
        }
        printf("\n");
    }*/
    int start[] = {0,0}, end[] = {299,599};
    save_to_file("subsection_ag.pbm", get_subsection(image,start,end));
    int start_2[] = {300,0}, end_2[] = {599, 599};
    save_to_file("subsection_ag_2.pbm", get_subsection(image,start_2,end_2));
    return save_to_file("apollonian_gasket.pbm", image);
}
