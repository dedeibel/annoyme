/* xeviedemo.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/X.h>
#include <X11/extensions/Xevie.h>
#include <X11/Xutil.h>


static void
print_key_event (XEvent *ev)
{
   XKeyEvent *key_ev;
   char buffer[20];
   int bufsize = 19;
   KeySym key;
   XComposeStatus compose;
   int char_count;

   key_ev = (XKeyEvent *)ev;

   printf ("        State: %x KeyCode: %x\n", key_ev->state & ShiftMask, key_ev->keycode);
     char_count = XLookupString(key_ev, buffer, bufsize, &key, &compose);
   buffer[char_count] = '\0';
   printf ("        Char Count: %d KeySym: %x char: |%c|\n", char_count, key, buffer[0]);
}

int
main(int argc, char **argv)
{
   Display *dpy;
   int major, minor;
   XEvent  event;
   XClientMessageEvent *xcme;
   int count = 0;
   long delay = 0;
   int ret;

   if (argc > 2) {
     printf ("Usage: xeviedemo delay (in milliseconds)\n");
     exit(1);
   } else if (argc == 2) {
     delay = strtol(argv[1], 0, 0);
   }
   printf("Delay is %d milliseconds\n", delay);
   delay *= 1000;

   dpy = XOpenDisplay(NULL);
   XevieQueryVersion(dpy, &major, &minor);
   printf("major = %d, minor = %d\n", major, minor);
   if(XevieStart(dpy))
     printf("XevieStart(dpy) finished \n");
   else {
     printf("XevieStart(dpy) failed, only one client is allowed to do event interception\n");
     exit(1);
   }
/*
   XevieSelectInput(dpy, KeyPressMask | KeyReleaseMask |
                   ButtonPressMask | ButtonReleaseMask |
                   PointerMotionMask);
*/
   XevieSelectInput(dpy, KeyPressMask | KeyReleaseMask );
   while(1) {
     XNextEvent(dpy, &event);
     xcme = (XClientMessageEvent *)&event;
     /* for readOnly users, send events back to Xserver immediately */
     printf("(%4d)", count++);
     switch(event.type) {
       case KeyPress:
            usleep(delay);
            printf(" KeyPress\n");
            print_key_event (&event);
            break;
       case KeyRelease: printf(" KeyRelease\n"); break;
       case ButtonPress: usleep(delay); printf(" ButtonPress\n"); break;
       case ButtonRelease: printf(" ButtonRelease\n"); break;
       case MotionNotify: printf(" MotionNotify\n"); break;
       case ClientMessage: printf("ClientMessage: <%s>\n", &xcme->data.b[1]); break;
       default: printf(" unknown event %x\n", event.type); break;
     }
     XevieSendEvent(dpy, &event, XEVIE_UNMODIFIED);
     if(count > 10000) {
       break;
     }
   }
   XevieEnd(dpy);
   printf("XevieEnd(dpy) finished \n");
   exit(0);
} 
