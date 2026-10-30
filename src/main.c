#include <string.h>
#include <stdio.h>

#include <util.h>
#include <lds.h>
#include <device.h>
#include <tty.h>
#include <shell.h>
#include <log.h>
#include <target.h>

LOGGER_DECLARE(main);

#if 0
typedef uint32_t journal_magic_t;
typedef uint32_t journal_crc_t;

typedef struct journal_entry {
  journal_magic_t magic;
  journal_crc_t   crc;
  uint8_t         size;
  uint8_t         data[];
} journal_entry_t;

// typedef struct journal_header {
//   journal_magic_t magic;
// } journal_header_t;

// typedef struct journal_dev_desc_t {};

typedef struct journal {
  device_t * dev;
} journal_t;
#endif

#if 0
struct test { int cnt; int data; };

LDS_DECLARE(test, struct test);
LDS_DEFINE(test);

LDS_APPEND(test, {.cnt = 0, .data = 42});
LDS_APPEND(test, {.cnt = 1, .data = 69});

static void test() {
  printf("async_init: %d\n", async_init());
#if TEST_DEFINE
  printf("main.c: TEST_DEFINE=1\n");
#endif

  __has_module_async;
  __has_module_test_mod;

  has_module(async);

#define PRINT(__x) printf("%s ", UTIL_STRINGIFY(__x));
  UTIL_FOR_EACH(PRINT, Apple, Banana, Orange);

  printf("\n");

#define PRINT2(__i, __x) printf("[%d] %s\n", __i, UTIL_STRINGIFY(__x));
  UTIL_ENUMERATE(PRINT2, Apple, Banana, Orange);

#define PRINT3(__i) printf("%d ", __i);
  UTIL_REPEAT(3, PRINT3);

  printf("\n");

#define ZERO 0
#define ONE 1
#define NOTHING

  // TODO: Move to module/util/test
  __STATIC_ASSERT(UTIL_IF_1(ONE, 1, 0) == 1, "");
  __STATIC_ASSERT(UTIL_IF_1(ZERO, 1, 0) == 0, "");
  __STATIC_ASSERT(UTIL_IF_0(ZERO, 1, 0) == 1, "");
  __STATIC_ASSERT(UTIL_IF_0(ONE, 1, 0) == 0, "");
  __STATIC_ASSERT(UTIL_IF_EMPTY(NOTHING, 1, 0) == 1, "");
  __STATIC_ASSERT(UTIL_IF_EMPTY(ONE, 1, 0) == 0, "");
  __STATIC_ASSERT(UTIL_IF_EMPTY(ZERO, 1, 0) == 0, "");
  __STATIC_ASSERT(UTIL_IF_EMPTY(, 1, 0) == 1, "");

  LDS_FOR_EACH(test, t) {
    printf("%p: test { .cnd = %d, .data = %d }\n", t, t->cnt, t->data);
  }

#define PRED(__x) __x->cnt == 1
  printf("%d\n", LDS_GET_BY_PRED(test, PRED)->data);

  printf("%d\n", LDS_GET_BY(test, cnt, 1)->data);
}
#endif

#if 0
static void echo() {
  bool run = true;
  char buf[64];

  while (run) {
    if (tty_readline(&console_tty, buf, sizeof(buf)) == E_OK) {
      tty_printf(&console_tty, "received: '%s'\n", buf);

      if (!strcmp(buf, "q")) {
        run = false;
      }

      memset(buf, 0, sizeof(buf));
    }
  }
}
#endif

static bool run = true;

#if has_module(shell)
static int8_t sh_q(shell_t * sh, uint8_t argc, char ** argv) {
  run = false;
  return 0;
}

SHELL_CMD_DECLARE(q, sh_q, "Quit", "Quits the loop");

static int8_t sh_atest(shell_t * sh, uint8_t argc, char ** argv) {
  for (int i = 0; i < argc; ++i) {
    tty_printf(sh->tty, "%s\n", argv[i]);
  }
  return 0;
}

SHELL_CMD_DECLARE(atest, sh_atest, "Arg test", "Prints all arguments");

static int8_t sh_test_help(shell_t * sh, uint8_t argc, char ** argv) {
#define PRED(__x) !strcmp(__x->name, "test")
  tty_puts(sh->tty, LDS_GET_BY_PRED(shell_cmd, PRED)->help);
  return 0;
}

static int8_t sh_test_do(shell_t * sh, uint8_t argc, char ** argv) {
  for (uint8_t i = 0; i < argc; ++i) {
    tty_printf(sh->tty, "%s\n", argv[i]);
  }
  return 0;
}

SHELL_SUBCMD_DECLARE(
  test_subcmd,
  (help, sh_test_help, "Run help"),
  (do,   sh_test_do,   "Do (arg) test")
);

static int8_t sh_test(shell_t * sh, uint8_t argc, char ** argv) {
  int8_t result = 0;

  if (shell_subcmd_execute(sh, &result, test_subcmd, argc, argv) == E_OK) {
    return result;
  }

  if (argc) {
    tty_printf(sh->tty, "Unknown subcommand '%s' for command test\n", argv[0]);
  } else {
    tty_puts(sh->tty, "Subcommand expected for command test\n");
  }

  return 1;
}

SHELL_CMD_DECLARE(test, sh_test, "subcmd test", "Test of SUBCMD mechanism");

static int8_t sh_colortest(shell_t * sh, uint8_t argc, char ** argv) {
  tty_colortest(sh->tty);
  return 0;
}

SHELL_CMD_DECLARE(colortest, sh_colortest, "color test", "ANSI Color Codes Test");
#endif

int main() {
  target_init();

  log_info("Initialized");
  log_printf(LOG_DEBUG, "Raw log");

  LDS_FOR_EACH(shell_cmd, cmd) {
    tty_printf(&console_tty, "%s\r\n", cmd->name);
  }

  shell_t sh;

  shell_init(&sh, &console_tty);

  log_info("shell_execute_multiline start");
  shell_execute_multiline(&sh, "help test; log; log main 0; log");
  log_info("shell_execute_multiline end");

  uint32_t cycle = 0;

  while (run) {
    // if (cycle % 5000000 == 0) {
    //   tty_printf(&console_tty, "\rTest cycle %d\r\n", cycle);
    // }
    shell_process(&sh);
    cycle++;
  }

  return 0;
}
