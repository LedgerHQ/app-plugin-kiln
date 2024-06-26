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

bool stake_in_pool_ui(ethQueryContractUI_t *msg) {
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

bool request_pooling_exit_ui(ethQueryContractUI_t *msg) {
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

bool multiclaim_ui(ethQueryContractUI_t *msg) {
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

bool claim_ui(ethQueryContractUI_t *msg) {
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