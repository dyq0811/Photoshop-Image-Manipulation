// Yingqi Ding (yding50) & Vivian Looi (nlooi1)
// ppm_io.c
// 601.220, Spring 2019

#include <assert.h>
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp);
  
  int n = 100; // max size
  char tag[n];

  fgets(tag, n, fp);
  if (strcmp(tag, "P6\n") != 0) {
    return NULL;
  }

  fgets(tag, n, fp);
  if (tag[0] == '#') {
    fgets(tag, n, fp);
  }
  
  int c = 0;
  int r = 0;
  sscanf(tag, "%d %d", &c, &r);
  if (c <= 0 || r <= 0) {
    return NULL;
  }

  fgets(tag, n, fp);
  if (strcmp(tag, "255\n") != 0) {
    return NULL;
  }

  Image *im = (Image *) malloc(sizeof(Image));
  im->cols = c;
  im->rows = r;

  im->data = malloc(sizeof(Pixel) * c * r);
  fread(im->data, sizeof(Pixel), c * r, fp);
  fclose(fp);

  return im;
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

