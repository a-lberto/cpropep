#include <math.h>
#include <stdio.h>

typedef struct _star
{
  int N;
  double n;
  double e;
  double R;
  double w;
  double r1;
  double r2;
} star_t;

double torad(double deg);

int main(int argc, char *argv[])
{
  
  star_t star;

  double x, y;
  double dx, dy;
  
  star.N = 6;
  star.n = 33.53;
  star.e = 30;
  star.R = 1.5;
  star.w = 0.5;
  star.r1 = 0.0625;
  star.r2 = 0.0625;

  /* header */
  printf("%%!PS-Adobe-1.0\n");
  printf("%%%%BoundingBox: 0 0 595 842\n");
  printf("/cm {72 mul} def\n");
  printf("/n {newpath} def\n");
  printf("/s {stroke} def\n");
  printf("/t {translate} def\n");
  printf("0 setgray\n");
  printf("1.5 setlinewidth\n");
  printf("2 cm 2 cm t\n");
  
  /* external circle */
  
  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n", 0., 0.,
         star.R, 0., 360.);

  printf("0 %d 360 {\n", (int) 360.0/star.N);
  printf(" gsave\n   rotate\n");
  
  /* star shape first part*/
  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n",
         0., 0., (star.R - star.w),
         90.0 - 180.0/star.N,
         90.0 - star.e);

  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n",
         (star.R - star.w - star.r1)*cos(torad(90 - star.e)),
         (star.R - star.w - star.r1)*sin(torad(90 - star.e)),
         star.r1,
         90.0 - star.e,
         180.0 - star.n);

  x = (star.R - star.w - star.r1)*cos(torad(90 - star.e)) -
    star.r1*cos(torad(star.n));
  //  star.r1*cos(torad(180.0 + star.e - star.n - 180.0/star.N));
  y = (star.R - star.w - star.r1)*sin(torad(90 - star.e)) +
    star.r1*sin(torad(star.n));
//    star.r1*sin(torad(180.0 + star.e - star.n - 180.0/star.N));

  dx = -((star.R-star.w-star.r1)*sin(torad(star.e))/sin(torad(star.n)) -
          star.r1*tan(torad(90.0 - star.n)) -
          star.r2*tan(torad(90.0 - star.n))
         )*sin(torad(star.n));

  dy = -((star.R-star.w-star.r1)*sin(torad(star.e))/sin(torad(star.n)) -
          star.r1*tan(torad(90.0 - star.n)) -
          star.r2*tan(torad(90.0 - star.n))
         )*cos(torad(star.n));
  
  printf("n %.10f cm %.10f cm moveto \n", x, y);
  printf("%.10f cm %.10f cm lineto s\n", x+dx, y+dy);

  /* circle */

  x = (star.R - star.w - star.r1)*cos(torad(90 - star.e)) +
    star.r1*cos(torad(180.0 + star.e - star.n - 180.0/star.N)) +
    dx - star.r2*cos(torad(star.n));

  y = (star.R - star.w - star.r1)*sin(torad(90 - star.e)) +
     star.r1*sin(torad(star.n)) +
    dy + star.r2*sin(torad(star.n));

  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n", 0., y, star.r2,
         -180+star.n, -star.n);

  //printf("n -1 cm %.10f cm moveto 1 cm 0 cm rlineto s\n", y);
  
  /* second part */
  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n",
         0., 0., (star.R - star.w),
         90.0 + star.e,
         90.0 + 180.0/star.N);

  printf("n %.10f cm %.10f cm %.10f cm %.10f %.10f arc s\n",
         -(star.R - star.w - star.r1)*cos(torad(90 - star.e)),
         (star.R - star.w - star.r1)*sin(torad(90 - star.e)),
         star.r1,
         star.n,
         90.0 + star.e);

  x = -(star.R - star.w - star.r1)*cos(torad(90 - star.e)) +
    star.r1*cos(torad(star.n));
    //star.r1*cos(torad(star.n + 180.0/star.N - star.e));
  y = (star.R - star.w - star.r1)*sin(torad(90 - star.e)) +
    star.r1*sin(torad(star.n));
    //star.r1*sin(torad(star.n + 180.0/star.N - star.e));
  
  printf("%.10f cm %.10f cm moveto\n", x, y);
  printf("%.10f cm %.10f cm rlineto s\n",-dx, dy);
  
  printf(" grestore\n} for\n");

  printf("showpage\n");
  return 0;
}

double torad(double deg)
{
  return deg*2*M_PI/360;
}
