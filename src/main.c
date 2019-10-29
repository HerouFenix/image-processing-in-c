#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process_rgb.h"
#include "process_grayscale.h"
#include "image_processing.h"

#define STRING_LEN 10
#define MAX_NUMBER_OF_OPERATIONS 100

RGBImage *RGB_IMAGE;
RGBImage *RGB_OG;
RGBImage *RGB_BACKUP;
int LOADED_RGB = 0;
int RGB_OPERATIONS_INDEX;
char RGB_OPERATIONS[MAX_NUMBER_OF_OPERATIONS][STRING_LEN];

GrayscaleImage *GRAY_IMAGE;
GrayscaleImage *GRAY_OG;
GrayscaleImage *GRAY_BACKUP;
int LOADED_GRAY = 0;
int GRAY_OPERATIONS_INDEX;
char GRAYSCALE_OPERATIONS[MAX_NUMBER_OF_OPERATIONS][STRING_LEN];

GrayscaleImage *RGB_TO_GRAY_IMAGE;
GrayscaleImage **RGB_TO_GRAY_IMAGES;

//Add stuff for binary

void increment_rgb_operations(char *operation)
{
    if (RGB_OPERATIONS_INDEX > MAX_NUMBER_OF_OPERATIONS)
    {
        RGB_OPERATIONS_INDEX = 0;
    }

    strcpy(RGB_OPERATIONS[RGB_OPERATIONS_INDEX++], operation);
}

void increment_gray_operations(char *operation)
{
    if (GRAY_OPERATIONS_INDEX > MAX_NUMBER_OF_OPERATIONS)
    {
        GRAY_OPERATIONS_INDEX = 0;
    }

    
    strcpy(GRAYSCALE_OPERATIONS[GRAY_OPERATIONS_INDEX++], operation);
}

int main()
{
    char option;

    while (1)
    {
        fflush(stdin); //Clear keyboard buffer

        puts("\nPick an Operation"
             "\n[L]oad Image"
             "\n[S]ave Image"
             "\n[R]eset Image"
             "\n[B]ackup Image"
             "\n[U]nload Image"
             "\n[C]ut Image"
             "\n[F]ilter Image"
             "\n[I]ntensify Image"
             "\n[G]et Pixel"
             "\n[A]dapt Image"
             "\n[W]atermark Image"
             "\n[H]istory of changes to Image"
             "\n[D]etails about current Image"
             "\n.........."
             "\n[x]Exit\n");
        if (!scanf(" %c", &option))
        {
            fprintf(stderr, "ERROR: Something went wrong reading your input\n");
            exit(1);
        }

        switch (option)
        {
        case 'L':          //Load Image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhat type of image do you want to load?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel\n");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                puts("\nType down your image's path: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                RGB_IMAGE = load_rgb_file(file_path); //Load Image
                RGB_OG = copy_rgb_image(RGB_IMAGE);   //Make a copy of the image for safe keeping
                LOADED_RGB = 1;

                increment_rgb_operations("  Loaded  ");
                printf("\nSuccessfully loaded RGB Image from %s\n", file_path);
            }
            else if (option == 'G')
            {
                fflush(stdin);

                puts("\nType down your image's path: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                GRAY_IMAGE = load_grayscale_file(file_path); //Load Image
                GRAY_OG = copy_grayscale_image(GRAY_IMAGE);  //Make a copy of the image for safe keeping
                LOADED_GRAY = 1;

                increment_gray_operations("  Loaded  ");

                printf("\nSuccessfully loaded Grayscale Image from %s\n", file_path);
            }
            else if (option == 'C')
            {
                break;
            }
            break;

        case 'S':          //Save Image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhat type of image do you want to save?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                puts("\nType down your image's path: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                save_rgb_to_file(file_path, RGB_IMAGE); //Save Image
                increment_rgb_operations("  Saved   ");
                printf("\nSuccessfully saved RGB Image to %s\n", file_path);
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                puts("\nType down your image's path: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                save_grayscale_to_file(file_path, GRAY_IMAGE); //Save Image
                increment_gray_operations("  Saved   ");
                printf("\nSuccessfully saved Grayscale Image to %s\n", file_path);
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'R':          //Reset current image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to reset?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                puts("\nDo you want to do a Hard Reset or a Backup Reset?"
                     "\n[H]ard Reset"
                     "\n[B]ackup Reset");

                if (!scanf(" %c", &option))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                if (option == 'H')
                {
                    RGB_IMAGE = copy_rgb_image(RGB_OG); //Reset Image
                }
                else
                {
                    if (RGB_BACKUP != NULL)
                    {
                        RGB_IMAGE = copy_rgb_image(RGB_BACKUP); //Reset Image
                    }
                    else
                    {
                        puts("\nERROR: You haven't made any backups!");
                        break;
                    }
                }

                increment_rgb_operations("  Reset   ");
                printf("\nSuccessfully reset all changes made to the RGB Image\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                puts("\nDo you want to do a Hard Reset or a Backup Reset?"
                     "\n[H]ard Reset"
                     "\n[B]ackup Reset");

                if (!scanf(" %c", &option))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                if (option == 'H')
                {
                    GRAY_IMAGE = copy_grayscale_image(GRAY_OG); //Reset Image
                }
                else
                {
                    if (GRAY_BACKUP != NULL)
                    {
                        GRAY_IMAGE = copy_grayscale_image(GRAY_BACKUP); //Reset Image
                    }
                    else
                    {
                        puts("\nERROR: You haven't made any backups!");
                        break;
                    }
                }
                increment_gray_operations("  Reset   ");
                printf("\nSuccessfully reset all changes made to the Grayscale Image\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'B':          //Backup current image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to backup?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                RGB_BACKUP = copy_rgb_image(RGB_IMAGE); //Reset Image
                increment_rgb_operations("  Backup  ");
                printf("\nSuccessfully backed up all changes made to the RGB Image\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                GRAY_BACKUP = copy_grayscale_image(GRAY_IMAGE); //Reset Image
                increment_gray_operations("  Backup  ");
                printf("\nSuccessfully backed up all changes made to the Grayscale Image\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;

        case 'U':          //Unload current image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to unload?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                RGB_IMAGE = NULL; //UNLOAD
                LOADED_RGB = 0;
                increment_rgb_operations("  Unload  ");
                printf("\nSuccessfully unloaded the RGB Image\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                puts("\nType down your image's path: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                GRAY_IMAGE = NULL; //UNLOAD
                LOADED_GRAY = 0;
                increment_gray_operations("  Unload  ");
                printf("\nSuccessfully unloaded the Grayscale Image\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;

            //TO DO CODE
            break;
        case 'C':          //Subsect current image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhat type of image do you want to subsect?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                int start[2], end[2];

                puts("\nType in your starting ROW position?");
                if (!scanf(" %d", &start[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your starting COL position?");
                if (!scanf(" %d", &start[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your ending ROW position?");
                if (!scanf(" %d", &end[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your ending COL position?");
                if (!scanf(" %d", &end[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }


                RGB_IMAGE = get_rgb_subsection(RGB_IMAGE,start,end); //Save Image
                increment_rgb_operations(" Subsect  ");
                printf("\nSuccessfully subsected RGB Image\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                int start[2], end[2];

                puts("\nType in your starting ROW position?");
                if (!scanf(" %d", &start[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your starting COL position?");
                if (!scanf(" %d", &start[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your ending ROW position?");
                if (!scanf(" %d", &end[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your ending COL position?");
                if (!scanf(" %d", &end[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }


                GRAY_IMAGE = get_grayscale_subsection(GRAY_IMAGE,start,end); //Save Image
                increment_gray_operations(" Subsect  ");
                printf("\nSuccessfully subsected Grayscale Image\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'F':          //Apply filter to image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhat type of image do you want to apply a filter to?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                int kernel_size[2];

                puts("\nType in your Kernel's HEIGHT");
                if (!scanf(" %d", &kernel_size[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your Kernel's WIDTH");
                if (!scanf(" %d", &kernel_size[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                double kernel[kernel_size[0] * kernel_size[1]];
                int kernel_index = 0;
                for(int i = 0 ; i < kernel_size[0] ; i++){
                    for (int j = 0 ; j < kernel_size[1] ; j++){
                        printf("\nType in your kernel's (%d,%d) value: ",i,j);
                        if (!scanf(" %lf", &kernel[kernel_index++]))
                        {
                            fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                            exit(1);
                        }
                    }
                }


                apply_rgb_filter(RGB_IMAGE,kernel,kernel_size); //Save Image
                increment_rgb_operations("  Filter  ");
                printf("\nSuccessfully applied a filter to the RGB Image\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                int kernel_size[2];

                puts("\nType in your Kernel's HEIGHT");
                if (!scanf(" %d", &kernel_size[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your Kernel's WIDTH");
                if (!scanf(" %d", &kernel_size[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                double kernel[kernel_size[0] * kernel_size[1]];
                int kernel_index = 0;
                for(int i = 0 ; i < kernel_size[0] ; i++){
                    for (int j = 0 ; j < kernel_size[1] ; j++){
                        printf("\nType in your kernel's (%d,%d) value",i,j);
                        if (!scanf(" %lf", &kernel[kernel_index++]))
                        {
                            fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                            exit(1);
                        }
                    }
                }


                apply_grayscale_filter(GRAY_IMAGE,kernel,kernel_size); //Save Image
                increment_gray_operations("  Filter  ");
                printf("\nSuccessfully applied a filter to the Gray Image\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'I':          //Change an image's intensity
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhat type of image do you want to change the intensity of?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                int intensity[3];

                puts("\nHow much do you want to change the RED intensity by?");
                if (!scanf(" %d", &intensity[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nHow much do you want to change the GREEN intensity by?");
                if (!scanf(" %d", &intensity[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nHow much do you want to change the BLUE intensity by?");
                if (!scanf(" %d", &intensity[2]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                change_rgb_intensity(RGB_IMAGE,intensity);
                increment_rgb_operations("Intensify ");
                printf("\nSuccessfully changed the RGB Image's intensity\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                int intensity;

                puts("\nHow much do you want to change the intensity by?");
                if (!scanf(" %d", &intensity))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                change_gray_intensity(GRAY_IMAGE,intensity);
                increment_gray_operations("Intensify ");
                printf("\nSuccessfully changed the Grayscale Image's intensity\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'A':          //Adapt rgb image to grayscale
            fflush(stdin); //Clear keyboard buffer

            puts("\nDo you want to convert your RGB into a single Grayscale or one Grayscale per colour (RGB)?"
                 "\n[S]ingle"
                 "\n[M]ultiple"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'S')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                puts("\nType down a path to save your image to: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                RGB_TO_GRAY_IMAGE = convert_rgb_to_grayscale(RGB_IMAGE);
                save_grayscale_to_file(file_path, RGB_TO_GRAY_IMAGE); //Save Image
                increment_rgb_operations(" Adapted  ");
                printf("\nSuccessfully converted RGB image to single Grayscale image and saved it to %s\n", file_path);
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                puts("\nType down a path to save your image to: ");
                char file_path[256];
                if (!scanf(" %s", file_path))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                RGB_TO_GRAY_IMAGES = convert_rgb_to_three_grayscale(RGB_IMAGE);
                for(int i = 0 ; i < 3 ; i++){
                    if (i == 0) puts("\nType down a path to save your RED grayscale converison image to: ");
                    else if(i == 1) puts("\nType down a path to save your GREEN grayscale conversion image to: ");
                    else puts("\nType down a path to save your BLUE grayscale conversion image to: ");
                    
                    char file_path[256];
                    if (!scanf(" %s", file_path))
                    {
                        fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                        exit(1);
                    }
                    save_grayscale_to_file(file_path, RGB_TO_GRAY_IMAGES[i]); //Save Image
                    increment_rgb_operations(" AdaptedM ");
                    printf("\nSuccessfully converted one of the RGB Image's colours to a Grayscale image and saved it to %s\n", file_path);
                }
                
            }
            else if (option == 'C')
            {
                break;
            }
            break;
        case 'H':          //Print out the history of changes applied to an image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to know the History of?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                /*
                for (int i = 0; i < RGB_OPERATIONS_INDEX; i++)
                {
                    printf("%s ;", RGB_OPERATIONS[i]);
                }
                */
                printf("%s", RGB_OPERATIONS[0]);
                printf("\n\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                /*for (int i = 0; i <= GRAY_OPERATIONS_INDEX; i++)
                {
                    printf("%s ;", GRAYSCALE_OPERATIONS[i]);
                }*/
                printf("%s", GRAYSCALE_OPERATIONS[0]);
                printf("\n\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;

        case 'D':          //Print out the details of an image
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to know the details of?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                printf("Width: %d\nHeight: %d\n",RGB_IMAGE->width,RGB_IMAGE->height);
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                printf("Width: %d\nHeight: %d\n",GRAY_IMAGE->width,GRAY_IMAGE->height);
            }
            else if (option == 'C')
            {
                break;
            }
            break;

        case 'G':          //Print out info on a single pixel
            fflush(stdin); //Clear keyboard buffer

            puts("\nWhich Image do you want to get a pixel from?"
                 "\n[R]GB"
                 "\n[G]rayscale"
                 //Add option for binary
                 "\n.........."
                 "\n[C]ancel");
            if (!scanf(" %c", &option))
            {
                fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                exit(1);
            }

            if (option == 'R')
            {
                fflush(stdin);

                if (LOADED_RGB == 0)
                {
                    puts("\nERROR: You haven't loaded any RGB Images into memory!");
                    break;
                }

                int pos[2];

                puts("\nType in your pixel's ROW?");
                if (!scanf(" %d", &pos[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your pixel's COL?");
                if (!scanf(" %d", &pos[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                Colour pixel = get_rgb_pixel(RGB_IMAGE,pos[0],pos[1]);

                printf("R: %d G: %d B: %d\n", pixel.R, pixel.G, pixel.B);
                printf("\nSuccessfully gathered RGB pixel's information\n");
            }
            else if (option == 'G')
            {
                fflush(stdin);

                if (LOADED_GRAY == 0)
                {
                    puts("\nERROR: You haven't loaded any Grayscale Images into memory!");
                    break;
                }

                int pos[2];

                puts("\nType in your pixel's ROW?");
                if (!scanf(" %d", &pos[0]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                puts("\nType in your pixel's COL?");
                if (!scanf(" %d", &pos[1]))
                {
                    fprintf(stderr, "ERROR: Something went wrong reading your input\n");
                    exit(1);
                }

                Grayscale pixel = get_grayscale_pixel(GRAY_IMAGE,pos[0],pos[1]);

                printf("Gray: %d\n", pixel.Gray);
                printf("\nSuccessfully gathered Grayscale pixel's information\n");
            }
            else if (option == 'C')
            {
                break;
            }
            break;

        case 'W':          //Watermark Image TODO: PEDRO
            break;

        case 'x':
            puts("Bye bye!");
            return 0;
        default:
            puts("\nYou've entered an invalid command!\nDoing nothing...");
        }
    }
    return 0;
}