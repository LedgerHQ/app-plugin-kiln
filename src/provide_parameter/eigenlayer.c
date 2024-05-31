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

/**
 * @brief Set a bit in a bitfield
 *
 * @param bf: bitfield to update
 * @param index: index of the bit to update
 * @param value: value to set the bit to (0 or 1)
 *
 */
void set_bit(bitfield *bf, int index, bool value) {
    if (index < 0 || index >= MAX_DISPLAYABLE_LR_STRATEGIES_COUNT) {
        return;  // Index out of range
    }
    if (value) {
        bf->bits |= (1U << index);
    } else {
        bf->bits &= ~(1U << index);
    }
}

/**
 * @brief Compare two addresses
 *
 * @param a: first address
 * @param b: second address
 *
 * @return true if the addresses are the same
 */
bool compare_addresses(const char a[ADDRESS_STR_LEN], const char b[ADDRESS_STR_LEN]) {
    for (size_t i = 0; i < ADDRESS_STR_LEN; i += 1) {
        if (tolower((unsigned char) a[i]) != tolower((unsigned char) b[i])) {
            return false;
        }
    }
    return true;
}

/**
 * @brief If address is a known erc20, update lr display context with its name
 * otherwise set it to unkwown (UNKNOWN_LR_ERC20)
 *
 * @param address: address to compare
 *
 * @returns index of the erc20 in the context or UNKNOWN_LR_ERC20 if not found
 */
uint8_t find_lr_known_erc20(const char address[ADDRESS_STR_LEN]) {
    for (size_t i = 0; i < LR_STRATEGIES_COUNT; i++) {
        if (compare_addresses(address, lr_erc20_addresses[i])) {
            return i;
        }
    }
    // if unknown erc20, indicate it
    return UNKNOWN_LR_ERC20;
}

/**
 * @brief If address is a known strategy, update lr display context with its name
 * otherwise set it to unkwown (UNKNOWN_LR_STRATEGY)
 *
 * @param address: address to compare
 *
 * @returns index of the strategy in the context or UNKNOWN_LR_STRATEGY if not found
 */
uint8_t find_lr_known_strategy(const char address[ADDRESS_STR_LEN]) {
    for (size_t i = 0; i < LR_STRATEGIES_COUNT; i++) {
        if (compare_addresses(address, lr_strategy_addresses[i])) {
            return i;
        }
    }
    // if unknown strategy, indicate it
    return UNKNOWN_LR_STRATEGY;
}

/**
 * @brief Handle the parameters for the depositIntoStrategy(strategy,erc20,amount)
 * selector
 *
 * @param msg: message containing the parameter
 * @param context: context to update
 *
 */
void handle_lr_deposit_into_strategy(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // function depositIntoStrategy(
    //    address strategy,
    //    address erc20,
    //    uint256 amount
    // ) external
    //
    // **************************************************************************
    // example
    // [  0] selector
    // [  4] strategy
    // [ 36] erc20
    // [ 68] amount

    uint8_t buffer[ADDRESS_LENGTH];
    char address_buffer[ADDRESS_STR_LEN];

    switch (context->next_param) {
        case LR_DEPOSIT_INTO_STRATEGY_STRATEGY:
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);
            context->param_data.lr_deposit.strategy_to_display =
                find_lr_known_strategy(address_buffer);

            context->next_param = LR_DEPOSIT_INTO_STRATEGY_TOKEN;
            break;
        case LR_DEPOSIT_INTO_STRATEGY_TOKEN:
            copy_address(buffer, msg->parameter, sizeof(buffer));
            getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);
            context->param_data.lr_deposit.erc20_to_display = find_lr_known_erc20(address_buffer);

            context->next_param = LR_DEPOSIT_INTO_STRATEGY_AMOUNT;
            break;
        case LR_DEPOSIT_INTO_STRATEGY_AMOUNT:
            copy_parameter(context->param_data.lr_deposit.erc20_amount_to_display,
                           msg->parameter,
                           sizeof(context->param_data.lr_deposit.erc20_amount_to_display));
            context->next_param = LR_DEPOSIT_INTO_STRATEGY_UNEXPECTED_PARAMETER;
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

/**
 * @brief Handle the parameters for the queueWithdrawals(queuedWithdrawals[]) selector
 *
 * @param msg: message containing the parameter
 * @param context: context to update
 *
 */
void handle_lr_queue_withdrawals(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // struct QueuedWithdrawalParams {
    //      address[] strategies;
    //      uint256[] shares;
    //      address withdrawer;
    // }
    //
    // function queueWithdrawals(
    //    QueuedWithdrawalParams[] calldata queuedWithdrawalParams
    // ) external
    //
    // **************************************************************************
    // example for 2 queue withdrawals with 2 strategies each (2x2 dimension)
    // [  0] selector
    // [  4] QueuedWithdrawalParams_offset
    // [ 36] QueuedWithdrawalParams_length
    // [ 68] QueuedWithdrawalParams_0_offset
    // [100] QueuedWithdrawalParams_1_offset
    // [132] QueuedWithdrawalParams_0
    //        [132] strategies_offset
    //        [164] shares_offset
    //        [196] withdrawer
    //        [228] strategies_length
    //        [260] strategies_0
    //        [292] strategies_1
    //        [324] shares_length
    //        [356] shares_0
    //        [388] shares_1
    // [388] QueuedWithdrawalParams_1
    //        [388] strategies_offset
    //        [420] shares_offset
    //        [452] withdrawer
    //        [484] strategies_length
    //        [516] strategies_0
    //        [548] strategies_1
    //        [580] shares_length
    //        [612] shares_0
    //        [644] shares_1

    lr_queue_withdrawals_t *params = &context->param_data.lr_queue_withdrawals;

    switch (context->next_param) {
        // ********************************************************************
        // QUEUEWITHDRAWALPARAMS[]
        // ********************************************************************
        case LR_QUEUE_WITHDRAWALS_QWITHDRAWALS_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);
            if (offset != PARAM_OFFSET) {
                // valid offset should only skip this offset
                PRINTF("Unexpected parameter offset\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = LR_QUEUE_WITHDRAWALS_QWITHDRAWALS_LENGTH;
            break;
        }
        case LR_QUEUE_WITHDRAWALS_QWITHDRAWALS_LENGTH:
            U2BE_from_parameter(msg->parameter, &params->queued_withdrawals_count);
            params->current_item_count = params->queued_withdrawals_count;
            context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRUCT_OFFSET;
            break;

        // ********************************************************************
        // QUEUEWITHDRAWALPARAMS STRUCT
        // ********************************************************************
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRUCT_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);

            // We have limited size on the context and can't store all the offset values
            // of the queuedWithdrawal structs. So we compute their checksum and expect to
            // be able to recompute it using the offset of the parsed structures later.
            // _preview will be equal to _value at the end of the parsing if everything is fine
            checksum_offset_params_t h_params;
            memset(&h_params, 0, sizeof(h_params));
            memcpy(&h_params.prev_checksum,
                   &(params->qwithdrawals_offsets_checksum_preview),
                   sizeof(h_params.prev_checksum));

            // we hash the previous checksum with the offset of the beginning of the structure.
            // the offset we parse is actually after SELECTOR + the 2 above param so we add them to
            // it.
            h_params.new_offset = offset + SELECTOR_SIZE + PARAM_OFFSET * 2;

            if (cx_keccak_256_hash((void *) &h_params,
                                   sizeof(h_params),
                                   params->qwithdrawals_offsets_checksum_preview) != CX_OK) {
                PRINTF("unable to compute keccak hash\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            // we skip all the queuewithdrawal structs offsets
            PRINTF("CURRENT ITEM COUNT: %d\n", params->current_item_count);
            if (params->current_item_count > 0) {
                params->current_item_count -= 1;
            }

            if (params->current_item_count == 0) {
                context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_OFFSET;
            }
            break;
        }
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);
            // here we are at the beginning of the queuedWithdrawal struct, so we want
            // to compute the offset of the struct we're at for the queuedWithdrawal struct array
            // offsets checksum

            checksum_offset_params_t h_params;
            memset(&h_params, 0, sizeof(h_params));
            memcpy(&h_params.prev_checksum,
                   &(params->qwithdrawals_offsets_checksum_value),
                   sizeof(h_params.prev_checksum));
            h_params.new_offset = msg->parameterOffset;

            if (cx_keccak_256_hash((void *) &h_params,
                                   sizeof(h_params),
                                   params->qwithdrawals_offsets_checksum_value) != CX_OK) {
                PRINTF("unable to compute keccak hash\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            if (params->queued_withdrawals_count == 1) {
                // if we are on the last item of the array of queuedWithdrawal struct
                // we can check the checksum
                PRINTF("CHECKSUMS %.*H %.*H\n",
                       sizeof(params->qwithdrawals_offsets_checksum_preview),
                       params->qwithdrawals_offsets_checksum_preview,
                       sizeof(params->qwithdrawals_offsets_checksum_value),
                       params->qwithdrawals_offsets_checksum_value);
                if (memcmp(params->qwithdrawals_offsets_checksum_preview,
                           params->qwithdrawals_offsets_checksum_value,
                           sizeof(params->qwithdrawals_offsets_checksum_preview)) != 0) {
                    PRINTF("Checksums do not match\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            if (offset != PARAM_OFFSET * 3) {
                // valid offset should only skip this + shares_offset + withdrawer
                PRINTF("Unexpected parameter offset %d != %d\n", offset, PARAM_OFFSET * 3);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_OFFSET;
            break;
        }
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_OFFSET:
            // we can only check this value once we know strategies array length
            U2BE_from_parameter(msg->parameter, &params->shares_array_offset);

            context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_WITHDRAWER;
            break;
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_WITHDRAWER: {
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                char address_buffer[ADDRESS_STR_LEN];
                getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);
                // we only support same withdrawer accross all the withdrawals
                if (params->withdrawer[0] == '\0') {
                    memcpy(params->withdrawer, address_buffer, sizeof(params->withdrawer));
                } else if (strcmp(params->withdrawer, address_buffer) != 0) {
                    PRINTF("Unexpected withdrawer address, %s != expected %s\n",
                           msg->parameter,
                           params->withdrawer);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }
            context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_LENGTH;
            break;
        }
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_LENGTH: {
            // get number of item to parse
            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            PRINTF("STRATEGIES LENGTH: %d\n", params->current_item_count);

            {  // check that the shares array offset is correct
                // -- 1. we skip shares_offset + withdrawer + strategies_length
                uint16_t should_be = PARAM_OFFSET * 3
                                     // -- 2. we skip the strategies array length
                                     + PARAM_OFFSET + PARAM_OFFSET * params->current_item_count;
                ;
                if (params->shares_array_offset != should_be) {
                    PRINTF("Unexpected parameter offset %d != %d\n",
                           params->shares_array_offset,
                           should_be);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            if (params->current_item_count == 0) {
                // if no strategies we go to the shares array
                context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_LENGTH;
            } else {
                context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__STRATEGIES_ITEM;
            }
            break;
        }
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__STRATEGIES_ITEM: {
            // get strategy we need to display
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                char address_buffer[ADDRESS_STR_LEN];
                getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);

                uint8_t strategy_index = find_lr_known_strategy(address_buffer);
                params->strategies[params->strategies_count] =
                    (strategy_index != UNKNOWN_LR_STRATEGY) ? strategy_index : UNKNOWN_LR_STRATEGY;

                PRINTF("STRATEGY #: %d STRATEGY: %d\n", params->strategies_count, strategy_index);
            }

            // we just processed one strategy item
            params->strategies_count += 1;
            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                // when we arrive at the end of the strategies array we go to the shares array
                context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_LENGTH;
            }
            break;
        }
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_LENGTH:
            // get number of items to parse
            U2BE_from_parameter(msg->parameter, &params->current_item_count);

            context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__SHARES_ITEM;
            break;
        case LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__SHARES_ITEM:
            // we skip parsing shares item as they are not needed for clearsigning
            // as not having ETH / ERC20 amount to display would confuse users
            if (params->current_item_count > 0) {
                params->current_item_count -= 1;
            }
            if (params->current_item_count == 0) {
                // here we arrive at the end of the queuedWithdrawal array element

                // check if there are other queuedWithdrawals to parse
                params->queued_withdrawals_count -= 1;
                if (params->queued_withdrawals_count == 0) {
                    // if not we finished parsing
                    context->next_param = LR_QUEUE_WITHDRAWALS_UNEXPECTED_PARAMETER;
                } else {
                    // if there are other queuedWithdrawals we go back to parsing the
                    // next queueWithdrawal struct
                    context->next_param = LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_OFFSET;
                }
            }
            break;

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}

/**
 * @brief Handle the parameters for the completeQueuedWithdrawals(Withdrawal[] withdrawals,
 * address[][] tokens, uint256[] middlewareTimesIndexes, bool[] receiveAsTokens) selector
 * @param msg: message containing the parameter
 * @param context: context to update
 *
 */
void handle_lr_complete_queued_withdrawals(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    //  struct Withdrawal {
    // 	    address staker;
    // 	    address delegatedTo;
    // 	    address withdrawer;
    // 	    uint256 nonce;
    // 	    uint32 startBlock;
    // 	    address[] strategies;
    // 	    uint256[] shares;
    // }
    //
    // function completeQueuedWithdrawals(
    // 	    Withdrawal[] calldata withdrawals,
    // 	    address[][] calldata tokens,
    // 	    uint256[] calldata middlewareTimesIndexes,
    // 	    bool[] calldata receiveAsTokens
    // ) external
    //
    // **************************************************************************
    // example for 2 complete queued withdrawals with 2 tokens each
    // [  0] selector
    // [  4] withdrawals_offset
    // [ 36] tokens_offset
    // [ 68] middlewareTimesIndexes_offset
    // [100] receiveAsTokens_offset
    // [132] withdrawals_length
    // [164] withdrawals_0_offset
    // [196] withdrawals_1_offset
    // [228] withdrawals_0
    //        [228] staker
    //        [260] delegatedTo
    //        [292] withdrawer
    //        [324] nonce
    //        [356] startBlock
    //        [388] strategies_offset
    //        [420] shares_offset
    //        [452] strategies_length
    //        [484] strategies_0
    //        [516] strategies_1
    //        [548] shares_length
    //        [580] shares_0
    //        [612] shares_1
    // [644] withdrawals_1
    //        [644] staker
    //        [676] delegatedTo
    //        [708] withdrawer
    //        [740] nonce
    //        [772] startBlock
    //        [804] strategies_offset
    //        [836] shares_offset
    //        [868] strategies_length
    //        [900] strategies_0
    //        [932] strategies_1
    //        [964] shares_length
    //        [996] shares_0
    //        [1028] shares_1
    // [1060] tokens_length
    // [1092] tokens_0_offset
    // [1124] tokens_1_offset
    // [1156] tokens_0
    //        [1156] tokens_0_length
    //        [1188] tokens_0_0
    //        [1220] tokens_0_1
    // [1252] tokens_1
    //        [1252] tokens_1_length
    //        [1284] tokens_1_0
    //        [1316] tokens_1_1
    // [1348] middlewareTimesIndexes_length
    // [1380] middlewareTimesIndexes_0
    // [1412] middlewareTimesIndexes_1
    // [1444] receiveAsTokens_length
    // [1476] receiveAsTokens_0
    // [1508] receiveAsTokens_1
    lr_complete_queued_withdrawals_t *params = &context->param_data.lr_complete_queued_withdrawals;

    switch (context->next_param) {
        // ********************************************************************
        // FUNCTION PARAMETERS PARSING
        // ********************************************************************
        case LRCQW_WITHDRAWALS_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);
            if (offset != PARAM_OFFSET * 4) {
                // valid offset should only skip this offset + tokens + middlewareTimesIndexes +
                // receiveAsTokens offsets
                PRINTF("Unexpected parameter offset %d != %d\n", offset, PARAM_OFFSET);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = LRCQW_TOKENS_OFFSET;
            break;
        }
        case LRCQW_TOKENS_OFFSET:
            U2BE_from_parameter(msg->parameter, &(params->tokens_offset));
            context->next_param = LRCQW_MIDDLEWARE_TIMES_INDEXES_OFFSET;
            break;
        case LRCQW_MIDDLEWARE_TIMES_INDEXES_OFFSET:
            U2BE_from_parameter(msg->parameter, &(params->middlewareTimesIndexes_offset));
            context->next_param = LRCQW_RECEIVE_AS_TOKENS_OFFSET;
            break;
        case LRCQW_RECEIVE_AS_TOKENS_OFFSET:
            U2BE_from_parameter(msg->parameter, &(params->receiveAsTokens_offset));
            context->next_param = LRCQW_WITHDRAWALS_LENGTH;
            break;

        // ********************************************************************
        // WITHDRAWAL[] PARSING
        // ********************************************************************
        case LRCQW_WITHDRAWALS_LENGTH:

            U2BE_from_parameter(msg->parameter, &params->parent_item_count);
            params->current_item_count = params->parent_item_count;
            PRINTF("LRCQW_WITHDRAWALS_LENGTH: %d\n", params->parent_item_count);

            if (params->parent_item_count == 0) {
                context->next_param = LRCQW_TOKENS_LENGTH;
            } else {
                context->next_param = LRCQW_WITHDRAWALS__OFFSET_ITEMS;
            }
            break;
        case LRCQW_WITHDRAWALS__OFFSET_ITEMS: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);

            // We have limited size on the context and can't store all the offset values
            // of the Withdrawal structs. So we compute their checksum and expect to
            // be able to recompute it using the offset of the parsed structures later.
            // _preview will be equal to _value at the end of the parsing if everything is fine
            checksum_offset_params_t h_params;
            memset(&h_params, 0, sizeof(h_params));
            memcpy(&h_params.prev_checksum,
                   &(params->withdrawals_offsets_checksum_preview),
                   sizeof(h_params.prev_checksum));
            // we hash the previous checksum with the offset of the beginning of the structure.
            // the offset we parse is actually after SELECTOR + the 5 above param so we add them to
            // it.
            h_params.new_offset = offset + SELECTOR_SIZE + PARAM_OFFSET * 5;

            if (cx_keccak_256_hash((void *) &h_params,
                                   sizeof(h_params),
                                   params->withdrawals_offsets_checksum_preview) != CX_OK) {
                PRINTF("unable to compute keccak hash\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                context->next_param = LRCQW_WITHDRAWALS__ITEM__STAKER;
            }
            break;
        }

        // ********************************************************************
        // WITHDRAWAL STRUCT PARSING
        // ********************************************************************
        case LRCQW_WITHDRAWALS__ITEM__STAKER: {
            // here we are at the beginning of the queuedWithdrawal struct, so we want
            // to compute the offset of the struct we're at for the queuedWithdrawal struct array
            // offsets checksum

            checksum_offset_params_t h_params;
            memset(&h_params, 0, sizeof(h_params));
            memcpy(&(h_params.prev_checksum),
                   &(params->withdrawals_offsets_checksum_value),
                   sizeof(h_params.prev_checksum));
            h_params.new_offset = msg->parameterOffset;

            if (cx_keccak_256_hash((void *) &h_params,
                                   sizeof(h_params),
                                   params->withdrawals_offsets_checksum_value) != CX_OK) {
                PRINTF("unable to compute keccak hash\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            if (params->parent_item_count == 1) {
                // if we are on the last item of the array of withdrawal struct
                // we can check the checksum
                PRINTF("CHECKSUMS %.*H %.*H\n",
                       sizeof(params->withdrawals_offsets_checksum_preview),
                       params->withdrawals_offsets_checksum_preview,
                       sizeof(params->withdrawals_offsets_checksum_value),
                       params->withdrawals_offsets_checksum_value);
                if (memcmp(params->withdrawals_offsets_checksum_preview,
                           params->withdrawals_offsets_checksum_value,
                           sizeof(params->withdrawals_offsets_checksum_preview)) != 0) {
                    PRINTF("Checksums do not match\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }
            context->next_param = LRCQW_WITHDRAWALS__ITEM__DELEGATED_TO;
            break;
        }
        case LRCQW_WITHDRAWALS__ITEM__DELEGATED_TO:
            context->next_param = LRCQW_WITHDRAWALS__ITEM__WITHDRAWER;
            break;
        case LRCQW_WITHDRAWALS__ITEM__WITHDRAWER: {
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                // we only support same withdrawer accross all the withdrawals
                if (allzeroes(params->withdrawer, sizeof(params->withdrawer)) == 1) {
                    memcpy(params->withdrawer, buffer, sizeof(params->withdrawer));
                } else if (strcmp((const char *) params->withdrawer, (const char *) buffer) != 0) {
                    PRINTF("Unexpected withdrawer address, %s != expected %s\n",
                           msg->parameter,
                           params->withdrawer);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            context->next_param = LRCQW_WITHDRAWALS__ITEM__NONCE;
            break;
        }
        case LRCQW_WITHDRAWALS__ITEM__NONCE:
            context->next_param = LRCQW_WITHDRAWALS__ITEM__START_BLOCK;
            break;
        case LRCQW_WITHDRAWALS__ITEM__START_BLOCK:
            context->next_param = LRCQW_WITHDRAWALS__ITEM__STRATEGIES_OFFSET;
            break;
        case LRCQW_WITHDRAWALS__ITEM__STRATEGIES_OFFSET: {
            {
                uint16_t offset;

                U2BE_from_parameter(msg->parameter, &offset);
                if (offset != PARAM_OFFSET * 7) {
                    // valid offset should only skip the above elements in the structure + this one
                    // + shares offset
                    PRINTF("Unexpected parameter offset %d != %d\n", offset, PARAM_OFFSET * 7);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }
            context->next_param = LRCQW_WITHDRAWALS__ITEM__SHARES_OFFSET;
            break;
        }
        case LRCQW_WITHDRAWALS__ITEM__SHARES_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);

            // save offset to verify it on array start
            // offset starts at the beginning of the structure (current scope)
            params->cached_offset = offset + (msg->parameterOffset - PARAM_OFFSET * 6);
        }
            context->next_param = LRCQW_WITHDRAWALS__ITEM__STRATEGIES_LENGTH;
            break;
        case LRCQW_WITHDRAWALS__ITEM__STRATEGIES_LENGTH:

            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            PRINTF("LRCQW_WITHDRAWALS__ITEM__STRATEGIES_LENGTH: %d\n", params->current_item_count);

            if (params->current_item_count == 0) {
                context->next_param = LRCQW_WITHDRAWALS__ITEM__SHARES_LENGTH;
            } else {
                context->next_param = LRCQW_WITHDRAWALS__ITEM__STRATEGIES__ITEMS;
            }
            break;
        case LRCQW_WITHDRAWALS__ITEM__STRATEGIES__ITEMS: {
            // get strategy we need to display
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                char address_buffer[ADDRESS_STR_LEN];
                getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);

                uint8_t strategy_index = find_lr_known_strategy(address_buffer);
                if (params->strategies_count >= MAX_DISPLAYABLE_LR_STRATEGIES_COUNT) {
                    PRINTF("Too many strategies to display, reverting\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
                uint8_t withdrawal = params->withdrawals_count;

                uint8_t strategy =
                    (strategy_index != UNKNOWN_LR_STRATEGY) ? strategy_index : UNKNOWN_LR_STRATEGY;

                if (withdrawal >= 16 || strategy >= 16) {
                    PRINTF("INVALID WITHDRAWAL #: %d STRATEGY #: %d\n", withdrawal, strategy);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }

                params->strategies[params->strategies_count] =
                    (withdrawal << 4) | (strategy & 0x0F);
                ;
            }

            // we just processed one strategy item
            params->strategies_count += 1;
            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                // when we arrive at the end of the strategies array we go to the shares array
                context->next_param = LRCQW_WITHDRAWALS__ITEM__SHARES_LENGTH;
            }
            break;
        }
        case LRCQW_WITHDRAWALS__ITEM__SHARES_LENGTH:

            if (params->cached_offset != msg->parameterOffset) {
                PRINTF("Unexpected shares parameter offset %d != %d\n",
                       params->cached_offset,
                       msg->parameterOffset);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            PRINTF("LRCQW_WITHDRAWALS__ITEM__SHARES_LENGTH: %d\n", params->current_item_count);

            if (params->current_item_count == 0 && params->parent_item_count > 0) {
                // shares array is empty AND we have other queuedWithdrawals to parse
                params->parent_item_count -= 1;
                params->withdrawals_count += 1;
                context->next_param = LRCQW_WITHDRAWALS__ITEM__STAKER;
            } else {
                context->next_param = LRCQW_WITHDRAWALS__ITEM__SHARES__ITEMS;
            }
            break;
        case LRCQW_WITHDRAWALS__ITEM__SHARES__ITEMS:

            // we don't need to parse shares items as they are not needed for clearsigning
            // as not having ETH / ERC20 amount to display would confuse users
            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                // we arrive at the end of the Withdrawal struct
                params->parent_item_count -= 1;
                params->withdrawals_count += 1;
                if (params->parent_item_count == 0) {
                    // we arrive at the end of the Withdrawals array
                    context->next_param = LRCQW_TOKENS_LENGTH;
                } else {
                    // we have other Withdrawals to parse
                    context->next_param = LRCQW_WITHDRAWALS__ITEM__STAKER;
                }
            }
            break;

        // ********************************************************************
        // TOKENS[][] PARSING
        //       ^
        // ********************************************************************
        case LRCQW_TOKENS_LENGTH:

            if (params->tokens_offset != msg->parameterOffset - SELECTOR_SIZE) {
                PRINTF("Unexpected tokens parameter offset %d != %d\n",
                       params->tokens_offset,
                       msg->parameterOffset);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            U2BE_from_parameter(msg->parameter, &params->parent_item_count);
            if (params->parent_item_count > params->withdrawals_count) {
                PRINTF("Unexpected number of tokens, %d > withdrawals %d\n",
                       params->parent_item_count,
                       params->withdrawals_count);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            params->current_item_count = params->parent_item_count;
            PRINTF("LRCQW_TOKENS_LENGTH: %d\n", params->parent_item_count);

            // reset checksums and cached offset
            memset(&params->withdrawals_offsets_checksum_preview,
                   0,
                   sizeof(params->withdrawals_offsets_checksum_preview));
            memset(&params->withdrawals_offsets_checksum_value,
                   0,
                   sizeof(params->withdrawals_offsets_checksum_value));
            params->cached_offset = 0;

            if (params->parent_item_count == 0) {
                context->next_param = LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH;
            } else {
                context->next_param = LRCQW_TOKENS__OFFSET_ITEMS;
            }
            break;
        case LRCQW_TOKENS__OFFSET_ITEMS: {
            {
                // We have limited size on the context and can't store all the offset values
                // of the tokens structs. So we compute their checksum and expect to
                // be able to recompute it using the offset of the parsed items later.
                // _preview will be equal to _value at the end of the parsing if everything is fine
                checksum_offset_params_t h_params;
                memset(&h_params, 0, sizeof(h_params));
                memcpy(&h_params.prev_checksum,
                       &(params->withdrawals_offsets_checksum_preview),
                       sizeof(h_params.prev_checksum));
                // we hash the previous checksum with the offset we receive.
                uint16_t offset;
                U2BE_from_parameter(msg->parameter, &offset);

                // if we are on the first element of the array, we save the offset, which all
                // received offset values will be based on for checksum computation
                if (params->cached_offset == 0) {
                    params->cached_offset = msg->parameterOffset;
                }

                h_params.new_offset = offset + params->cached_offset;

                if (cx_keccak_256_hash((void *) &h_params,
                                       sizeof(h_params),
                                       params->withdrawals_offsets_checksum_preview) != CX_OK) {
                    PRINTF("unable to compute keccak hash\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                context->next_param = LRCQW_TOKENS__ITEM__LENGTH;
            }
            break;
        }

        // ********************************************************************
        // TOKENS[][] PARSING
        //         ^
        // ********************************************************************
        case LRCQW_TOKENS__ITEM__LENGTH: {
            {
                checksum_offset_params_t h_params;
                memset(&h_params, 0, sizeof(h_params));
                memcpy(&h_params.prev_checksum,
                       &(params->withdrawals_offsets_checksum_value),
                       sizeof(h_params.prev_checksum));
                // we hash the previous checksum with the offset of the beginning of the structure.
                h_params.new_offset = msg->parameterOffset;

                if (cx_keccak_256_hash((void *) &h_params,
                                       sizeof(h_params),
                                       params->withdrawals_offsets_checksum_value) != CX_OK) {
                    PRINTF("unable to compute keccak hash\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            PRINTF("LRCQW_TOKENS__ITEM__LENGTH: %d\n", params->current_item_count);

            if (params->parent_item_count == 1) {
                // if we are on the last item of the array of tokens struct
                // we can check the checksum
                PRINTF("TOKENS[] CHECKSUMS %.*H %.*H\n",
                       sizeof(params->withdrawals_offsets_checksum_preview),
                       params->withdrawals_offsets_checksum_preview,
                       sizeof(params->withdrawals_offsets_checksum_value),
                       params->withdrawals_offsets_checksum_value);
                if (memcmp(params->withdrawals_offsets_checksum_preview,
                           params->withdrawals_offsets_checksum_value,
                           sizeof(params->withdrawals_offsets_checksum_preview)) != 0) {
                    PRINTF("Checksums do not match\n");
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }
            }

            if (params->current_item_count == 0) {
                context->next_param = LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH;
            } else {
                context->next_param = LRCQW_TOKENS__ITEM__ITEMS;
            }

            break;
        }
        case LRCQW_TOKENS__ITEM__ITEMS: {
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                char address_buffer[ADDRESS_STR_LEN];
                getEthDisplayableAddress(buffer, address_buffer, sizeof(address_buffer), 0);

                uint8_t token_index = find_lr_known_erc20(address_buffer);
                // we check if the token matches the corresponding strategy
                uint8_t strategy_index = params->strategies[params->tokens_count] & 0x0F;
                if (strategy_index != UNKNOWN_LR_STRATEGY && token_index != strategy_index) {
                    PRINTF("Token idx %d does not match strategy idx %d\n",
                           token_index,
                           strategy_index);
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    return;
                }

                params->tokens_count += 1;
            }

            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                // we arrive at the end of the tokens array
                params->parent_item_count -= 1;
                if (params->parent_item_count == 0) {
                    // if we don't have other Tokens to parse
                    context->next_param = LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH;
                } else {
                    // if we have other Tokens to parse
                    context->next_param = LRCQW_TOKENS__ITEM__LENGTH;
                }
            }
            break;
        }

        // ********************************************************************
        //  MIDDLEWARETIMESINDEXES[] PARSING
        // ********************************************************************
        case LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH:

            if (params->middlewareTimesIndexes_offset != msg->parameterOffset - SELECTOR_SIZE) {
                PRINTF("Unexpected middlewareTimesIndexes parameter offset %d != %d\n",
                       params->middlewareTimesIndexes_offset,
                       msg->parameterOffset);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            if (params->current_item_count > params->withdrawals_count) {
                PRINTF("Unexpected middlewareTimesIndexes parameter length %d > withdrawals %d\n",
                       params->current_item_count,
                       msg->parameterOffset);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            PRINTF("LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH: %d\n", params->current_item_count);

            if (params->current_item_count == 0) {
                context->next_param = LRCQW_RECEIVE_AS_TOKENS_LENGTH;
            } else {
                context->next_param = LRCQW_MIDDLEWARE_TIMES_INDEXES__ITEMS;
            }
            break;
        case LRCQW_MIDDLEWARE_TIMES_INDEXES__ITEMS:

            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                context->next_param = LRCQW_RECEIVE_AS_TOKENS_LENGTH;
            }
            break;

        // ********************************************************************
        //  RECEIVEASTOKENS[] PARSING
        // ********************************************************************
        case LRCQW_RECEIVE_AS_TOKENS_LENGTH:

            if (params->receiveAsTokens_offset != msg->parameterOffset - SELECTOR_SIZE) {
                PRINTF("Unexpected receiveAsTokens parameter offset %d != %d\n",
                       params->receiveAsTokens_offset,
                       msg->parameterOffset);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }

            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            if (params->current_item_count > params->withdrawals_count) {
                PRINTF("Unexpected receiveAsTokens length %d > withdrawals length %d\n",
                       params->current_item_count,
                       params->withdrawals_count);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            // we save the number of redelegations to parse
            params->relegations_count = params->current_item_count;
            PRINTF("LRCQW_RECEIVE_AS_TOKENS_LENGTH: %d\n", params->current_item_count);

            if (params->current_item_count == 0) {
                // reached the end
                context->next_param = LRCQW_UNEXPECTED_PARAMETER;
            } else {
                context->next_param = LRCQW_RECEIVE_AS_TOKENS__ITEMS;
            }
            break;
        case LRCQW_RECEIVE_AS_TOKENS__ITEMS: {
            {
                uint16_t index = params->relegations_count - params->current_item_count;

                uint16_t value;
                U2BE_from_parameter(msg->parameter, &value);
                // if false, token is redelegated
                set_bit(&(params->is_redelegated), index, value == 0);
                PRINTF("RECEIVE AS TOKENS #%d: %d\n", index, value);
            }
            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                // reached the end
                context->next_param = LRCQW_UNEXPECTED_PARAMETER;
            }
            break;
        }

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}

/**
 * @brief Handle the parameters for the delegateTo(address,(bytes,uint256),bytes32) selector
 *
 * @param msg: message containing the parameter
 * @param context: context to update
 *
 */
void handle_lr_delegate_to(ethPluginProvideParameter_t *msg, context_t *context) {
    // **************************************************************************
    // FUNCTION TO PARSE
    // **************************************************************************
    //
    // struct Signature {
    //     bytes signature;
    //     uint256 expiry;
    // }
    //
    // function delegateTo(
    //    address operator,
    //    Signature signature,
    //    bytes32 approverSalt
    // ) external
    //
    // **************************************************************************
    // example
    // [0] selector
    // [4] operator
    // [36] signature_offset
    // [68] approver_salt
    // [100] signature
    //      [100] signature.signature.offset
    //      [132] signature.expiry
    //      [164] signature.signature.length
    //      [192] signature.signature.items

    lr_delegate_to_t *params = &context->param_data.lr_delegate_to;

    switch (context->next_param) {
        case LR_DELEGATE_TO_OPERATOR: {
            {
                uint8_t buffer[ADDRESS_LENGTH];
                copy_address(buffer, msg->parameter, sizeof(buffer));
                getEthDisplayableAddress(buffer,
                                         params->operator_address,
                                         sizeof(params->operator_address),
                                         0);

                if (compare_addresses((const char *) params->operator_address,
                                      lr_kiln_operator_address)) {
                    params->is_kiln = true;
                }
            }
            context->next_param = LR_DELEGATE_TO_SIGNATURE_OFFSET;
            break;
        }
        case LR_DELEGATE_TO_SIGNATURE_OFFSET: {
            uint16_t offset;
            U2BE_from_parameter(msg->parameter, &offset);
            if (offset != PARAM_OFFSET * 3) {
                // valid offset should only skip this offset + approver_salt
                PRINTF("Unexpected parameter offset %d != %d\n", offset, PARAM_OFFSET * 3);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = LR_DELEGATE_TO_APPROVER_SALT;
            break;
        }
        case LR_DELEGATE_TO_APPROVER_SALT:
            context->next_param = LR_DELEGATE_TO_SIGNATURE_SIG_OFFSET;
            break;
        case LR_DELEGATE_TO_SIGNATURE_SIG_OFFSET: {
            uint16_t offset_sig;
            U2BE_from_parameter(msg->parameter, &offset_sig);
            if (offset_sig != PARAM_OFFSET * 2) {
                // valid offset should only skip this offset + expiry
                PRINTF("Unexpected parameter offset %d != %d\n", offset_sig, PARAM_OFFSET * 2);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                return;
            }
            context->next_param = LR_DELEGATE_TO_SIGNATURE_EXPIRY;
            break;
        }
        case LR_DELEGATE_TO_SIGNATURE_EXPIRY:
            context->next_param = LR_DELEGATE_TO_SIGNATURE_SIG_LENGTH;
            break;
        case LR_DELEGATE_TO_SIGNATURE_SIG_LENGTH:
            U2BE_from_parameter(msg->parameter, &params->current_item_count);
            PRINTF("LR_DELEGATE_TO_SIGNATURE_SIG_LENGTH: %d\n", params->current_item_count);

            if (params->current_item_count == 0) {
                context->next_param = LR_DELEGATE_TO_UNEXPECTED_PARAMETER;
            } else {
                context->next_param = LR_DELEGATE_TO_SIGNATURE_SIG_ITEMS;
            }
            break;
        case LR_DELEGATE_TO_SIGNATURE_SIG_ITEMS:
            // we skip parsing signature items as they are not needed for clearsigning

            params->current_item_count -= 1;
            if (params->current_item_count == 0) {
                context->next_param = LR_DELEGATE_TO_UNEXPECTED_PARAMETER;
            }
            break;
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}