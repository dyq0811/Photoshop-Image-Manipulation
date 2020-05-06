// Yingqi Ding (yding50) & Vivian Looi (nlooi1)
// imageManip.c
// 601.220, Spring 2020

#include "imageManip.h"

Image *new_im(int c, int r) {
  Image *im = (Image *) malloc(sizeof(Image));
  if (!im) {
    return NULL;
  }

  im->cols = c;
  im->rows = r;
  im->data = malloc(sizeof(Pixel) * c * r);
  if (!im->data) {
    free(im);
    return NULL;
  }

  return im;
}

double sq(double x) {
  return pow(x, 2);
}

unsigned char color_constraint(double color) {
  if (color > 255)
    return 255;
  if (color < 0)
    return 0;
  return (unsigned char) color;
}

Image *exposure(Image *img, float ev) {
  Image *im = new_im(img->cols, img->rows);
  if (!im) {
    return NULL;
  }

  double constant = pow(2, (double) ev);
  for (int i = 0; i < im->rows * im->cols; i++) {
    im->data[i].r = color_constraint(img->data[i].r * constant);
    im->data[i].g = color_constraint(img->data[i].g * constant);
    im->data[i].b = color_constraint(img->data[i].b * constant);
  }
  return im;
}

Image *blend(Image *im1, Image *im2, float a) {
  int max_c = im1->cols;
  if (im2->cols > max_c)
    max_c = im2->cols;
  int max_r = im1->rows;
  if (im2->rows > max_r)
    max_r = im2->rows;

  Image *im = new_im(max_c, max_r);
  if (!im) {
    return NULL;
  }

  for (int i = 0; i < im->rows * im->cols; i++) {
    int c = i % im->cols;
    int r = i / im->cols;
    int i1 = r * im1->cols + c;
    int i2 = r * im2->cols + c;

    if (c < im1->cols && r < im1->rows) {
      if (c < im2->cols && r < im2->rows) {
        // blend
        im->data[i].r = a * im1->data[i1].r + (1 - a) * im2->data[i2].r;
        im->data[i].g = a * im1->data[i1].g + (1 - a) * im2->data[i2].g;
        im->data[i].b = a * im1->data[i1].b + (1 - a) * im2->data[i2].b;
      }
      else {
        // only im1
        im->data[i].r = im1->data[i1].r;
        im->data[i].g = im1->data[i1].g;
        im->data[i].b = im1->data[i1].b;
      }
    }
    else if (c < im2->cols && r < im2->rows) {
      // only im2
      im->data[i].r = im1->data[i2].r;
      im->data[i].g = im1->data[i2].g;
      im->data[i].b = im1->data[i2].b;
    }
    else {
      // black
      im->data[i].r = (unsigned char) 0;
      im->data[i].g = (unsigned char) 0;
      im->data[i].b = (unsigned char) 0;
    }
  }
  return im;
}

Image *zoom_in(Image *im) {
  Image *im1 = new_im(2 * (im->cols), 2 * (im->rows));
  if (!im1) {
    return NULL;
  }

  // even-numbered rows of new iamge
  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      int new_ind = (i * 2) * im1->cols + (j * 2);
      int orig_ind = i * im->cols + j;
        im1->data[new_ind].r = im->data[orig_ind].r;
        im1->data[new_ind].g = im->data[orig_ind].g;
        im1->data[new_ind].b = im->data[orig_ind].b;
        im1->data[new_ind + 1].r = im->data[orig_ind].r;
        im1->data[new_ind + 1].g = im->data[orig_ind].g;
        im1->data[new_ind + 1].b = im->data[orig_ind].b;
    }
  }
  // odd-numbered rows of new image
  for (int i = 1; i < im1->rows; i = i + 2) {
    for (int j = 0; j < im1->cols; j++) {
      im1->data[i * im1->cols + j].r = im1->data[(i - 1) * im1->cols + j].r;
      im1->data[i * im1->cols + j].g = im1->data[(i - 1) * im1->cols + j].g;
      im1->data[i * im1->cols + j].b = im1->data[(i - 1) * im1->cols + j].b;
    }
  }
  return im1;
 }

Image *zoom_out (Image *im) {
  Image *im1 = new_im((im->cols) / 2, (im->rows) / 2);
  if (!im1) {
    return NULL;
  }

  for (int i = 0; i < im1->rows; i++) {
    for (int j = 0; j < im1->cols; j++) {
    int ind = i * im1->cols + j;
    int p1 = (i * 2) * im->cols + (j * 2);
    int p2 = (i * 2) * im->cols + (j * 2 + 1);
    int p3 = (i * 2 + 1) * im->cols + (j * 2);
    int p4 = (i * 2 + 1) * im->cols + (j * 2 + 1);
      im1->data[ind].r = (im->data[p1].r + im->data[p2].r + im->data[p3].r + im->data[p4].r) / 4;
      im1->data[ind].g = (im->data[p1].g + im->data[p2].g + im->data[p3].g + im->data[p4].g) / 4;
      im1->data[ind].b = (im->data[p1].b + im->data[p2].b + im->data[p3].b + im->data[p4].b) / 4;
    }
  }

  return im1;
}

Image *pointilism(Image *im) {
  int T = 0.03 * im->cols * im->rows;
  for (int t = 0; t < T; t++) {
    // pick random central pixel and radius
    int x = rand() % im->cols;
    int y = rand() % im->rows;
    int center = y * im->cols + x;
    int radius = rand() % 5 + 1;

    // check in square and set the cirle to the same color
    for (int r = y - radius; r <= y + radius; r++) {
      for (int c = x - radius; c <= x + radius; c++) {
        if (r >= 0 && r < im->rows && c >= 0 && c < im->cols) {
          double dist = sqrt(sq(c - x) + sq(r - y));
          if (dist <= (double) radius) {
            int i = r * im->cols + c;
            im->data[i].r = im->data[center].r;
            im->data[i].g = im->data[center].g;
            im->data[i].b = im->data[center].b;
          }
        }
      }
    }
  }
  return im;
}

Image *swirl(Image *im, int x, int y, int s) {
  Image *im1 = new_im(im->cols, im->rows);
  if (!im1) {
    return NULL;
  }

  for (int i = 0; i < im->rows; i++) {
    for (int j = 0; j < im->cols; j++) {
      double a = sqrt(sq(j - x) + sq(i - y)) / s;
      int x1 = (j - x) * cos(a) - (i - y) * sin(a) + x;
      int y1 = (j - x) * sin(a) + (i - y) * cos(a) + y;
      if ((x1 >= 0) && (x1 < im->cols) && (y1 >= 0) && (y1 < im->rows)) {
        im1->data[i * im1->cols + j].r = im->data[y1 * im->cols + x1].r;
        im1->data[i * im1->cols + j].g = im->data[y1 * im->cols + x1].g;
        im1->data[i * im1->cols + j].b = im->data[y1 * im->cols + x1].b;
      }
      else {
        // out of boud, set black
        im1->data[i * im1->cols + j].r = 0;
        im1->data[i * im1->cols + j].g = 0;
        im1->data[i * im1->cols + j].b = 0;
      }
    }
  }
  return im1;
}

Image *blur(Image *im, double sigma) {
  int N = 10 * sigma;
  N += (N % 2 == 0);
  double gs[N][N]; // gaussian matrix
  int radius = N / 2;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int dx = abs(i - radius);
      int dy = abs(j - radius);
      double g = (1.0 / (2.0 * PI * sq(sigma))) * exp(-(sq(dx) + sq(dy)) / (2 * sq(sigma)));
      gs[i][j] = g;
    }
  }

  Image *im_new = new_im(im->cols, im->rows);
  if (!im_new) {
    return NULL;
  }

  for (int idx = 0; idx < im->cols * im->rows; idx++) {
    int c = idx % im->cols;
    int r = idx / im->cols;
    double sum_r = 0;
    double sum_g = 0;
    double sum_b = 0;
    double norm = 0;

    // blur pixels that are not out of bound
    for (int i = r - radius; i <= r + radius; i++) {
      for (int j = c - radius; j <= c + radius; j++) {
        if (i >= 0 && i < im->rows && j >= 0 && j < im->cols) {
          int k = i * im->cols + j;
          double gsi = gs[i - r + radius][j - c + radius];
          norm += gsi;
          sum_r += im->data[k].r * gsi;
          sum_g += im->data[k].g * gsi;
          sum_b += im->data[k].b * gsi;
        }
      }
    }
    // normalization
    im_new->data[idx].r = (unsigned char) (sum_r / norm);
    im_new->data[idx].g = (unsigned char) (sum_g / norm);
    im_new->data[idx].b = (unsigned char) (sum_b / norm);
  }

  return im_new;
}
