// Yingqi Ding (yding50) & Vivian Looi (nlooi1)
// imageManip.h
// 601.220, Spring 2020

#ifndef IMAGEMANIPH
#define IMAGEMANIPH

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ppm_io.h"
#define PI (double) 3.14159265358979

// Create an empty new img with c cols and r rows.
Image *new_im(int c, int r);

// Convert color code to legal range.
unsigned char color_constraint(double color);

// Modify the exposure of an image.
Image *exposure(Image *img, float ev);

// Blend two input images into one using a given blend ratio alpha. 
Image *blend(Image *im1, Image *im2, float a);

// Zoom in the image.
Image *zoom_in(Image *im);

// Zoom out the image.
Image *zoom_out(Image *im);

// An interesting artistic effect.
Image *pointilism(Image *im);

// Swirl the image by some scale.
Image *swirl(Image *im, int cx, int cy, int s);

// Square the number.
double sq(double x);

// Blur the image according to a Gaussian distribution.
Image *blur(Image *img, double sigma);

#endif
