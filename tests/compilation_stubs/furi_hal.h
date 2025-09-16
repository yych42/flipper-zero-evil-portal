#pragma once

#include <stdint.h>
#include <stddef.h>

typedef enum {
  FuriHalUartIdUSART1 = 0,
} FuriHalUartId;

#ifdef __cplusplus
extern "C" {
#endif

void furi_hal_uart_tx(FuriHalUartId channel, const uint8_t *data, size_t size);
void furi_hal_uart_set_br(FuriHalUartId channel, uint32_t baudrate);
void furi_hal_uart_set_irq_cb(FuriHalUartId channel,
                              void (*callback)(uint8_t event, uint8_t data,
                                               void *context),
                              void *context);
void furi_hal_console_disable(void);
void furi_hal_console_enable(void);

#ifdef __cplusplus
}
#endif
