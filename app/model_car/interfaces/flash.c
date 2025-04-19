/*
 * @Author: andy.chang 
 * @Date: 2024-11-25 22:32:13 
 * @Last Modified by: andy.chang
 * @Last Modified time: 2024-12-31 13:09:22
 */

#include "interface.h"

#include <string.h>
#include "main.h"
#include "errno.h"

#include "components/log/log.h"
#define TAG "FLASH"

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct = {0};

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr) {
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

/**
 * @brief 
 * 
 * @param addr 
 * @param len 
 * @return int 
 */
int flash_erase(uint32_t addr, uint32_t len) {
    uint32_t FirstPage = 0, NbOfPages = 0, PageError = 0;

    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    /* Get the 1st page to erase */
    FirstPage = GetPage(addr);

    /* Get the number of pages to erase from 1st page */
    NbOfPages = GetPage(addr+len) - FirstPage + 1;

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Page        = FirstPage;
    EraseInitStruct.NbPages     = NbOfPages;

    /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
       you have to make sure that these data are rewritten before they are accessed during code
       execution. If this cannot be done safely, it is recommended to flush the caches by setting the
       DCRST and ICRST bits in the FLASH_CR register. */
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
        /*
          Error occurred while page erase.
          User can add here some code to deal with this error.
          PageError will contain the faulty page and then to know the code error on this page,
          user can call function 'HAL_FLASH_GetError()'
        */
        /* Lock the Flash to disable the flash control register access (recommended
           to protect the FLASH memory against possible unwanted operation) *********/
        HAL_FLASH_Lock();
        LOGE(TAG, "Flash erase fail!");
        return -EIO;
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();

    return 0;
}

/**
 * @brief 
 * 
 * @param addr 
 * @param data 
 * @param len 
 * @return int 
 */
int flash_write(uint32_t addr, uint8_t *data, uint32_t len) {
    /* Unlock the Flash to enable the flash control register access *************/
    HAL_FLASH_Unlock();

    while (addr < addr+len) {
        uint64_t buf = 0;
        if (len >= sizeof(buf)) {
            memcpy(&buf, data, sizeof(buf));
            data += sizeof(buf);
            len -= sizeof(buf);
        } else {
            memcpy(&buf, data, sizeof(len));
            data += len;
            len = 0;
        }
    
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, addr, buf) == HAL_OK) {
          addr += sizeof(buf);
        } else {
            /* Error occurred while writing data in Flash memory.
               User can add here some code to deal with this error */
            LOGE(TAG, "Flash write fail!");

            /* Lock the Flash to disable the flash control register access (recommended
               to protect the FLASH memory against possible unwanted operation) *********/
            HAL_FLASH_Lock();
            return -EIO;
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    HAL_FLASH_Lock();
    
    return 0;
}

/**
 * @brief 
 * 
 * @param addr 
 * @param data 
 * @param len 
 * @return int 
 */
int flash_read(uint32_t addr, uint8_t *data, uint32_t len) {
    
    while (addr < addr+len) {
        uint32_t buf = *(uint32_t *)addr;
        if (len >= sizeof(buf)) {
            memcpy(data, &buf, sizeof(buf));
            data += sizeof(buf);
            len -= sizeof(buf);
        } else {
            memcpy(data, &buf, sizeof(len));
            data += len;
            len = 0;
        }
        addr += sizeof(buf);
    }
    return 0;
}


/**
 * @brief 
 * 
 * @return int 
 */
int flash_init(void) {
    
    return 0;
}
