#pragma once

#include <platform.h>
#include <device.h>
#include <tty.h>

#if WITH_DEFAULT_LOG_BACKEND
#define DEFAULT_LOG_BACKEND_TTY &console_tty
#endif

DEVICE_EXPORT(console);

extern tty_t console_tty;

error_t target_init();
