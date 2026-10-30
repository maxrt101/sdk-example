#include <target.h>
#include <util.h>

static dev_cio_ctx_t console_ctx = {0};

DEVICE_DECLARE(console, cio, &console_ctx);

tty_t console_tty;

void console_init() {
  device_open(&console, stdout, stdin);

  tty_init(&console_tty, &console);
#if NON_BLOCKING
  console_tty.flags = TTY_FLAG_BELL;
#else
  console_tty.flags = TTY_FLAG_ECHO | TTY_FLAG_BELL;
  tty_set_blocking(&console_tty, false);
#endif
}

__DESTRUCTOR(255) static void console_cleanup() {
  device_close(&console);
}

