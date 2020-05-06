// Yingqi Ding (yding50) & Vivian Looi (nlooi1)
// project.c
// 601.220, Spring 2019

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppm_io.h"
#include "imageManip.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Failed to supply input filename or output filename, or both.\n");
    return 1;
  }

  FILE *input = fopen(argv[1], "rb");
  if (!input) {
    fprintf(stderr, "Specified input file could not be opened.\n");
    return 2;
  }

  Image *im = read_ppm(input);
  if (!im) {
    fprintf(stderr, "Specified input file is not a properly-formatted PPM file, or reading input somehow fails.\n");
    return 3;
  }

 if (argc < 4) {
  free(im->data); free(im);
  fprintf(stderr, "Operation name specified was invalid.\n");
  return 4;
 }

  Image *im_out;
  // exposure
  if (strcmp(argv[3], "exposure") == 0) {
    float ev;
    if (argc != 5 || sscanf(argv[4], "%f", &ev) != 1) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    if (ev < -3 || ev > 3) {
      free(im->data); free(im);
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless.\n");
      return 6;
    }

    im_out = exposure(im, ev);
  }
  // blend
  else if (strcmp(argv[3], "blend") == 0) {
    float a;
    if (argc != 6 || sscanf(argv[5], "%f", &a) != 1) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    FILE *input2 = fopen(argv[4], "rb");
    if (!input2) {
      free(im->data); free(im);
      fprintf(stderr, "Specified input2 file could not be opened.\n");
      return 2;
    }

    Image *im2 = read_ppm(input2);
    if (!im2) {
      free(im->data); free(im);
      fprintf(stderr, "Specified input2 file is not a properly-formatted PPM file, or reading input2 somehow fails.\n");
      return 3;
    }

    if (a < 0 || a > 1) {
      free(im->data); free(im);
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless.\n");
      return 6;
    }

    im_out = blend(im, im2, a);
    free(im2->data); free(im2);
  }
  // zoom in
  else if (strcmp(argv[3], "zoom_in") == 0) {
    if (argc != 4) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    im_out = zoom_in(im);
  }
  // zoom out
  else if (strcmp(argv[3], "zoom_out") == 0) {
    if (argc != 4) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }
    
    if (im->rows <= 1 || im->cols <= 1) {
      free(im->data); free(im);
      fprintf(stderr, "Specified output file could not be opened for writing, or writing output somehow fails.\n");
      return 7;
    }

    im_out = zoom_out(im);
  }
  // pointilism
  else if (strcmp(argv[3], "pointilism") == 0) {
    if (argc != 4) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    im_out = pointilism(im);
  }
  // swirl
  else if (strcmp(argv[3], "swirl") == 0) {
    int cx;
    int cy;
    int s;

    if (argc != 7 || sscanf(argv[4], "%d", &cx) != 1 || sscanf(argv[5], "%d", &cy) != 1 || sscanf(argv[6], "%d", &s) != 1) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    if (s <= 0 || cx < 0 || cy < 0 || cx >= im->cols || cy >= im-> rows) {
      free(im->data); free(im);
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless.\n");
      return 6;
    }

    im_out = swirl(im, cx, cy, s);
  }
  // blur
  else if (strcmp(argv[3], "blur") == 0) {
    double sig;
    if (argc != 5) {
      free(im->data); free(im);
      fprintf(stderr, "Incorrect number of arguments or kind of arguments specified for the specified operation.\n");
      return 5;
    }

    if (sscanf(argv[4], "%lf", &sig) != 1 || sig <= 0) {
      free(im->data); free(im);
      fprintf(stderr, "Arguments for the specified operation were out of range for the given input image, or otherwise senseless.\n");
      return 6;
    }

    im_out = blur(im, sig);
  }
  else {
    free(im->data); free(im);
    fprintf(stderr, "Operation name specified was invalid.\n");
    return 4;
  }

  if (!im_out) {
    free(im->data); free(im);
    fprintf(stderr, "Specified output file could not be opened for writing.\n");
    return 7;
  }

  FILE *output = fopen(argv[2], "wb");
  if (!output) {
    free(im->data); free(im);
    if (strcmp(argv[3], "pointilism") != 0) {
      free(im_out->data); free(im_out);
    }
    fprintf(stderr, "Specified output file could not be opened for writing.\n");
    return 7;
  }

  int num_pixels_written = write_ppm(output, im_out);
  fclose(output);
  if (!num_pixels_written) {
    free(im->data); free(im);
    if (strcmp(argv[3], "pointilism") != 0) {
      free(im_out->data); free(im_out);
    }
    fprintf(stderr, "Writing output somehow fails.\n");
    return 7;
  }

  free(im->data); free(im);
  if (strcmp(argv[3], "pointilism") != 0) {
    free(im_out->data); free(im_out);
  }
  fprintf(stdout, "No errors detected :)\n");
  return 0;
}