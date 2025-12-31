
#include "usart.h"

#define PORT huart1

extern UART_HandleTypeDef PORT;

/*# Retarget printf to UART (std library and toolchain dependent) #########*/

#if defined(__GNUC__)

#include <stdbool.h>
#include <string.h>
#include "components/ring_buf/ring_buf.h"

static uint8_t tx_buf_data[512];
static struct ring_buf tx_buf = {
    .head = tx_buf_data,
    .tail = tx_buf_data,
    .data = tx_buf_data,
    .len = ARRAY_SIZE(tx_buf_data),
    .cnt = 0,
    .size = sizeof(tx_buf_data[0]),
};
static uint8_t dma_buf[64] = {0};

static volatile bool dma_tx_done = true;

static void tx_dma_tranfer(void) {
    uint16_t cnt = 0;
    memset(dma_buf, 0, sizeof(dma_buf));

    // Pop data from ring buffer
    for (cnt = 0; cnt < sizeof(dma_buf) && tx_buf.cnt; cnt++){
        ring_buf_pop(&tx_buf, (void *)&dma_buf[cnt]);
    }

    // Transmit data
    HAL_UART_Transmit_DMA(&huart1, dma_buf, cnt);
    dma_tx_done = false;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    (void) huart;

    dma_tx_done = true;

    // Check Tx buffer is empty
    if(tx_buf.cnt) {
        tx_dma_tranfer();
    }
}

int _write(int fd, char * ptr, int len)
{
  (void) fd;

  // Push data to ring buffer
  for (int i = 0; i < len; i++) {
    while (tx_buf.cnt >= tx_buf.len){
      HAL_Delay(5);
    }
        
    ring_buf_push(&tx_buf, (void *)ptr);
    ptr++;
  }

  if (dma_tx_done) {
    tx_dma_tranfer();
  }

  return len;
}
#elif defined (__ICCARM__)
#include "LowLevelIOInterface.h"
size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  HAL_UART_Transmit(&PORT, (uint8_t *) buffer, size, HAL_MAX_DELAY);
  return size;
}
#elif defined (__CC_ARM)
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&PORT, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

// OR:

// Add syscalls.c with GCC

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&PORT, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}
