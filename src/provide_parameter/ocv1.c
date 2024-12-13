/*******************************************************************************
 *
 * ██╗  ██╗██╗██╗     ███╗   ██╗
 * ██║ ██╔╝██║██║     ████╗  ██║
 * █████╔╝ ██║██║     ██╔██╗ ██║
 * ██╔═██╗ ██║██║     ██║╚██╗██║
 * ██║  ██╗██║███████╗██║ ╚████║
 * ╚═╝  ╚═╝╚═╝╚══════╝╚═╝  ╚═══╝
 *
 * Kiln Ethereum Ledger App
 * (c) 2022-2024 Kiln
 *
 * contact@kiln.fi
 ********************************************************************************/

#include "provide_parameter.h"

void handle_v1_withdraw_funcs(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function f(
    //    bytes
    // )
    //
    // **************************************************************************
    // example:
    //
    // function withdrawEL(
    //    bytes validatorPubkey
    // ) external
    //
    // [  0] selector
    // [  4] validatorPubkeyoffset
    // [  36] validatorPubkeylength
    // [  68] validatorPubkey_chunk_0
    // [  100] validatorPubkey_chunk_1
    //

    v1_withdraw_funcs_t *params = &context->param_data.v1_withdraw_funcs;

    switch (context->next_param) {
        case V1_WFUNCS_BYTES_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);
            if (offset != PARAMETER_LENGTH) {
                PRINTF("Malformed calldata, unexpected parameter offset %d != %d\n",
                       offset,
                       PARAM_OFFSET);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = V1_WFUNCS_BYTES_LENGTH;
            break;
        }
        case V1_WFUNCS_BYTES_LENGTH:
            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            if (params->current_item_count == 0) {
                context->next_param = V1_WFUNCS_UNEXPECTED_PARAMETER;
            } else {
                context->next_param = V1_WFUNCS_BYTES__ITEMS;
            }
            break;
        case V1_WFUNCS_BYTES__ITEMS:
            if (params->current_item_count >= PARAMETER_LENGTH) {
                params->current_item_count -= PARAMETER_LENGTH;
            } else {
                context->next_param = V1_WFUNCS_UNEXPECTED_PARAMETER;
            }
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}