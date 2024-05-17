/*--------------------------------------------------------------------------*/
/* Copyright 2023 NXP                                                       */
/*                                                                          */
/* NXP Confidential. This software is owned or controlled by NXP and may    */
/* only be used strictly in accordance with the applicable license terms.   */
/* By expressly accepting such terms or by downloading, installing,         */
/* activating and/or otherwise using the software, you are agreeing that    */
/* you have read, and that you agree to comply with and are bound by, such  */
/* license terms. If you do not agree to be bound by the applicable license */
/* terms, then you may not retain, install, activate or otherwise use the   */
/* software.                                                                */
/*--------------------------------------------------------------------------*/

/** @file  mcuxClHmac_Core_Functions_Sw.h
 *  @brief Internal definitions of SW engine functions for the HMAC component
 */

#ifndef MCUXCLHMAC_CORE_FUNCTIONS_SW_H_
#define MCUXCLHMAC_CORE_FUNCTIONS_SW_H_

#include <mcuxClConfig.h> // Exported features flags header
#include <internal/mcuxClHmac_Internal_Types.h>


#ifdef __cplusplus
extern "C" {
#endif


/****************************/
/* Engine functions         */
/****************************/

/**
 * @brief HMAC engine function for the oneshot computation using SW
 *
 * This function performs the actual HMAC operation using the SW.
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClHmac_Engine_Oneshot_Sw, mcuxClHmac_ComputeEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClHmac_Engine_Oneshot_Sw(
    mcuxClSession_Handle_t session,                /* HMAC session handle */
    mcuxClHmac_Context_Generic_t * const pContext, /* HMAC context */
    mcuxCl_InputBuffer_t pIn,                    /* HMAC input */
    uint32_t inLength,                            /* Input size */
    mcuxCl_Buffer_t pOut,                         /* HMAC output */
    uint32_t * const pOutLength                   /* Output size */
);

/**
 * @brief HMAC engine function for the init phase of a multi-part computation using SW
 *
 * This function prepares a context to start a multi-part HMAC computation.
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClHmac_Engine_Init_Sw, mcuxClHmac_InitEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClHmac_Engine_Init_Sw(
    mcuxClSession_Handle_t session,                /*  HMAC session handle */
    mcuxClHmac_Context_Generic_t * const pContext  /*  HMAC context */
);

/**
 * @brief HMAC engine function for the update phase of a multi-part computation using SW
 *
 * This function processes additional data for a multi-part HMAC computation.
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClHmac_Engine_Update_Sw, mcuxClHmac_UpdateEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClHmac_Engine_Update_Sw(
    mcuxClSession_Handle_t session,                /* HMAC session handle */
    mcuxClHmac_Context_Generic_t * const pContext, /* HMAC context */
    mcuxCl_InputBuffer_t pIn,                      /* HMAC input */
    uint32_t inLength                             /* Input size */
);

/**
 * @brief HMAC engine function for the finalize phase of a multi-part computation using SW
 *
 * This function finalizes a multi-part HMAC computation.
 *
 */
MCUX_CSSL_FP_FUNCTION_DECL(mcuxClHmac_Engine_Finalize_Sw, mcuxClHmac_FinalizeEngine_t)
MCUX_CSSL_FP_PROTECTED_TYPE(mcuxClMac_Status_t) mcuxClHmac_Engine_Finalize_Sw(
    mcuxClSession_Handle_t session,                /* HMAC session handle */
    mcuxClHmac_Context_Generic_t * const pContext, /* HMAC context */
    mcuxCl_Buffer_t pOut,                          /* HMAC output */
    uint32_t *const pOutLength                    /* Output size */
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MCUXCLHMAC_CORE_FUNCTIONS_SW_H_ */
