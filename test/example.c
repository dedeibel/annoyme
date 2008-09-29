#include <X11/Xlib.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 
 int main(int argc, char *argv[]) {
   Display *d;
   Window w;
   XEvent e;
   char *msg = "Hello, World!";
   int s;
 
                        /* open connection with the server */
   d = XOpenDisplay(NULL);
   if (d == NULL) {
     printf("Cannot open display\n");
     exit(1);
   }
 
   s = DefaultScreen(d);
 
                        /* create window */
   w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                           BlackPixel(d, s), WhitePixel(d, s));
 
                        /* select kind of events we are interested in */
   XSelectInput(d, w, ExposureMask | KeyPressMask);
   XGrabButton(d, AnyButton, AnyModifier, DefaultRootWindow(d),
               True, ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync,
               None, None);
 
                        /* map (show) the window */
   XMapWindow(d, w);
   int c = 0;
 
                        /* event loop */
   while (1) {
     XNextEvent(d, &e);
                        /* draw or redraw the window */
     if (e.type == Expose) {
       XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
       XDrawString(d, w, DefaultGC(d, s), 50, 50, msg, strlen(msg));
     }
                        /* exit on key press */
     if (e.type == KeyPress)
     {
       printf("KEY PRESSED\n");
       if (c++ > 10)    break;
     }
   }
 
                        /* close connection to server */
   XCloseDisplay(d);
 
   return 0;
 }

