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

#include "query_contract_ui.h"

void handle_query_contract_ui(ethQueryContractUI_t *msg) {
    context_t *context = (context_t *) msg->pluginContext;
    bool ret = false;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    switch (context->selectorIndex) {
        case KILN_V1_DEPOSIT:
            ret = stake_ui(msg);
            break;

        case KILN_V1_WITHDRAW:
        case KILN_V1_WITHDRAW_EL:
        case KILN_V1_WITHDRAW_CL:
            ret = withdraw_ui(msg, context);
            break;

        case KILN_V1_BATCH_WITHDRAW:
        case KILN_V1_BATCH_WITHDRAW_EL:
        case KILN_V1_BATCH_WITHDRAW_CL:
            ret = batch_withdraw_ui(msg, context);
            break;

        case KILN_V1_REQUEST_EXIT:
            ret = request_exit_ui(msg);
            break;

        case KILN_V2_STAKE:
            ret = stake_in_pool_ui(msg);
            break;

        case KILN_V2_REQUEST_EXIT:
            ret = request_pooling_exit_ui(msg);
            break;

        case KILN_V2_MULTICLAIM:
            ret = multiclaim_ui(msg);
            break;

        case KILN_V2_CLAIM:
            ret = claim_ui(msg);
            break;

        case KILN_LR_DEPOSIT_INTO_STRATEGY:
            ret = deposit_into_strategy_ui(msg, context);
            break;

        case KILN_LR_QUEUE_WITHDRAWALS:
            ret = queue_withdrawals_ui(msg, context);
            break;

        case KILN_LR_COMPLETE_QUEUED_WITHDRAWALS:
            ret = complete_queued_withdrawals_ui(msg, context);
            break;

        case KILN_LR_DELEGATE_TO:
            ret = delegate_to_ui(msg, context);
            break;

        case KILN_LR_UNDELEGATE:
            ret = undelegate_ui(msg);
            break;

        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            break;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
