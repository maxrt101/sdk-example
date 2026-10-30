#include <target.h>

void console_init();

error_t target_init() {
  console_init();
  return E_OK;
}
