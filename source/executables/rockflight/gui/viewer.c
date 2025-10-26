
#include <X11/Xlib.h> // Every Xlib program must include this
#include <linux/mc146818rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>   // So we got the profile for 10 seconds

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <errno.h>

#include "rocflight.h"
#include "num.h"
#include "state.h"


#define PIXEL_M 15
#define PIXEL_M_S 15

#define H 40
#define L 40
#define SCALEY 1

#define WIDTH  L * PIXEL_M
#define HEIGHT (int)(H * PIXEL_M * SCALEY)

#define MAX_HISTORY 200
#define FREQ  32
#define STEP (1.0/FREQ)

#define POSX_TO_PIXEL(x) (WIDTH/2 + (int)((x) * PIXEL_M))
#define POSY_TO_PIXEL(y) (HEIGHT - (int)((y) * PIXEL_M * SCALEY))


struct rocketAnim_t 
{
  int width, height;

  GC gc;
  GC tmpgc;
  Display *display;
  Window window;
  Pixmap pixmap;
  unsigned long red, blue, white, green;

  XFontStruct *font_info;
  int font_height;
  
} rocket;



int eom(int neq, double time, double *y, double *dy, void *data);



unsigned long AllocColor(char *colName)
{
  Colormap cmap;
  XColor c0, c1;
  cmap = DefaultColormap(rocket.display, 0);
  XAllocNamedColor(rocket.display, cmap, colName, &c1, &c0);
  return(c1.pixel);
}

void ExposeScreen(void)
{
  XCopyArea(rocket.display, rocket.pixmap,
            rocket.window,  rocket.tmpgc,
            0, 0, rocket.width, rocket.height, 0, 0);
}

void rocketInit(void)
{
  Display *d;
  Window   w;
  Pixmap   p;
  XEvent   e;
  
  d = rocket.display = XOpenDisplay(NULL);
  
  w = rocket.window = XCreateSimpleWindow(d, DefaultRootWindow(d),
                                          0, 0,
                                          rocket.width, rocket.height,
                                          2, BlackPixel(d, 0),
                                          WhitePixel(d, 0));
  
  p = rocket.pixmap = XCreatePixmap(d, DefaultRootWindow(d),
                                    rocket.width, rocket.height,
                                    DefaultDepth(d, 0));

  rocket.gc     = XCreateGC(d, w, 0, 0);
  rocket.tmpgc  = XCreateGC(d, w, 0, 0);
  rocket.red    = AllocColor("red");
  rocket.blue   = AllocColor("blue");
  rocket.white  = AllocColor("white");
  rocket.green  = AllocColor("green");

  rocket.font_info = XQueryFont(d, XGContextFromGC(rocket.gc));
  rocket.font_height = rocket.font_info->ascent + rocket.font_info->descent;
  
  XSetForeground(rocket.display, rocket.gc, rocket.white);
  XFillRectangle(d, p, rocket.gc, 0, 0, rocket.width, rocket.height);
  XMapWindow(d, w);
  XSelectInput(d, w, StructureNotifyMask);

  for(;;) {
    XNextEvent(d, &e);
    if (e.type == MapNotify)
      break;
  }
  XFlush(d);
}

void DrawLine(int x1, int y1, int x2, int y2)
{ 
  /* Draw the new line */
  XDrawLine(rocket.display, rocket.pixmap, rocket.gc, x1, y1, x2, y2);
}

void DrawPoint(int x, int y)
{
  /* draw the point */
  XSetForeground(rocket.display, rocket.gc, rocket.red);
  XDrawPoint(rocket.display, rocket.pixmap, rocket.gc, x, y);
}

void DrawFillCircle(int x, int y, int r)
{
  /* Draw the circle */
  XSetForeground(rocket.display, rocket.gc, rocket.red);
  XFillArc(rocket.display, rocket.pixmap, rocket.gc,
           x - r, y - r, 2*r, 2*r, 0, 360*64);
}

void DrawSystem(double *y)
{
  int i, j;

  double x1, y1;

  double theta;
  double vitesse_tot;

  char vitessex[32];
  char vitessey[32];
  char vitesset[32];

  char alpha_str[32];

  char theta_str[32];
  char velocity_str[32];
  
  double velocity;

  theta = y[10];
  
  velocity = sqrt(y[0]*y[0] + y[1]*y[1] + y[2]*y[2]);
  
  vitesse_tot =
    sqrt(pow(y[0], 2) + pow(y[2], 2));

  sprintf(velocity_str, "Velocity : % .2f m/s", velocity);
  sprintf(alpha_str, "Angle of attack : % 6.2f deg", atan(y[2]/y[0])*180/M_PI);
  sprintf(theta_str, "Theta : % .2f deg", theta*180/M_PI);
  
  sprintf(vitessex, "Vitesse axe x  : % .2f", y[0]);
  sprintf(vitessey, "Vitesse axe y  : % .2f", y[2]);
  sprintf(vitesset, "Vitesse absolue: % .2f", vitesse_tot);
  
  /* Clear the screen */
  XSetForeground(rocket.display, rocket.gc, rocket.white);
  XFillRectangle(rocket.display, rocket.pixmap,
                 rocket.gc, 0, 0, rocket.width, rocket.height);
  
  //DrawFillCircle(POSX_TO_PIXEL(x), POSY_TO_PIXEL(y), 10);
  //printf("%f\n", y[10]);


  /* Draw the rocket */
  XSetForeground(rocket.display, rocket.gc, rocket.red);

  x1 = 0 + 8*cos(theta);
  y1 = 20 + 8*sin(theta);
  
  DrawLine(POSX_TO_PIXEL(0 - sin(theta)), POSY_TO_PIXEL(20 + cos(theta)),
           POSX_TO_PIXEL(x1 - sin(theta)), POSY_TO_PIXEL(y1 + cos(theta)));
  DrawLine(POSX_TO_PIXEL(0 + sin(theta)), POSY_TO_PIXEL(20 - cos(theta)),
           POSX_TO_PIXEL(x1 + sin(theta)), POSY_TO_PIXEL(y1 - cos(theta)));
  DrawLine(POSX_TO_PIXEL(x1 - sin(theta)), POSY_TO_PIXEL(y1 + cos(theta)),
           POSX_TO_PIXEL(x1 + 4*cos(theta)), POSY_TO_PIXEL(y1 + 4*sin(theta)));
  DrawLine(POSX_TO_PIXEL(x1 + sin(theta)), POSY_TO_PIXEL(y1 - cos(theta)),
           POSX_TO_PIXEL(x1 + 4*cos(theta)), POSY_TO_PIXEL(y1 + 4*sin(theta)));
  
  
  XSetForeground(rocket.display, rocket.gc, rocket.blue);


  x1 = (y[0]*cos(theta) - y[2]*sin(theta))/50;
  y1 = 20 + (y[0]*sin(theta) + y[2]*cos(theta))/50;
  
  DrawLine(POSX_TO_PIXEL(0), POSY_TO_PIXEL(20),
           POSX_TO_PIXEL(x1), POSY_TO_PIXEL(y1));
  
  //DrawLine(x, y, 
    
/*
  if (modeleInfo.full)
    j = MAX_HISTORY;
  else
    j = modeleInfo.lasthistory;

  for (i = 0; i < j; i++)
    DrawPoint(POSX_TO_PIXEL(modeleInfo.pointhistory[i][0]),
              POSY_TO_PIXEL(modeleInfo.pointhistory[i][1]));
*/ 
  

  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 10,
              HEIGHT - 3*rocket.font_height, velocity_str,
              strlen(velocity_str));

  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 10,
              HEIGHT - 2*rocket.font_height, alpha_str, strlen(alpha_str));

  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 10,
              HEIGHT - rocket.font_height, theta_str, strlen(theta_str));

  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 100,
  	      HEIGHT - 3*rocket.font_height, vitessex,
              strlen(vitessex));
  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 100,
              HEIGHT - 2*rocket.font_height, vitessey,
              strlen(vitessey));
  XDrawString(rocket.display, rocket.pixmap, rocket.gc, 100,
              HEIGHT - 1*rocket.font_height, vitesset,
              strlen(vitesset));



/*  XFillRectangle(rocket.display, rocket.pixmap, rocket.gc, 140,
                 HEIGHT - 4*rocket.font_height + rocket.font_info->ascent/2,
                 (int)(PIXEL_M_S * fabs(modeleInfo.state[2])),
                 rocket.font_height/2);

  XFillRectangle(rocket.display, rocket.pixmap, rocket.gc, 140,
                 HEIGHT - 3*rocket.font_height + rocket.font_info->ascent/2,
                 (int)(PIXEL_M_S * fabs(modeleInfo.state[3])),
                 rocket.font_height/2);

  XFillRectangle(rocket.display, rocket.pixmap, rocket.gc, 140,
                 HEIGHT - 2*rocket.font_height + rocket.font_info->ascent/2,
                 (int)(PIXEL_M_S * vitesse_tot),
                 rocket.font_height/2);
  
*/
  
  ExposeScreen();
  XFlush(rocket.display);
}






int main(int argc, char *argv[])
{

    int i, n;
  int neq = 12;
 
  double time;
  double res[12];

  double *ans;
  double *init_cond;

  double u      = 100;  /* velocity component in x */
  double v      = 0;  /* velocity component in y */
  double w      = 0;  /* velocity component in z */
  
  double P      = 0;  /* angular velocity */
  double Q      = 0;  /* */
  double R      = 0;  /* */

  double lambda = 0*(M_PI/180);   /* latitude (positive north) */
  double mu     = 0*(M_PI/180);   /* longitude (positive east) */
  double Re     = EARTH_RAD + 5000;      /* distance from earth center */
  double phi    = 0*(M_PI/180);   /*  */
  double theta  = 45*(M_PI/180);   /* positive up */
  double psi    = 0*(M_PI/180); /* positive cw*/


//  double dt = 0.05;
  double duration = 10;

  state_t s;
  
  
  int fd, retval;
  unsigned long tmp, data;
  struct rtc_time rtc_tm;

  /* Initialize the window */
  rocket.width  = WIDTH;
  rocket.height = HEIGHT;
  rocketInit();


  /* open and configure the realtime clock */
  fd = open ("/dev/rtc", O_RDONLY);
  if (fd ==  -1)
  {
    perror("/dev/rtc");
    exit(errno);
  }

  /* Set the periodic IRQ rate to FREQ Hz */
  retval = ioctl(fd, RTC_IRQP_SET, FREQ);
  if (retval == -1)
  {
		perror("ioctl");
		exit(errno);
	}

  /* Enable periodic interrupts */
	retval = ioctl(fd, RTC_PIE_ON, 0);
	if (retval == -1)
  {
		perror("ioctl");
		exit(errno);
	}

  //initModele();


  s.m = 10.0;

  s.Ix = 25e-3;
  s.Iy = 3;
  s.Iz = 3;

  s.Faero[0] = 0;
  s.Faero[1] = 0;
  s.Faero[2] = 0;

  s.Maero[0] = 0;
  s.Maero[1] = 0;
  s.Maero[2] = 0;
    
  init_cond = (double *) malloc(sizeof(double) * neq);
  
  s.L_BV[0][0] = cos(theta)*cos(psi);
  s.L_BV[0][1] = cos(theta)*sin(psi);
  s.L_BV[0][2] =-sin(theta);
  s.L_BV[1][0] = sin(phi)*sin(theta)*cos(psi) - cos(phi)*sin(psi);
  s.L_BV[1][1] = sin(phi)*sin(theta)*sin(psi) + cos(phi)*cos(psi);
  s.L_BV[1][2] = sin(phi)*cos(theta);
  s.L_BV[2][0] = cos(phi)*sin(theta)*cos(psi) + sin(phi)*sin(psi);
  s.L_BV[2][1] = cos(phi)*sin(theta)*sin(psi) - sin(phi)*cos(psi);
  s.L_BV[2][2] = cos(phi)*cos(theta);
  
  init_cond[0]  = u;             /* velocity component in x */
  init_cond[1]  = v;             /* velocity component in y */
  init_cond[2]  = w;             /* velocity component in z */
  init_cond[3]  = P+ (s.L_BV[0][0]*WE*cos(lambda)-s.L_BV[0][2]*WE*sin(lambda));
  init_cond[4]  = Q+ (s.L_BV[1][0]*WE*cos(lambda)-s.L_BV[1][2]*WE*sin(lambda));
  init_cond[5]  = R+ (s.L_BV[2][0]*WE*cos(lambda)-s.L_BV[2][2]*WE*sin(lambda));
  init_cond[6]  = lambda;  /* latitude  */
  init_cond[7]  = mu;      /* longitude */
  init_cond[8]  = Re;      /* radius */
  init_cond[9]  = phi;     /* phi */
  init_cond[10] = theta;   /* theta */
  init_cond[11] = psi;     /* psi */

  time = 0;
  
//  eom(neq, time, init_cond, res, &s);
/*
  fprintf(stderr, "% e (u dot)\n", res[0]);
  fprintf(stderr, "% e (v dot)\n", res[1]);
  fprintf(stderr, "% e (w dot)\n", res[2]);
  fprintf(stderr, "% e (p dot)\n", res[3]);
  fprintf(stderr, "% e (q dot)\n", res[4]);
  fprintf(stderr, "% e (r dot)\n", res[5]);
  fprintf(stderr, "% e (lambda dot)\n", res[6]);
  fprintf(stderr, "% e (mu dot)\n", res[7]);
  fprintf(stderr, "% e (R dot)\n", res[8]);
  fprintf(stderr, "% e (phi dot)\n", res[9]);
  fprintf(stderr, "% e (theta dot)\n", res[10]);
  fprintf(stderr, "% e (psi dot)\n", res[11]);
*/

  n = NUM_rk4 (eom, neq, STEP, duration, init_cond, &ans, &s);
  

  
  /* set the initial conditions */
  //modeleInfo.state[0] = 7.0;
  //modeleInfo.state[1] = 25.0;//.3944;
  //modeleInfo.state[2] = 0.0;
  //modeleInfo.state[3] = 0.0;


//  for( i = 0; i < n; i++)
//  {
//    printf("% .12e ", ans[0 + neq*i]); /* u */
//    printf("% .12e ", ans[1 + neq*i]); /* v */
//    printf("% .12e ", ans[2 + neq*i]); /* w */
//    printf("% .12e ", ans[3 + neq*i]); /* p */
//    printf("% .12e ", ans[4 + neq*i]); /* q */
//    printf("% .12e ", ans[5 + neq*i]); /* r */
//    printf("% .12e ", ans[6 + neq*i]); /* lambda */
//    printf("% .12e ", ans[7 + neq*i]); /* mu */
//    printf("% .12e ", ans[8 + neq*i]); /* R */
//    printf("% .12e ", ans[9 + neq*i]); /* phi */
//    printf("% .12e ", ans[10 + neq*i]); /* theta */
//    printf("% .12e \n", ans[11 + neq*i]); /* psi */
//  }

    
//  DrawSystem(y);
  
  for (i = 0; i < n; i++)
  {
		/* This blocks */
		retval = read(fd, &data, sizeof(unsigned long));
		if (retval == -1)
    {
			perror("read");
			exit(errno);
		}

    DrawSystem(ans + neq*i);    
	}

  /* Disable periodic interrupts */
	retval = ioctl(fd, RTC_PIE_OFF, 0);
	if (retval == -1)
  {
		perror("ioctl");
		exit(errno);
	}
  
  
  free(init_cond);
  //free(ans);

  return 0;
}
