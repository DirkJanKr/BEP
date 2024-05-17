/*--------------------------------------------------------------------------*/
/* Copyright 2023-2024 NXP                                                  */
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

#include <mcuxClToolchain.h>
#include <mcuxClCore_Examples.h> // Defines and assertions for examples
#include <mcuxClExample_Session_Helper.h>
#include <mcuxClEls.h>               // Interface to the entire mcuxClEls component
#include <mcuxClExample_ELS_Helper.h>
#include <mcuxClKey.h>
#include <mcuxClSession.h>
#include <mcuxCsslFlowProtection.h>
#include <mcuxClCore_FunctionIdentifiers.h> // Code flow protection
#include <mcuxClMac.h> // Interface to the entire mcuxClMac component
#include <mcuxClHmac.h> // Interface to the entire mcuxClHmac component
#include <mcuxClHash.h>
#include <mcuxClHashModes.h>
#include <mcuxClExample_RNG_Helper.h>

/** Performs a HMAC computation using functions of the mcuxClKey component.
 * @retval MCUXCLEXAMPLE_STATUS_OK         The example code completed successfully
 * @retval MCUXCLEXAMPLE_STATUS_ERROR      The example code failed */
MCUXCLEXAMPLE_FUNCTION(mcuxClHmac_Sw_Oneshot_example)
{
    /* Example (unpadded) key. */
    const ALIGNED uint8_t hmac_key[] = {
        0x00u, 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u,
        0x88u, 0x99u, 0xaau, 0xbbu, 0xccu, 0xddu, 0xeeu, 0xffu,
        0x00u, 0x11u, 0x22u, 0x33u, 0x44u, 0x55u, 0x66u, 0x77u,
        0x88u, 0x99u, 0xaau, 0xbbu, 0xccu, 0xddu, 0xeeu, 0xffu
    };

    /* Example input to the HMAC function. */
    const ALIGNED uint8_t hmac_input[] = {
        0x00u, 0x9fu, 0x5eu, 0x39u, 0x94u, 0x30u, 0x03u, 0x82u,
        0x50u, 0x72u, 0x1bu, 0xe1u, 0x79u, 0x65u, 0x35u, 0xffu,
        0x21u, 0xa6u, 0x09u, 0xfdu, 0xf9u, 0xf0u, 0xf6u, 0x12u,
        0x66u, 0xe3u, 0xafu, 0x75u, 0xd7u, 0x04u, 0x31u, 0x7du,
        0x55u, 0x06u, 0xf8u, 0x06u, 0x5cu, 0x48u, 0x72u, 0x18u,
        0xe9u, 0x9eu, 0xb4u, 0xc3u, 0xd4u, 0x54u, 0x6cu, 0x4du,
        0x60u, 0x70u, 0x16u, 0x90u, 0x11u, 0x38u, 0x73u, 0x9du,
        0xbdu, 0xf4u, 0x37u, 0xa5u, 0xe6u, 0xf5u, 0x02u, 0x1au
    };

    /* Example reference HMAC. */
    const ALIGNED uint8_t hmac_output_reference[MCUXCLHASH_OUTPUT_SIZE_SHA_256] = {
        0x06u, 0xb8u, 0xb8u, 0xc3u, 0x21u, 0x79u, 0x15u, 0xbeu,
        0x0bu, 0x0fu, 0x86u, 0x90u, 0x4fu, 0x76u, 0x74u, 0x1bu,
        0x1bu, 0xe2u, 0x86u, 0x79u, 0x38u, 0xf4u, 0xf0u, 0x5du,
        0x34u, 0x15u, 0xbbu, 0x36u, 0x8fu, 0x4au, 0x57u, 0xfbu
    };

    /**************************************************************************/
    /* Preparation                                                            */
    /**************************************************************************/

    /* Initialize ELS (needed for Hash computation) */
    if(!mcuxClExample_Els_Init(MCUXCLELS_RESET_DO_NOT_CANCEL))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Output buffer for the computed MAC. */
    static uint8_t result_buffer[MCUXCLHMAC_MAX_OUTPUT_SIZE];

    /* Allocate and initialize session / workarea */
    mcuxClSession_Descriptor_t sessionDesc;
    mcuxClSession_Handle_t session = &sessionDesc;

    /* Allocate and initialize session / workarea */
    MCUXCLEXAMPLE_ALLOCATE_AND_INITIALIZE_SESSION(session, MCUXCLEXAMPLE_MAX_WA(MCUXCLHMAC_MAX_CPU_WA_BUFFER_SIZE, MCUXCLRANDOMMODES_NCINIT_WACPU_SIZE), 0u);

    /* Initialize the PRNG */
    MCUXCLEXAMPLE_INITIALIZE_PRNG(session);

    /**************************************************************************/
    /* Key setup                                                              */
    /**************************************************************************/

    /* Create and initialize mcuxClKey_Descriptor_t structure. */
    uint32_t keyDesc[MCUXCLKEY_DESCRIPTOR_SIZE_IN_WORDS];
    mcuxClKey_Handle_t key = (mcuxClKey_Handle_t) &keyDesc;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyInit_result, keyInit_token, mcuxClKey_init(
        /* mcuxClSession_Handle_t pSession:                */  session,
        /* mcuxClKey_Handle_t key:                         */  key,
        /* const mcuxClKey_Type* type:                     */  mcuxClKey_Type_Hmac_variableLength,
MCUX_CSSL_ANALYSIS_START_SUPPRESS_DISCARD_CONST("Required by API function")
        /* uint8_t * pKeyData:                            */  (uint8_t *) hmac_key,
MCUX_CSSL_ANALYSIS_STOP_SUPPRESS_DISCARD_CONST()
        /* uint32_t keyDataLength:                        */  sizeof(hmac_key)));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_init) != keyInit_token) || (MCUXCLKEY_STATUS_OK != keyInit_result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Generate an HMAC mode containing the hash algorithm                    */
    /**************************************************************************/

    ALIGNED uint8_t hmacModeDescBuffer[MCUXCLHMAC_HMAC_MODE_DESCRIPTOR_SIZE];
    mcuxClMac_CustomMode_t mode = (mcuxClMac_CustomMode_t) hmacModeDescBuffer;

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(hashCreateMode_result, hashCreateMode_token, mcuxClHmac_createHmacMode(
    /* mcuxClMac_CustomMode_t mode:       */ mode,
    /* mcuxClHash_Algo_t hashAlgorithm:   */ mcuxClHash_Algorithm_Sha256)
    );

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClHmac_createHmacMode) != hashCreateMode_token) || (MCUXCLMAC_STATUS_OK != hashCreateMode_result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* HMAC computation                                                       */
    /**************************************************************************/

    /* Call the mcuxClMac_compute function to compute a HMAC in one shot. */
    uint32_t result_size = 0u;
    MCUXCLBUFFER_INIT_RO(hmac_input_buf, session, hmac_input, sizeof(hmac_input));
    MCUXCLBUFFER_INIT_RW(hmac_output_buf, session, result_buffer, sizeof(result_buffer));
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(macCompute_result, macCompute_token, mcuxClMac_compute(
        /* mcuxClSession_Handle_t session:  */ session,
        /* const mcuxClKey_Handle_t key:    */ key,
        /* const mcuxClMac_Mode_t mode:     */ mode,
        /* mcuxCl_InputBuffer_t pIn:        */ hmac_input_buf, /* No extra space for padding is required */
        /* uint32_t inLength:              */ sizeof(hmac_input),
        /* mcuxCl_Buffer_t pMac:            */ hmac_output_buf,
        /* uint32_t * const pMacLength:    */ &result_size
    ));

    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClMac_compute) != macCompute_token) || (MCUXCLMAC_STATUS_OK != macCompute_result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /**************************************************************************/
    /* Verification                                                           */
    /**************************************************************************/

    /* Compare the output size with the expected MAC size */
    if(sizeof(hmac_output_reference) != result_size)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /* Compare the result to the reference value. */
    if(!mcuxClCore_assertEqual(hmac_output_reference, result_buffer, sizeof(hmac_output_reference)))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    /**************************************************************************/
    /* Cleanup                                                                */
    /**************************************************************************/

    /* Flush the key. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(keyFlush_result, keyFlush_token, mcuxClKey_flush(session,
                                                                  key));
    if((MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClKey_flush) != keyFlush_token) || (MCUXCLKEY_STATUS_OK != keyFlush_result))
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();


    /* Clean-up and destroy the session. */
    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionCleanup_result, sessionCleanup_token, mcuxClSession_cleanup(
         /* mcuxClSession_Handle_t           pSession: */           session));
    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_cleanup) != sessionCleanup_token || MCUXCLSESSION_STATUS_OK != sessionCleanup_result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    MCUX_CSSL_FP_FUNCTION_CALL_BEGIN(sessionDestroy_result, sessionDestroy_token, mcuxClSession_destroy(
         /* mcuxClSession_Handle_t           pSession: */           session));
    if(MCUX_CSSL_FP_FUNCTION_CALLED(mcuxClSession_destroy) != sessionDestroy_token || MCUXCLSESSION_STATUS_OK != sessionDestroy_result)
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }
    MCUX_CSSL_FP_FUNCTION_CALL_END();

    /** Disable the ELS **/
    if(!mcuxClExample_Els_Disable())
    {
        return MCUXCLEXAMPLE_STATUS_ERROR;
    }

    return MCUXCLEXAMPLE_STATUS_OK;
}
