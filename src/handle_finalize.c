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

#include "kiln_plugin.h"

void handle_finalize(ethPluginFinalize_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    switch (context->selectorIndex) {
        case KILN_V1_DEPOSIT:
        case KILN_V1_WITHDRAW:
        case KILN_V1_WITHDRAW_EL:
        case KILN_V1_WITHDRAW_CL:
        case KILN_V1_BATCH_WITHDRAW:
        case KILN_V1_BATCH_WITHDRAW_EL:
        case KILN_V1_BATCH_WITHDRAW_CL:
        case KILN_V1_REQUEST_EXIT:
            msg->numScreens = 1;
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        case KILN_V2_STAKE:
        case KILN_V2_REQUEST_EXIT:
        case KILN_V2_MULTICLAIM:
        case KILN_V2_CLAIM:
            msg->numScreens = 1;
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;

        case KILN_LR_DEPOSIT_INTO_STRATEGY:
            msg->numScreens = 3;
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        case KILN_LR_QUEUE_WITHDRAWALS: {
            {
                lr_queue_withdrawals_t *params = &context->param_data.lr_queue_withdrawals;
                // function screen + withdrawer
                msg->numScreens = 2;
                // one screen per withdrawal
                msg->numScreens += params->strategies_count;
                PRINTF("NUMBER OF STRATEGIES TO DISPLAY: %d\n", params->strategies_count);
            }
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        }
        case KILN_LR_COMPLETE_QUEUED_WITHDRAWALS: {
            {
                lr_complete_queued_withdrawals_t *params =
                    &context->param_data.lr_complete_queued_withdrawals;
                // function screen + withdrawer
                msg->numScreens = 2;
                // one screen per strategy
                msg->numScreens += params->strategies_count;
                PRINTF("NUMBER OF STRATEGIES TO DISPLAY: %d\n", params->strategies_count);
            }
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        }
        case KILN_LR_DELEGATE_TO:
            msg->numScreens = 2;
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        case KILN_LR_UNDELEGATE:
            msg->numScreens = 1;
            msg->result = ETH_PLUGIN_RESULT_OK;
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
