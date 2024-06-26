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

void handle_provide_parameter(ethPluginProvideParameter_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    switch (context->selectorIndex) {
        case KILN_V1_DEPOSIT:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;

        case KILN_V1_WITHDRAW:
        case KILN_V1_WITHDRAW_EL:
        case KILN_V1_WITHDRAW_CL:
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        case KILN_V1_BATCH_WITHDRAW:
        case KILN_V1_BATCH_WITHDRAW_EL:
        case KILN_V1_BATCH_WITHDRAW_CL:
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        case KILN_V1_REQUEST_EXIT:
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        case KILN_V2_STAKE:
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
        case KILN_V2_REQUEST_EXIT:
        case KILN_V2_MULTICLAIM:
        case KILN_V2_CLAIM:
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        case KILN_LR_DEPOSIT_INTO_STRATEGY:
            handle_lr_deposit_into_strategy(msg, context);
            break;
        case KILN_LR_QUEUE_WITHDRAWALS:
            handle_lr_queue_withdrawals(msg, context);
            break;
        case KILN_LR_COMPLETE_QUEUED_WITHDRAWALS:
            handle_lr_complete_queued_withdrawals(msg, context);
            break;
        case KILN_LR_DELEGATE_TO:
            handle_lr_delegate_to(msg, context);
            break;
        case KILN_LR_UNDELEGATE:
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
