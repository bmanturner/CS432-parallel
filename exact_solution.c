#include <stdio.h>
#include <math.h>

int main(int argc, char **argv) {
  long double s, h, area, x;
  long n = 1000000000;
  int i;



  s = sqrtl(3.5L) - 1;
  h = s / n;

  printf("%.60Le\n", h);

  area = ((sqrtl(9) - 2 - s) + (sqrtl(9-(s*s)) - 2 - s))/2;
  for (i = 1; i < n; i++) {
    x = i * h;
    area += (sqrtl(9-(x*x)) - 2 - s);
  }

  area *= h*2;
  area += (s*s);
  printf("Area=%.15Le\n", area * 4);
}
