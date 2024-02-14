#include <stdbool.h>
#include "kiln_plugin.h"

static bool withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Rewards", msg->titleLength);

    switch (context->selectorIndex) {
        case KILN_V1_WITHDRAW:
            strlcpy(msg->msg, "Consensus & Exec", msg->msgLength);
            break;

        case KILN_V1_WITHDRAW_EL:
            strlcpy(msg->msg, "Execution Layer", msg->msgLength);
            break;

        case KILN_V1_WITHDRAW_CL:
            strlcpy(msg->msg, "Consensus Layer", msg->msgLength);
            break;

        default:
            strlcpy(msg->msg, "?", msg->msgLength);
            break;
    }
    return true;
}

static bool withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            ret = withdraw_rewards_ui(msg, context);
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool batch_withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Rewards", msg->titleLength);

    switch (context->selectorIndex) {
        case KILN_V1_BATCH_WITHDRAW:
            strlcpy(msg->msg, "Consensus & Exec", msg->msgLength);
            break;

        case KILN_V1_BATCH_WITHDRAW_EL:
            strlcpy(msg->msg, "Execution Layer", msg->msgLength);
            break;

        case KILN_V1_BATCH_WITHDRAW_CL:
            strlcpy(msg->msg, "Consensus Layer", msg->msgLength);
            break;

        default:
            strlcpy(msg->msg, "?", msg->msgLength);
            break;
    }
    return true;
}

static bool batch_withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            ret = batch_withdraw_rewards_ui(msg, context);
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool request_exits_ui(ethQueryContractUI_t *msg) {
    strlcpy(msg->title, "Request", msg->titleLength);
    strlcpy(msg->msg, "Validators Exit", msg->msgLength);
    return true;
}

static bool request_exit_ui(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            ret = request_exits_ui(msg);
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool stake_ui(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Stake", msg->titleLength);
            const uint8_t *eth_amount = msg->pluginSharedRO->txContent->value.value;
            uint8_t eth_amount_size = msg->pluginSharedRO->txContent->value.length;

            ret = amountToString(eth_amount,
                                 eth_amount_size,
                                 WEI_TO_ETHER,
                                 "ETH",
                                 msg->msg,
                                 msg->msgLength);
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool request_exit_ui_v2(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Request", msg->titleLength);
            strlcpy(msg->msg, "Position Exit", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool multiclaim_ui_v2(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            strlcpy(msg->msg, "Exited Positions", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool claim_ui_v2(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Withdraw", msg->titleLength);
            strlcpy(msg->msg, "Exited Position", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool deposit_into_stragey_ui_lr(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;
    lr_deposit_t *params = &context->param_data.lr_deposit;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "EigenLayer", msg->titleLength);
            strlcpy(msg->msg, "Deposit In Strategy", msg->msgLength);
            ret = true;
            break;
        case 1:
            strlcpy(msg->title, "Strategy", msg->titleLength);
            if (params->strategy_to_display == -1) {
                strlcpy(msg->msg, "UNKNOWN", msg->msgLength);
            } else {
                strlcpy(msg->msg, lr_tickers[params->strategy_to_display], MAX_TICKER_LEN);
            }
            ret = true;
            break;
        case 2:
            strlcpy(msg->title, "Amount", msg->titleLength);
            amountToString(
                params->erc20_amount_to_display,
                sizeof(params->erc20_amount_to_display),
                ERC20_DECIMALS,
                params->erc20_to_display == -1 ? "UNKNOWN" : lr_tickers[params->erc20_to_display],
                msg->msg,
                msg->msgLength);
            ret = true;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool queue_withdrawal_ui_lr(ethQueryContractUI_t *msg, context_t *context) {
    // TODO: display strategies
    bool ret = false;
    lr_queue_withdrawal_t *params = &context->param_data.lr_queue_withdrawal;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "EigenLayer", msg->titleLength);
            strlcpy(msg->msg, "Queue Withdrawal", msg->msgLength);
            ret = true;
            break;

        case 1:
            strlcpy(msg->title, "Withdrawer", msg->titleLength);
            strlcpy(msg->msg, params->withdrawer, msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

static bool complete_queued_withdrawal_ui_lr(ethQueryContractUI_t *msg) {
    // TODO: display strategies
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "EigenLayer", msg->titleLength);
            strlcpy(msg->msg, "Complete Withdrawal", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

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
            ret = stake_ui(msg);
            break;

        case KILN_V2_REQUEST_EXIT:
            ret = request_exit_ui_v2(msg);
            break;

        case KILN_V2_MULTICLAIM:
            ret = multiclaim_ui_v2(msg);
            break;

        case KILN_V2_CLAIM:
            ret = claim_ui_v2(msg);
            break;

        case KILN_LR_DEPOSIT_INTO_STRATEGY:
            ret = deposit_into_stragey_ui_lr(msg, context);
            break;

        case KILN_LR_QUEUE_WITHDRAWAL:
            ret = queue_withdrawal_ui_lr(msg, context);
            break;

        case KILN_LR_COMPLETE_QUEUED_WITHDRAWAL:
            ret = complete_queued_withdrawal_ui_lr(msg);
            break;

        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            break;
    }
    msg->result = ret ? ETH_PLUGIN_RESULT_OK : ETH_PLUGIN_RESULT_ERROR;
}
