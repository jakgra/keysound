#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void play_sound(char *keysym) {
  const char *exec_str = "aplay sounds/%s.wav&";
  char *real_exec_str;
  int len;

  len = strlen(exec_str) + strlen(keysym) + 1;
  real_exec_str = malloc(len * sizeof(char));
  snprintf(real_exec_str, len, exec_str, keysym);
  system(real_exec_str);
  free(real_exec_str);
}

int main() {

  Display *dis;
  char keys_return[32];
  char *keysym;
  int i;
  int j;
  struct timespec sleep_well;

  sleep_well.tv_sec = 0;
  sleep_well.tv_nsec = 120000000;
  dis = XOpenDisplay(NULL);
  while (1) {
    XQueryKeymap(dis, keys_return);
    for (i = 0; i < 32; ++i) {
      if (keys_return[i]) {
        for (j = 0; j < 8; ++j) {
          if (keys_return[i] & (1 << j)) {
            keysym = XKeysymToString(XkbKeycodeToKeysym(dis, i * 8 + j, 0, 0));
            play_sound(keysym);
            printf("Playing sound for keycode %s\n", keysym);
            nanosleep(&sleep_well, NULL);
            goto outer_loop;
          }
        }
      }
    }
  outer_loop:
    continue;
  }
  return 0;
}
