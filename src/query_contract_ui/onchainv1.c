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

bool stake_ui(ethQueryContractUI_t *msg) {
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

bool request_exit_ui(ethQueryContractUI_t *msg) {
    bool ret = false;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "Request", msg->titleLength);
            strlcpy(msg->msg, "Validators Exit", msg->msgLength);
            ret = true;
            break;

        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

bool withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context) {
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

bool withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
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

bool batch_withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context) {
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

bool batch_withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
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
