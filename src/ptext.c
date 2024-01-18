#include <files.h>
#include <input.h>
#include <output.h>
#include <ptext.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <unistd.h>
#include <utils.h>

struct config conf;

void init(void);
void freeall(void);
void freeall(void);
void done(void);

int main(int argc, char *argv[]) {
  enableRawMode();
  init();
  if (argc >= 2) {
    openFile(argv[1]);
  }
  while (1) {
    refresh();
    procKey();
  }
  return 0;
}

void die(const char *s) {
  perror(s);
  exit(1);
}

void init(void) {
  conf.mod = 1; // no coding
  conf.numrows = 0;
  conf.rows = NULL;
  conf.cx = 0;
  conf.cy = 0;
  conf.dirty = 0;
  conf.rowoff = 0;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  conf.width = w.ws_col;
  conf.height = w.ws_row;
  conf.filename = NULL;
  setStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
  atexit(done);
}

void freeall(void) {
  if (conf.rows != NULL && conf.numrows != 0) {
    int i;
    for (i = 0; i < conf.numrows; i++) {
      free(conf.rows[i].chars);
      free(conf.rows[i].renchar);
      free(conf.rows[i].hl);
    }
    free(conf.rows);
    conf.rows = NULL;
    free(conf.filename);
  }
}

void done(void) {
  freeall();
  disableRawMode();
}
