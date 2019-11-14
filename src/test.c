#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "process_rgb.h"
#include "process_grayscale.h"
#include "process_binary.h"
#include "image_processing.h"

double elapsed_time(void)
{
  static struct timespec last_time,current_time;

  last_time = current_time;
  if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)  // the first argument could also be CLOCK_REALTIME
    return -1.0; // clock_gettime() failed!!!
  return ((double)current_time.tv_sec - (double)last_time.tv_sec) +
         1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}

int main(int argc, char *argv[]){
	char *filename;
	if (argc > 1) filename = argv[1];
	else filename = "../lena.ppm";

	//Loading rgb image
    double init_timer, fin_timer;
    init_timer = elapsed_time();
	RGBImage *image = load_rgb_file(filename);
    fin_timer = elapsed_time();
    printf("It took %.8f to load rgb image\n", fin_timer-init_timer);
	//Save original rgb image
	save_rgb_to_file("rgb_image.ppm", image);

	//Convert rgb to one gray img
    init_timer = elapsed_time();
    GrayscaleImage *gray_img = convert_rgb_to_grayscale(image);
    fin_timer = elapsed_time();
    printf("It took %.8f to convert image to grayscale\n", fin_timer-init_timer);
    
    //Save that gray image
    save_grayscale_to_file("gray_image.pgm", gray_img);

    //Convert rgb to three different imgs and save them all
    GrayscaleImage **gray_images;
    init_timer = elapsed_time();
    gray_images = convert_rgb_to_three_grayscale(image);
    fin_timer = elapsed_time();
    printf("It took %.8f to convert image to 3 grayscale\n", fin_timer-init_timer);
    
    save_grayscale_to_file("gray_img_R.pgm", gray_images[0]);
    save_grayscale_to_file("gray_img_G.pgm", gray_images[1]);
    save_grayscale_to_file("gray_img_B.pgm", gray_images[2]);

    //Load a gray image
    gray_img = load_grayscale_file("gray_image.pgm");

    //convert grayscale to binary file

    BinaryImage * bin_image = convert_gray_to_bin(gray_img, 190);
    save_to_bin_file("binary_img.pbm", bin_image);
    fin_timer = elapsed_time();
    printf("It took %.8f to convert grayscale to binary\n", fin_timer-init_timer);
    
    //Convert with the Otsu algorithm
    bin_image = convert_gray_to_bin_otsu(gray_img);
    save_to_bin_file("binary_img_otsu.pbm", bin_image);
    fin_timer = elapsed_time();
    printf("It took %.8f to convert grayscale to binary and calculate appropriate Otsu level\n", fin_timer-init_timer);
    
    //Load a binary file
    bin_image = load_bin_file("binary_img.pbm");

    //Cutting operations
    int start[2], end[2];
    start[0] = 0, start[1] = 0;
    end[0] = 255, end[1] = 511;
    init_timer = elapsed_time();
    save_rgb_to_file("rgb_img_subsection.ppm", get_rgb_subsection(image, start, end));
    save_grayscale_to_file("grayscale_img_subsection.pgm", get_grayscale_subsection(gray_img, start, end));
    save_to_bin_file("binary_img_subsection.pbm", get_bin_subsection(bin_image, start, end));
    fin_timer = elapsed_time();
    printf("It took %.8f to cut the three images\n", fin_timer-init_timer);
    
    //Get Pixel information
    Colour cpixel = get_rgb_pixel(image, image->width / 2, image->height / 2);
    printf("R%d G%d B%d\n", cpixel.R, cpixel.G, cpixel.B);
    
    Grayscale gpixel = get_grayscale_pixel(gray_img, gray_img->width / 2, gray_img->height / 2);
    printf("Gray%d\n", gpixel.Gray);

    unsigned char bpixel = access_bin_pixel(bin_image, bin_image->width / 2, bin_image->height / 2);
    printf("Bin%d\n", bpixel);

    //Change image intensity
    int intensity[3];
    intensity[0] = -100;
    intensity[1] = -100;
    intensity[2] = 255;
    init_timer = elapsed_time();
    change_rgb_intensity(image, intensity);
    save_rgb_to_file("rgb_img_intense.ppm", image);
    fin_timer = elapsed_time();
    printf("It took %.8f to change rgb intensity\n", fin_timer-init_timer);
        
    //Get original back
    image = load_rgb_file("rgb_image.ppm");

    int filter_dimension[2] = {3, 3};

    //Test filters
    double emboss_kernel[9] = {-2, -1, 0, -1, 1, 1, 0, 1, 2};
    init_timer = elapsed_time();
    apply_rgb_filter(image, emboss_kernel, filter_dimension);
    save_rgb_to_file("image_emboss.ppm", image);
    fin_timer = elapsed_time();
    printf("It took %.8f to apply a filter on a rgb\n", fin_timer-init_timer);
    
    init_timer = elapsed_time();
    apply_grayscale_filter(gray_img, emboss_kernel, filter_dimension);
    save_grayscale_to_file("gray_img_emboss.pgm", gray_img);
    fin_timer = elapsed_time();
    printf("It took %.8f to apply a filter on a grayscale\n", fin_timer-init_timer);

    double sobel_kernel[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    apply_rgb_filter(image, sobel_kernel, filter_dimension);
    save_rgb_to_file("image_sobel.ppm", image);

    apply_grayscale_filter(gray_img, sobel_kernel, filter_dimension);
    save_grayscale_to_file("gray_img_sobel.pgm", gray_img);
    

    double sharpen_kernel[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};

    apply_rgb_filter(image, sharpen_kernel, filter_dimension);
    save_rgb_to_file("image_sharpen.ppm", image);

    apply_grayscale_filter(gray_img, sharpen_kernel, filter_dimension);
    save_grayscale_to_file("gray_img_sharpen.pgm", gray_img);
    
    double outline_kernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};

    apply_rgb_filter(image, outline_kernel, filter_dimension);
    save_rgb_to_file("image_outline.ppm", image);

    apply_grayscale_filter(gray_img, outline_kernel, filter_dimension);
    save_grayscale_to_file("gray_img_outline.pgm", gray_img);

    
    double blur_kernel[9] = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};

    apply_rgb_filter(image, blur_kernel, filter_dimension);
    save_rgb_to_file("image_blur.ppm", image);

    apply_grayscale_filter(gray_img, blur_kernel, filter_dimension);
    save_grayscale_to_file("gray_img_blur.pgm", gray_img);
    
    //Add logo operation: which includes reduce + adding logo

    image = load_rgb_file("rgb_image.ppm");
    init_timer = elapsed_time();
    RGBImage* new_logo = reduce_image(image, 115,115);
    fin_timer = elapsed_time();
    printf("It took %.8f to reduce an rgb image\n", fin_timer-init_timer);
    save_rgb_to_file("new_small_image.ppm", new_logo);
    int pos[2] = {233,150};
    init_timer = elapsed_time();
    add_logo_to_image(image, new_logo, pos, 0.7);
    fin_timer = elapsed_time();
    printf("It took %.8f to apply add a logo\n", fin_timer-init_timer);
    save_rgb_to_file("image_with_logo.ppm", image);
	return 0;
}