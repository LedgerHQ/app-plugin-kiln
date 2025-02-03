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

bool defi_deposit_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;
    defi_deposit_t *params = &context->param_data.defi_deposit;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "DeFi", msg->titleLength);
            strlcpy(msg->msg, "Deposit", msg->msgLength);
            ret = true;
            break;
        case 1:
            strlcpy(msg->title, "Assets", msg->titleLength);
            uint256_to_decimal(params->assets_amount,
                               sizeof(params->assets_amount),
                               msg->msg,
                               msg->msgLength);
            ret = true;
            break;
        case 2:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            strlcpy(msg->msg, params->receiver_address, msg->msgLength);
            ret = true;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

bool defi_mint_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;
    defi_mint_t *params = &context->param_data.defi_mint;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "DeFi", msg->titleLength);
            strlcpy(msg->msg, "Mint", msg->msgLength);
            ret = true;
            break;
        case 1:
            strlcpy(msg->title, "Shares", msg->titleLength);
            uint256_to_decimal(params->shares_amount,
                               sizeof(params->shares_amount),
                               msg->msg,
                               msg->msgLength);
            ret = true;
            break;
        case 2:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            strlcpy(msg->msg, params->receiver_address, msg->msgLength);
            ret = true;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

bool defi_withdraw_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;
    defi_withdraw_t *params = &context->param_data.defi_withdraw;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "DeFi", msg->titleLength);
            strlcpy(msg->msg, "Withdraw", msg->msgLength);
            ret = true;
            break;
        case 1:
            strlcpy(msg->title, "Assets", msg->titleLength);
            uint256_to_decimal(params->assets_amount,
                               sizeof(params->assets_amount),
                               msg->msg,
                               msg->msgLength);
            ret = true;
            break;
        case 2:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            strlcpy(msg->msg, params->receiver_address, msg->msgLength);
            ret = true;
            break;
        case 3:
            strlcpy(msg->title, "Owner", msg->titleLength);
            strlcpy(msg->msg, params->owner_address, msg->msgLength);
            ret = true;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}

bool defi_redeem_ui(ethQueryContractUI_t *msg, context_t *context) {
    bool ret = false;
    defi_redeem_t *params = &context->param_data.defi_redeem;

    switch (msg->screenIndex) {
        case 0:
            strlcpy(msg->title, "DeFi", msg->titleLength);
            strlcpy(msg->msg, "Redeem", msg->msgLength);
            ret = true;
            break;
        case 1:
            strlcpy(msg->title, "Shares", msg->titleLength);
            uint256_to_decimal(params->shares_amount,
                               sizeof(params->shares_amount),
                               msg->msg,
                               msg->msgLength);
            ret = true;
            break;
        case 2:
            strlcpy(msg->title, "Receiver", msg->titleLength);
            strlcpy(msg->msg, params->receiver_address, msg->msgLength);
            ret = true;
            break;
        case 3:
            strlcpy(msg->title, "Owner", msg->titleLength);
            strlcpy(msg->msg, params->owner_address, msg->msgLength);
            ret = true;
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            break;
    }
    return ret;
}