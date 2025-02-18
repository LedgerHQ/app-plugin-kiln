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
 * (c) 2022-2025 Kiln
 *
 * contact@kiln.fi
 ********************************************************************************/

#include "kiln_plugin.h"

void handle_defi_deposit(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function deposit(uint256 assets, address receiver)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] assets_amount
    // [36] receiver_address
    // **************************************************************************
    defi_deposit_t *params = &context->param_data.defi_deposit;

    switch (context->next_param) {
        case DEFI_DEPOSIT_ASSETS_AMOUNT:
            copy_parameter(params->assets_amount, msg->parameter, sizeof(params->assets_amount));
            context->next_param = DEFI_DEPOSIT_RECEIVER_ADDRESS;
            break;
        case DEFI_DEPOSIT_RECEIVER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            char address_buffer[ADDRESS_STR_LEN];
            getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);
            memcpy(params->receiver_address, address_buffer, sizeof(params->receiver_address));
            context->next_param = DEFI_DEPOSIT_UNEXPECTED_PARAMETER;
            break;
        }
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_mint(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function mint(uint256 shares, address receiver)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] shares_amount
    // [36] receiver_address
    // **************************************************************************
    defi_mint_t *params = &context->param_data.defi_mint;

    switch (context->next_param) {
        case DEFI_MINT_SHARES_AMOUNT:
            copy_parameter(params->shares_amount, msg->parameter, sizeof(params->shares_amount));
            context->next_param = DEFI_MINT_RECEIVER_ADDRESS;
            break;
        case DEFI_MINT_RECEIVER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer,
                                     params->receiver_address,
                                     sizeof(params->receiver_address),
                                     0);
            context->next_param = DEFI_MINT_UNEXPECTED_PARAMETER;
            break;
        }
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_withdraw(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function withdraw(uint256 assets, address receiver, address owner)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] assets_amount
    // [36] receiver_address
    // [68] owner_address
    // **************************************************************************
    defi_withdraw_t *params = &context->param_data.defi_withdraw;

    switch (context->next_param) {
        case DEFI_WITHDRAW_ASSETS_AMOUNT:
            copy_parameter(params->assets_amount, msg->parameter, sizeof(params->assets_amount));
            context->next_param = DEFI_WITHDRAW_RECEIVER_ADDRESS;
            break;
        case DEFI_WITHDRAW_RECEIVER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer,
                                     params->receiver_address,
                                     sizeof(params->receiver_address),
                                     0);
            context->next_param = DEFI_WITHDRAW_OWNER_ADDRESS;
            break;
        }
        case DEFI_WITHDRAW_OWNER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer,
                                     params->owner_address,
                                     sizeof(params->owner_address),
                                     0);
            context->next_param = DEFI_WITHDRAW_UNEXPECTED_PARAMETER;
            break;
        }
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_redeem(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function redeem(uint256 shares, address receiver, address owner)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] shares_amount
    // [36] receiver_address
    // [68] owner_address
    // **************************************************************************
    defi_redeem_t *params = &context->param_data.defi_redeem;

    switch (context->next_param) {
        case DEFI_REDEEM_SHARES_AMOUNT:
            copy_parameter(params->shares_amount, msg->parameter, sizeof(params->shares_amount));
            context->next_param = DEFI_REDEEM_RECEIVER_ADDRESS;
            break;
        case DEFI_REDEEM_RECEIVER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer,
                                     params->receiver_address,
                                     sizeof(params->receiver_address),
                                     0);
            context->next_param = DEFI_REDEEM_OWNER_ADDRESS;
            break;
        }
        case DEFI_REDEEM_OWNER_ADDRESS: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer,
                                     params->owner_address,
                                     sizeof(params->owner_address),
                                     0);
            context->next_param = DEFI_REDEEM_UNEXPECTED_PARAMETER;
            break;
        }
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_approve(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function approve(address spender, uint256 amount)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] spender
    // [36] amount
    // **************************************************************************
    defi_approve_t *params = &context->param_data.defi_approve;

    switch (context->next_param) {
        case DEFI_APPROVE_SPENDER: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, params->spender, sizeof(params->spender), 0);
            context->next_param = DEFI_APPROVE_AMOUNT;
            break;
        }
        case DEFI_APPROVE_AMOUNT:
            copy_parameter(params->amount, msg->parameter, sizeof(params->amount));
            context->next_param = DEFI_APPROVE_UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_transfer(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function transfer(address to, uint256 amount)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] to
    // [36] amount
    // **************************************************************************
    defi_transfer_t *params = &context->param_data.defi_transfer;

    switch (context->next_param) {
        case DEFI_TRANSFER_TO: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, params->to, sizeof(params->to), 0);
            context->next_param = DEFI_TRANSFER_AMOUNT;
            break;
        }
        case DEFI_TRANSFER_AMOUNT:
            copy_parameter(params->amount, msg->parameter, sizeof(params->amount));
            context->next_param = DEFI_TRANSFER_UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

void handle_defi_transfer_from(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function transferFrom(address from, address to, uint256 amount)
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] from
    // [36] to
    // [68] amount
    // **************************************************************************
    defi_transfer_from_t *params = &context->param_data.defi_transfer_from;

    switch (context->next_param) {
        case DEFI_TRANSFER_FROM_FROM: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, params->from, sizeof(params->from), 0);
            context->next_param = DEFI_TRANSFER_FROM_TO;
            break;
        }
        case DEFI_TRANSFER_FROM_TO: {
            uint8_t buffer[ADDRESS_LENGTH];
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, params->to, sizeof(params->to), 0);
            context->next_param = DEFI_TRANSFER_FROM_AMOUNT;
            break;
        }
        case DEFI_TRANSFER_FROM_AMOUNT:
            copy_parameter(params->amount, msg->parameter, sizeof(params->amount));
            context->next_param = DEFI_TRANSFER_FROM_UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}