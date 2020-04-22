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
 * Header file containing the base subsystem API.
 *
 * @addtogroup BASE_SS (SS) Base Subsystem
 *
 * Module for Base subsystem access.
 *
 * @{
 */
/*==========================================================================*/

#ifndef SC_SS_BASE_H
#define SC_SS_BASE_H

/* Includes */

#include "main/types.h"
#include "ss/inf/inf.h"
#include "svc/pm/svc.h"
#include "drivers/xrdc2/fsl_xrdc2.h"
#include "main/sched.h"

/* Defines */

#define PD_DSC      0U
#define PD_SS       1U

#define CLKOFFTRANS()   ((from_mode <= SC_PM_PW_MODE_STBY) && (to_mode <= SC_PM_PW_MODE_STBY))
#define CLK2OFF()       ((from_mode > SC_PM_PW_MODE_STBY) && (to_mode <= SC_PM_PW_MODE_STBY))
#define CLK2ON()        ((from_mode <= SC_PM_PW_MODE_STBY) && (to_mode > SC_PM_PW_MODE_STBY))
#define PWR2OFF()       (to_mode == SC_PM_PW_MODE_OFF)
#define HSCLK()         ((from_mode <= SC_PM_PW_MODE_LP) && (to_mode > SC_PM_PW_MODE_LP))
#define PWR2ON()        (from_mode == SC_PM_PW_MODE_OFF)
#define RUN2CLKOFF()    ((from_mode > SC_PM_PW_MODE_STBY) && (to_mode == SC_PM_PW_MODE_STBY))

/* Types */

/*!
 * This type is used store static constant info on the signals for
 * a Q/P-channel interface.
 */
typedef struct
{
    uint8_t qreqn;
    uint8_t qacceptn;
    uint8_t qdeny;
    uint8_t qactive;
} ss_qchannel_t;

/* Functions */

SS_FUNC_PROTO_BASE

void ss_init_boot_base(sc_sub_t ss, ss_idx_t ss_idx, sc_pm_power_mode_t to_mode);
sc_bool_t ss_is_powered(sc_sub_t ss);
void ss_set_rsrc_clks(sc_sub_t ss, ss_idx_t ss_idx,
        sc_pm_power_mode_t from_mode, sc_pm_power_mode_t to_mode);
void ss_trans_pd(sc_sub_t ss, dsc_pdom_t pd,
    sc_pm_power_mode_t from_mode, sc_pm_power_mode_t to_mode);
sc_bool_t ss_qchannel_power(sc_dsc_t dsc, const ss_qchannel_t channel, sc_bool_t up);
void ss_ssi_link(sc_sub_t local, sc_sub_t remote, uint8_t local_port,
    uint8_t remote_port, sc_bool_t enable, sc_bool_t noblock);
void ss_ssi_po_link(sc_sub_t local, sc_sub_t remote, uint8_t port,
    sc_bool_t enable);
void ss_adb_link(sc_sub_t local, sc_sub_t remote, sc_bool_t enable);
void ss_adb_enb(sc_sub_t ss, uint32_t adb, sc_bool_t enable);
void ss_adb_x2_enb(sc_sub_t ss, uint32_t adb1, uint32_t adb2, sc_bool_t enable);
void ss_adb_wt(sc_sub_t ss, uint32_t adb, sc_bool_t enable);
void ss_adb_x2_wt(sc_sub_t ss, uint32_t adb1, uint32_t adb2, sc_bool_t enable);
sc_err_t ss_config_pll_rate(sc_sub_t ss, ss_clock_t clk_index,
    sc_pm_clock_rate_t *rate, sc_bool_t rom_boot);
sc_err_t ss_config_clk_rate(sc_sub_t ss, ss_clock_t clk_index, 
    sc_pm_clock_rate_t *rate, sc_bool_t rom_boot);
sc_err_t ss_config_pll_mode(sc_sub_t ss, ss_clock_t clk_index,
    sc_pm_clk_mode_t mode, sc_bool_t rom_boot);
sc_err_t ss_config_clk_mode(sc_sub_t ss, ss_clock_t clk_index,
    sc_pm_clk_mode_t mode, sc_bool_t rom_boot);
sc_err_t ss_ap_reset_vector(sc_faddr_t addr, uint32_t cpu, sc_bool_t *vector);
void ss_populate_boot_dev_info(ss_idx_t ss_idx, sc_sub_t ss, 
    sc_bool_t fix_pll_rate);
sc_err_t ss_get_clk_rate_by_index(sc_sub_t ss, ss_clock_t clk_index,
    sc_pm_clock_rate_t *rate);
sc_pm_clk_parent_t ss_get_parent_index(sc_sub_t ss, ss_clock_t clk_index);
void ss_set_parent_by_index(sc_sub_t ss, ss_clock_t clk_index,
    sc_pm_clk_parent_t new_parent_index);
sc_err_t ss_refactor_clks(sc_sub_t ss, ss_idx_t ss_idx, sc_pm_clk_t clk,
    const sc_pm_clock_rate_t *cur_rate, sc_pm_clock_rate_t *rate);
void ss_active_fbb_domains(sc_sub_t ss, uint8_t *active_mask, uint8_t *idle_mask);

#ifdef XRDC_SUPPORT
    void ss_rdc_power_down(sc_sub_t ss, dsc_pdom_t pd);
    void ss_rdc_set_custom_master(sc_sub_t ss, ss_idx_t ss_idx, ss_ridx_t
        rsrc_idx, sc_bool_t valid, sc_bool_t lock, sc_rm_spa_t sa, sc_rm_spa_t pa,
        sc_rm_did_t did, sc_rm_sid_t sid, xrdc_match_t match,
        xrdc_match_t mask, uint8_t cid);
    void ss_rdc_set_custom_mda(sc_sub_t ss, sc_rm_idx_t xrdc_master_idx,
        sc_bool_t valid, sc_bool_t lock, sc_rm_spa_t sa, sc_rm_spa_t pa, sc_rm_did_t did,
        sc_rm_sid_t sid, xrdc_match_t match, xrdc_match_t mask, uint8_t cid);
#endif /* XRDC_SUPPORT */
void ss_dsc_irq_handler_base(sc_dsc_t dsc, 
    const ss_dsc_l2irq_handler *ss_dsc_l2irq_handlers,
    const ss_dsc_l2irq_handler *ss_l2irq_handlers);
sc_bool_t ss_dsc_handler_default(sc_dsc_t dsc, uint32_t irqIndex);

/* Debug Functions */

#ifdef DEBUG
    void ss_dump_header_base(sc_sub_t ss);
    void ss_dump_master_counts_base(sc_sub_t ss);
    void ss_dump_xrdc_base(sc_sub_t ss, sc_dsc_t dsc,
        const ss_base_info_t *base);
    void ss_dump_clocks(sc_sub_t ss);
#endif

/* External variables */

extern const ss_dsc_l2irq_handler ss_dsc_l2irq_handlers_base[];
extern const ss_dsc_l2irq_handler ss_l2irq_handlers_base[];
extern sc_bool_t base_reboot;
extern sc_bool_t base_cci_noblock;

#endif /* SC_SS_BASE_H */

/**@}*/

