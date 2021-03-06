/***************************************************************************//**
* \file cyhal_sdhc.h
*
* \brief
* Provides a high level interface for interacting with the Cypress SDHC.
* This interface abstracts out the chip specific details. If any chip specific
* functionality is necessary, or performance is critical the low level functions
* can be used directly.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

/**
* \addtogroup group_hal_sdhc SDHC (SD Host Controller)
* \ingroup group_hal
* \{
* High level interface for interacting with the Cypress SDHC.
*
* \defgroup group_hal_sdhc_macros Macros
* \defgroup group_hal_sdhc_functions Functions
* \defgroup group_hal_sdhc_data_structures Data Structures
* \defgroup group_hal_sdhc_enums Enumerated Types
*/

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "cy_result.h"
#include "cyhal_hw_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
* \addtogroup group_hal_sdhc_enums
* \{
*/

/** Card types */
typedef enum
{
    CYHAL_SDHC_SD, //!< Secure Digital card
    CYHAL_SDHC_SDIO, //!< CD Input Output card
    CYHAL_SDHC_EMMC, //!< Embedded Multimedia card
    CYHAL_SDHC_COMBO, //!< Combo Card (SD + SDIO)
    CYHAL_SDHC_UNUSABLE, //!< Unusable card or unsupported type
    CYHAL_SDHC_NOT_EMMC, //!< Not an eMMC card
} cyhal_sdhc_card_type_t;

/** SDHC interrupt triggers */
typedef enum {
    CYHAL_SDHC_CMD_COMPLETE, //!> Command Complete
    CYHAL_SDHC_XFER_COMPLETE, //!> Host read/write transfer is complete
    CYHAL_SDHC_BGAP_EVENT, //!> This bit is set when both read/write transaction is stopped
    CYHAL_SDHC_DMA_INTERRUPT, //!> Host controller detects an SDMA Buffer Boundary during transfer
    CYHAL_SDHC_BUF_WR_READY, //!> This bit is set if the Buffer Write Enable changes from 0 to 1
    CYHAL_SDHC_BUF_RD_READY, //!> This bit is set if the Buffer Read Enable changes from 0 to 1
    CYHAL_SDHC_CARD_INSERTION, //!> This bit is set if the Card Inserted in the Present State
    CYHAL_SDHC_CARD_REMOVAL, //!> This bit is set if the Card Inserted in the Present State
    CYHAL_SDHC_CARD_INTERRUPT, //!> The synchronized value of the DAT[1] interrupt input for SD mode
    CYHAL_SDHC_INT_A,
    CYHAL_SDHC_INT_B,
    CYHAL_SDHC_INT_C,
    CYHAL_SDHC_RE_TUNE_EVENT, //!> This bit is set if the Re-Tuning Request changes from 0 to 1
    CYHAL_SDHC_FX_EVENT, //!> This status is set when R[14] of response register is set to 1
    CYHAL_SDHC_CQE_EVENT, //!> This status is set if Command Queuing/Crypto event has occurred
    CYHAL_SDHC_ERR_INTERRUPT, //!> If any of the bits in the Error Interrupt Status register are set
    CYHAL_SDHC_ALL_INTERRUPTS, //!> Is used to enable/disable all interrupts
} cyhal_sdhc_irq_event_t;

/** \} group_hal_sdhc_enums */

/**
* \addtogroup group_hal_sdhc_data_structures
* \{
*/

/** Handler for SDHC interrupts */
typedef void (*cyhal_sdhc_irq_handler_t)(void *handler_arg, cyhal_sdhc_irq_event_t event);

/** Defines configuration options for the SDHC block */
typedef struct
{
    bool                 enableLedControl; //!< Drive one IO to indicate when the card is being accessed
    bool                 lowVoltageSignaling; //!< Whether 1.8V signaling is supported
    uint8_t              busWidth; //!< The desired bus width
} cyhal_sdhc_config_t;

/** \} group_hal_sdhc_data_structures */


/**
* \addtogroup group_hal_sdhc_functions
* \{
*/

/** Initialize the SDHC peripheral
 *
 * @param[out] obj               The SDHC object
 * @param[out] clk               The pin connected to the clk signal
 * @param[in]  cmd               The pin connected to the command signal
 * @param[in]  data0             The pin connected to the data0 signal
 * @param[in]  data1             The pin connected to the data1 signal
 * @param[in]  data2             The pin connected to the data2 signal
 * @param[in]  data3             The pin connected to the data3 signal
 * @param[in]  data4             The pin connected to the data4 signal
 * @param[in]  data5             The pin connected to the data5 signal
 * @param[in]  data6             The pin connected to the data6 signal
 * @param[in]  data7             The pin connected to the data7 signal
 * @param[in]  cardDetect        The pin connected to the cardDetect signal
 * @param[out] ioVoltSel         The pin connected to the ioVoltSel signal
 * @param[out] cardIfPwrEn       The pin connected to the cardIfPwrEn signal
 * @param[in]  cardMechWriteProt The pin connected to the cardMechWriteProt signal
 * @param[in]  ledCtrl           The pin connected to the ledCtrl signal
 * @param[in]  cardEmmcReset     The pin connected to the cardEmmcReset signal
 * @return The status of the init request
 */
cy_rslt_t cyhal_sdhc_init(cyhal_sdhc_t *obj,
                   cyhal_gpio_t cmd,
                   cyhal_gpio_t clk,
                   cyhal_gpio_t data0,
                   cyhal_gpio_t data1,
                   cyhal_gpio_t data2,
                   cyhal_gpio_t data3,
                   cyhal_gpio_t data4,
                   cyhal_gpio_t data5,
                   cyhal_gpio_t data6,
                   cyhal_gpio_t data7,
                   cyhal_gpio_t cardDetect,
                   cyhal_gpio_t ioVoltSel,
                   cyhal_gpio_t cardIfPwrEn,
                   cyhal_gpio_t cardMechWriteProt,
                   cyhal_gpio_t ledCtrl,
                   cyhal_gpio_t cardEmmcReset);

/** Release the SDHC peripheral, not currently invoked. It requires further
 *  resource management.
 *
 * @param[in,out] obj The SDHC object
 */
void cyhal_sdhc_free(cyhal_sdhc_t *obj);

/** Configure the SDHC block.
 *
 * @param[in,out] obj    The SDHC object
 * @param[in]     config The card configuration object
 * @return The status of the configure request
 */
cy_rslt_t cyhal_sdhc_configure(cyhal_sdhc_t *obj, const cyhal_sdhc_config_t *config);

/** Attempts to read data over the SDHC peripheral.
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to read data from
 * @param[out]    data    Pointer to the byte-array of data to read from the device
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read request
 */
cy_rslt_t cyhal_sdhc_read(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Attempts to write data over SDHC peripheral
 *
 * @param[in]     obj     The SDHC object
 * @param[in]     address The address to write data to
 * @param[in]     data    Pointer to the byte-array of data to write to the device
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the write request
 */
cy_rslt_t cyhal_sdhc_write(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Attempts to erase a block of data over the SDHC peripheral
 *
 * @param[in] obj       The SDHC object
 * @param[in] startAddr Is the address of the first byte to erase
 * @param[in] length    The number of bytes (starting at startAddr) to erase
 * @return The status of the erase request
 */
cy_rslt_t cyhal_sdhc_erase(const cyhal_sdhc_t *obj, uint32_t startAddr, size_t length);

/** Begin the SDHC read
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to read data from
 * @param[out]    data    The receive buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the read_async request
 */
cy_rslt_t cyhal_sdhc_read_async(const cyhal_sdhc_t *obj, uint32_t address, uint8_t *data, size_t *length);

/** Begin the SDHC write
 *
 * @param[in]     obj     The SDHC object that holds the transfer information
 * @param[in]     address The address to write data to
 * @param[in]     data    The transmit buffer
 * @param[in,out] length  Number of bytes to read, updated with the number actually read
 * @return The status of the write_async request
 */
cy_rslt_t cyhal_sdhc_write_async(const cyhal_sdhc_t *obj, uint32_t address, const uint8_t *data, size_t *length);

/** Checks if the specified SDHC peripheral is in use
 *
 * @param[in]  obj  The SDHC peripheral to check
 * @return Indication of whether the SDHC is still transmitting
 */
bool cyhal_sdhc_is_busy(const cyhal_sdhc_t *obj);

/** Abort an SDHC transfer
 *
 * @param[in] obj The SDHC peripheral to stop
 * @return The status of the abort_async request
 */
cy_rslt_t cyhal_sdhc_abort_async(const cyhal_sdhc_t *obj);

/** The SDHC interrupt handler registration
 *
 * @param[in] obj         The SDHC object
 * @param[in] handler     The callback handler which will be invoked when the interrupt fires
 * @param[in] handler_arg Generic argument that will be provided to the handler when called
 */
void cyhal_sdhc_register_irq(cyhal_sdhc_t *obj, cyhal_sdhc_irq_handler_t handler, void *handler_arg);

/** Configure SDHC interrupt enablement.
 *
 * @param[in] obj      The SDHC object
 * @param[in] event    The SDHC IRQ type
 * @param[in] enable   True to turn on interrupts, False to turn off
 */
void cyhal_sdhc_irq_enable(cyhal_sdhc_t *obj, cyhal_sdhc_irq_event_t event, bool enable);

/** \} group_hal_sdhc_functions */

#if defined(__cplusplus)
}
#endif

/** \} group_hal_sdhc */
