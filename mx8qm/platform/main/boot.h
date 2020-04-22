/*
** ###################################################################
**
**     Copyright (c) 2016 Freescale Semiconductor, Inc.
**     Copyright 2017-2018 NXP
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**
** ###################################################################
*/

/*==========================================================================*/
/*!
 * @file
 *
 * Header file for interaction with the ROM.
 */
/*==========================================================================*/

#ifndef SC_BOOT_H
#define SC_BOOT_H

/* Includes */

#include "main/types.h"

/* Defines */

#define SC_BOOT_DATA_BARKER         0xC0FFEE16U
#define SC_BOOT_DATA_VER            0x01U
#ifndef USE_LEGACY_ROM
#define SC_BOOT_MAX_LIST            8U
#else
#define SC_BOOT_MAX_LIST            6U
#endif

#define SC_IMG_TYPE_EXEC            0x03U   /* Executable image type */
#define SC_IMG_TYPE_DATA            0x04U   /* Data image type */
#define SC_IMG_TYPE_DEBUG           0x08U   /* Signed debug enable message */
#define SC_IMG_TYPE_SECFUSE         0x09U   /* Signed secure fuse message */
#define SC_IMG_TYPE_RETURN          0x0AU   /* Signed field return message */
#define SC_IMG_TYPE_PATCH           0x0BU   /* Signed patch message */
#define SC_IMG_TYPE_ALL             0x0CU   /* All images */

#define SC_BD_FLAGS_NOT_SECURE      16U
#define SC_BD_FLAGS_NOT_ISOLATED    17U
#define SC_BD_FLAGS_RESTRICTED      18U
#define SC_BD_FLAGS_GRANT           19U
#define SC_BD_FLAGS_NOT_COHERENT    20U
#define SC_BD_FLAGS_ALT_CONFIG      21U
#define SC_BD_FLAGS_EARLY_CPU_START 22U
#define SC_BD_FLAGS_DDRTEST         23U
#define SC_BD_FLAGS_NO_AP           24U

/* Types */

/*!
 * This type is used to indicate boot type.
 */
typedef enum
{
    #ifdef USE_LEGACY_ROM
        SC_BT_TYPE_PRIMARY      = 0,
        SC_BT_TYPE_RECOVERY     = 1,
        SC_BT_TYPE_MANUFACTURE  = 2,
    #else
        SC_BT_TYPE_PRIMARY      = 1,
        SC_BT_TYPE_SECONDARY    = 2,
        SC_BT_TYPE_RECOVERY     = 3,
    #endif
    SC_BT_TYPE_SERIAL           = 0xFF
} sc_bt_type_t;

/*!
 * This type is used to indicate boot device.
 */
typedef enum
{
    SC_BT_DEV_TYPE_SD           = 1,
    SC_BT_DEV_TYPE_MMC          = 2,
    SC_BT_DEV_TYPE_NAND         = 3,
    SC_BT_DEV_TYPE_FLEXSPINOR   = 4,
    SC_BT_DEV_TYPE_SATA_DISK    = 7,
    SC_BT_DEV_TYPE_USB          = 14
} sc_bt_dev_type_t;

/*!
 * This type is used to store a CPU start request.
 */
typedef struct
{
    sc_faddr_t addr;                        //!< 64-bit address to boot from
    uint16_t cpu;                           //!< CPU to start
    uint16_t mu;                            //!< MU associated with this CPU
    uint8_t part;                           //!< Partition to start
    uint8_t resv[3];                        //!< Reserved
} sc_boot_list_t;

/*!
 * This type is used pass data from the ROM to the SCFW.
 */
typedef struct
{
    uint32_t barker;                        //!< Barker code
    uint16_t ver;                           //!< Version
    uint16_t size;                          //!< Size
    uint8_t num;                            //!< Number images in list
    uint8_t resv0[3];                       //!< Reserved
    uint32_t bd_flags;                      //!< Boot data flags
    sc_boot_list_t img[SC_BOOT_MAX_LIST];   //!< image list
    uint32_t resv1[4];                      //!< was SCD address
} sc_boot_data_t;

#ifdef SC_ROM_FUNC_ADDR
    /*!
     * This type is used make calls to the ROM.
     */
    typedef struct
    {
        uint16_t ver;
        uint16_t tag;
        uint32_t (*dsc_ai_read)(uint8_t dsc_id, uint8_t src_sel,
            uint16_t address);
        int32_t (*dsc_ai_write)(uint8_t dsc_id, uint8_t src_sel,
            uint16_t address, uint32_t data);
    } sc_rom_functions_t;
#endif

/* Functions */

sc_err_t boot_init(sc_bool_t allow);

/*!
 * This function returns flags passed to the SCFW from the ROM. These
 * come from the boot container (bd_flags).
 *
 * @param[out]    flag         pointer to return raw flag value
 * @param[out]    not_secure   pointer to return secure flag
 * @param[out]    not_isolated pointer to return isolated flag
 * @param[out]    restricted   pointer to return restricted flag
 * @param[out]    grant        pointer to return grant flag
 * @param[out]    not_coherent pointer to return coherent flag
 * @param[out]    alt_config   pointer to return alt_config flag
 * @param[out]    early        pointer to return early flag
 * @param[out]    ddrtest      pointer to return ddrtest flag
 * @param[out]    no_ap        pointer to return no_ap flag
 *
 * For all pointers, NULL will result in no return of that value.
 *
 * See the [Boot Flags](@ref BOOT_FLAGS) section for defintition
 * of these flags.
 *
 * Many of the flags (some inverted) are used to create the
 * boot partition by calling sc_rm_partition_alloc().
 * 
 * @return Returns an error code (SC_ERR_NONE = success).
 */
sc_err_t boot_get_data(uint32_t *flag, sc_bool_t *not_secure, sc_bool_t *not_isolated,
    sc_bool_t *restricted, sc_bool_t *grant, sc_bool_t *not_coherent, sc_bool_t *alt_config,
    sc_bool_t *early, sc_bool_t *ddrtest, sc_bool_t *no_ap);

sc_err_t boot_get_img_1(uint8_t type, sc_rsrc_t *cpu, sc_faddr_t *addr,
    sc_rsrc_t *mu, sc_rm_pt_t *pt);
sc_err_t boot_get_img_n(uint8_t type, sc_rsrc_t *cpu, sc_faddr_t *addr,
    sc_rsrc_t *mu, sc_rm_pt_t *pt);
sc_err_t boot_get_bring_up(sc_sub_t ss, sc_bool_t *flag);
sc_err_t boot_get_boot_dev(sc_bt_dev_type_t *dev_type, uint8_t *inst,
    sc_bt_type_t *bt_type);
sc_err_t boot_get_boot_sw_info(uint32_t *core_freq, uint32_t *axi_freq,
    uint32_t *ddr_freq, uint32_t *rom_tick_freq);
sc_err_t boot_pass_over(sc_rsrc_t resource_cpu, sc_faddr_t boot_addr);
uint8_t boot_get_img_num(void);
sc_err_t boot_get_img_start_addr(uint16_t img, sc_faddr_t *addr);

/*!
 * This function returns the boot device resource associate with
 * the specified partition.
 *
 * @param[in]     pt          partition
 * @param[in]     check_pt    Boolean to verify if boot device needs
 *                              to be associated with the partition.
 *
 * @return Returns the boot device.
 */
sc_rsrc_t boot_get_dev(sc_rm_pt_t pt, sc_bool_t check_pt);

void boot_check_rom_data(void);

#endif /* SC_BOOT_H */

