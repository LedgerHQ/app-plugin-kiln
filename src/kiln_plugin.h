#pragma once

#include <string.h>

#include "eth_internals.h"
#include "eth_plugin_interface.h"

#define PLUGIN_NAME        "Kiln"
#define VALIDATOR_KEY_SIZE 48

// Available selectors:
//
// V1 selectors
// --- 0. deposit()
// --- 1. withdraw(validator_pubkey)
// --- 2. withdrawEL(validator_pubkey)
// --- 3. withdrawCL(validator_pubkey)
// --- 4. batchWithdraw(validator_pubkeys)
// --- 5. batchWithdrawEL(validator_pubkeys)
// --- 6. batchWithdrawCL(validator_pubkeys)
// --- 7. requestValidatorsExit(validator_pubkeys)
//
// V2 selectors
// --- 8. stake()
// --- 9. requestExit(shares_count)
// --- 10. multiClaim(exit_queues, ticket_ids, cask_ids)
// --- 11. claim(uint256[],uint32[],uint16)
//
// LR selectors
// --- 12. depositIntoStrategy(address,address,uint256)
// --- 13. queueWithdrawal(uint256[],address[],uint256[],address,bool)
// --- 14.
// completeQueuedWithdrawal((address,address,address,uint256,uint32,address[],uint256[]),address[],uint256,bool)
//
#define NUM_SELECTORS 15

// Selectors available (see mapping above).
typedef enum {
    KILN_V1_DEPOSIT = 0,
    KILN_V1_WITHDRAW,
    KILN_V1_WITHDRAW_EL,
    KILN_V1_WITHDRAW_CL,
    KILN_V1_BATCH_WITHDRAW,
    KILN_V1_BATCH_WITHDRAW_EL,
    KILN_V1_BATCH_WITHDRAW_CL,
    KILN_V1_REQUEST_EXIT,
    KILN_V2_STAKE,
    KILN_V2_REQUEST_EXIT,
    KILN_V2_MULTICLAIM,
    KILN_V2_CLAIM,
    KILN_LR_DEPOSIT_INTO_STRATEGY,
    KILN_LR_QUEUE_WITHDRAWAL,
    KILN_LR_COMPLETE_QUEUED_WITHDRAWAL,
} selector_t;

extern const uint32_t KILN_SELECTORS[NUM_SELECTORS];

// ADDRESS_STR_LEN is 0x + addr + \0
#define ADDRESS_STR_LEN 43

// All supported ERC20 tokens have 18 decimals on mainnet.
#define ERC20_DECIMALS 18

// Parameters for deposit selector.
typedef enum {
    DEPOSIT_UNEXPECTED_PARAMETER,
} deposit_parameters;

// Parameters for LR deposit into strategy selector.
typedef enum {
    LR_DEPOSIT_INTO_STRATEGY_STRATEGY = 0,
    LR_DEPOSIT_INTO_STRATEGY_TOKEN,
    LR_DEPOSIT_INTO_STRATEGY_AMOUNT,
    LR_DEPOSIT_INTO_STRATEGY_UNEXPECTED_PARAMETER,
} lr_deposit_into_strategy_parameters;

// Parameters for LR queue withdrawal selector.
typedef enum {
    LR_QUEUE_WITHDRAWAL_STRATEGY_INDEXES_OFFSET = 0,
    LR_QUEUE_WITHDRAWAL_STRATEGIES_OFFSET,
    LR_QUEUE_WITHDRAWAL_SHARES_OFFSET,
    LR_QUEUE_WITHDRAWAL_WITHDRAWER,
    LR_QUEUE_WITHDRAWAL_UNDELEGATEIFPOSSIBLE,
    LR_QUEUE_WITHDRAWAL_UNEXPECTED_PARAMETER
} lr_queue_withdrawal_parameters;

// Parameters for LR complete queued withdrawal selector.
typedef enum {
    LR_COMPLETE_QUEUED_WITHDRAWAL_QUEUEDWITHDRAWAL_OFFSET = 0,
    LR_COMPLETE_QUEUED_WITHDRAWAL_TOKENS_OFFSET,
    LR_COMPLETE_QUEUED_WITHDRAWAL_MIDDLEWARETIMEINDEX,
    LR_COMPLETE_QUEUED_WITHDRAWAL_RECEIVEASTOKENS,
    LR_COMPLETE_QUEUED_WITHDRAWAL_UNEXPECTED_PARAMETER
} lr_complete_queued_withdrawal_parameters;

#define LR_STRATEGIES_COUNT 11

extern const char lr_strategy_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN];
extern const char lr_erc20_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN];
extern const char lr_tickers[LR_STRATEGIES_COUNT][MAX_TICKER_LEN];

// max number of strategies / erc20 to display
#define MAX_DISPLAY_COUNT 3

typedef struct {
    int strategy_to_display;
    int erc20_to_display;
    uint8_t erc20_amount_to_display[INT256_LENGTH];
} lr_deposit_t;

typedef struct {
    char withdrawer[ADDRESS_STR_LEN];
} lr_queue_withdrawal_t;

typedef struct {
} lr_complete_queued_withdrawal_t;

typedef struct context_t {
    uint8_t next_param;

    union {
        lr_complete_queued_withdrawal_t lr_complete_queued_withdrawal;
        lr_deposit_t lr_deposit;
        lr_queue_withdrawal_t lr_queue_withdrawal;
    } param_data;

    selector_t selectorIndex;
} context_t;

_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");
