#include <stdio.h>
#include <math.h>

int main(int argc, char **argv) {
  long double s, h, area, x;
  long n = 1000000000;
  int i;

  // find point where circles intersect
  s = sqrtl(3.5L) - 1;

  // width of rectangle
  h = s / n;

  // sum area of each rectangle
  for (i = 1; i < n; i++) {
    x = i * h;
    area += (sqrtl(9-(x*x)) - 2 - s);
  }

  // double area to account for symmetry in first quadrant
  area *= h*2;
  // add area of square between origin and circle intersection
  area += (s*s);

  // multiply area by 4 to account for radial symmetry around origin
  printf("Area=%.14Le\n", area * 4);
}
