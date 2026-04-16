#include "editor_test.h"

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  AppEntry app("hello sdl3");
  app.EventLoop();
  return 0;
}
