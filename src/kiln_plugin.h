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

#pragma once

#include <string.h>

#include "eth_plugin_interface.h"
#include "cx.h"
#include <ctype.h>

#define PLUGIN_NAME "Kiln"

// ADDRESS_STR_LEN is 0x + addr + \0
#define ADDRESS_STR_LEN 43

// ****************************************************************************
// * SUPPORTED SELECTORS
// ****************************************************************************

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
// --- 9. requestExit(shares_amount)
// --- 10. multiClaim(exit_queues, ticket_ids, cask_ids)
// --- 11. claim(uint256[],uint32[],uint16)
//
// LR selectors
// --- 12. depositIntoStrategy(address,address,uint256)
// --- 13. queueWithdrawals((address[],uint256[],address)[])
// --- 14.
// completeQueuedWithdrawal((address,address,address,uint256,uint32,address[],uint256[]),address[],uint256,bool)
// --- 15. delegateTo(address,(bytes,uint256),bytes32)
// --- 16. undelegate(address)
//
// DEFI
// --- 17. deposit(uint256,address)
// --- 18. mint(uint256,address)
// --- 19. withdraw(uint256,address,address)
// --- 20. redeem(uint256,address,address)
// --- 21. approve(address,uint256)
// --- 22. transfer(address,uint256)
// --- 23. transferFrom(address,address,uint256)
//
#define NUM_SELECTORS 24
extern const uint32_t KILN_SELECTORS[NUM_SELECTORS];

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
    KILN_LR_QUEUE_WITHDRAWALS,
    KILN_LR_COMPLETE_QUEUED_WITHDRAWALS,
    KILN_LR_DELEGATE_TO,
    KILN_LR_UNDELEGATE,
    KILN_DEFI_DEPOSIT,
    KILN_DEFI_MINT,
    KILN_DEFI_WITHDRAW,
    KILN_DEFI_REDEEM,
    KILN_DEFI_APPROVE,
    KILN_DEFI_TRANSFER,
    KILN_DEFI_TRANSFER_FROM,
} selector_t;

// ****************************************************************************
// * EIGENLAYER
// ****************************************************************************

// globals

#define LR_STRATEGIES_COUNT                 11
#define UNKNOWN_LR_ERC20                    15  // must be > LR_STRATEGIES_COUNT < 16
#define UNKNOWN_LR_STRATEGY                 15  // must be > LR_STRATEGIES_COUNT < 16
#define MAX_DISPLAYABLE_LR_STRATEGIES_COUNT 32  // must be > LR_STRATEGIES_COUNT
#define ERC20_DECIMALS                      18
#define PARAM_OFFSET                        32
#define OCV2_MAX_EXIT_QUEUES                2
#define DEFI_VAULTS_COUNT                   10
#define UNKNOWN_DEFI_VAULT                  100  // must be > DEFI_VAULTS_COUNT < 4

extern const char ocv2_exit_queues[OCV2_MAX_EXIT_QUEUES][ADDRESS_STR_LEN];
extern const char lr_strategy_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN];
extern const char lr_erc20_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN];
extern const char lr_tickers[LR_STRATEGIES_COUNT][MAX_TICKER_LEN];
extern const char lr_kiln_operator_address[ADDRESS_STR_LEN];
extern const char defi_vaults_addresses[DEFI_VAULTS_COUNT][ADDRESS_STR_LEN];
extern const char defi_vaults_names[DEFI_VAULTS_COUNT][ADDRESS_STR_LEN];
extern const char defi_shares_names[DEFI_VAULTS_COUNT][MAX_TICKER_LEN];
extern const uint8_t defi_shares_decimals[DEFI_VAULTS_COUNT];
extern const char defi_assets_names[DEFI_VAULTS_COUNT][MAX_TICKER_LEN];
extern const uint8_t defi_assets_decimals[DEFI_VAULTS_COUNT];

// ****************************************************************************

// Parameters and state machines for OCV1 parsing

typedef enum {
    V1_WFUNCS_UNEXPECTED_PARAMETER = 0,
    V1_WFUNCS_BYTES_OFFSET,
    V1_WFUNCS_BYTES_LENGTH,
    V1_WFUNCS_BYTES__ITEMS,
} v1_withdraw_funcs_parameters;

// Parameters and state machines for OCV2 parsing

typedef enum {
    V2_REQUEST_EXIT_UNEXPECTED_PARAMETER = 0,
    V2_REQUEST_EXIT_AMOUNT,
} v2_request_exit_parameters;

typedef enum {
    V2_CLAIM_UNEXPECTED_PARAMETER = 0,
    V2_CLAIM_TICKET_IDS_OFFSET,
    V2_CLAIM_CASK_IDS_OFFSET,
    V2_CLAIM_MAX_CLAIM_DEPTH,
    V2_CLAIM_TICKET_IDS_LENGTH,
    V2_CLAIM_TICKET_IDS__ITEMS,
    V2_CLAIM_CASK_IDS_LENGTH,
    V2_CLAIM_CASK_IDS__ITEMS,
} v2_claim;

typedef enum {
    V2_MULTICLAIM_UNEXPECTED_PARAMETER = 0,
    V2_MULTICLAIM_EXIT_QUEUES_OFFSET,
    V2_MULTICLAIM_TICKET_IDS_OFFSET,
    V2_MULTICLAIM_CASK_IDS_OFFSET,

    V2_MULTICLAIM_EXIT_QUEUES_LENGTH,
    V2_MULTICLAIM_EXIT_QUEUES__ITEMS,

    V2_MULTICLAIM_TICKETIDS_LENGTH,
    V2_MULTICLAIM_TICKETIDS__OFFSET_ITEMS,
    V2_MULTICLAIM_TICKETIDS__ITEM_LENGTH,
    V2_MULTICLAIM_TICKETIDS__ITEM__ITEMS,

    V2_MULTICLAIM_CASKIDS_LENGTH,
    V2_MULTICLAIM_CASKIDS__OFFSET_ITEMS,
    V2_MULTICLAIM_CASKIDS__ITEM_LENGTH,
    V2_MULTICLAIM_CASKIDS__ITEM__ITEMS,
} v2_multiclaim_parameters;

// Parameters and state machines for EigenLayer parsing

typedef enum {
    LR_UNDELEGATE_UNEXPECTED_PARAMETER = 0,
    LR_UNDELEGATE_ADDRESS,
} lr_undelegate_parameters;

typedef enum {
    LR_DEPOSIT_INTO_STRATEGY_UNEXPECTED_PARAMETER = 0,
    LR_DEPOSIT_INTO_STRATEGY_STRATEGY,
    LR_DEPOSIT_INTO_STRATEGY_TOKEN,
    LR_DEPOSIT_INTO_STRATEGY_AMOUNT,
} lr_deposit_into_strategy_parameters;

typedef enum {
    LR_QUEUE_WITHDRAWALS_UNEXPECTED_PARAMETER = 0,
    LR_QUEUE_WITHDRAWALS_QWITHDRAWALS_OFFSET,
    LR_QUEUE_WITHDRAWALS_QWITHDRAWALS_LENGTH,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRUCT_OFFSET,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_OFFSET,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_OFFSET,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_WITHDRAWER,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_STRATEGIES_LENGTH,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__STRATEGIES_ITEM,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS_SHARES_LENGTH,
    LR_QUEUE_WITHDRAWALS__QWITHDRAWALS__SHARES_ITEM,
} lr_queue_withdrawals_parameters;

typedef enum {
    LRCQW_UNEXPECTED_PARAMETER = 0,
    LRCQW_WITHDRAWALS_OFFSET,
    LRCQW_TOKENS_OFFSET,
    LRCQW_MIDDLEWARE_TIMES_INDEXES_OFFSET,
    LRCQW_RECEIVE_AS_TOKENS_OFFSET,

    LRCQW_WITHDRAWALS_LENGTH,
    LRCQW_WITHDRAWALS__OFFSET_ITEMS,

    LRCQW_WITHDRAWALS__ITEM__STAKER,
    LRCQW_WITHDRAWALS__ITEM__DELEGATED_TO,
    LRCQW_WITHDRAWALS__ITEM__WITHDRAWER,
    LRCQW_WITHDRAWALS__ITEM__NONCE,
    LRCQW_WITHDRAWALS__ITEM__START_BLOCK,
    LRCQW_WITHDRAWALS__ITEM__STRATEGIES_OFFSET,
    LRCQW_WITHDRAWALS__ITEM__SHARES_OFFSET,
    LRCQW_WITHDRAWALS__ITEM__STRATEGIES_LENGTH,
    LRCQW_WITHDRAWALS__ITEM__STRATEGIES__ITEMS,
    LRCQW_WITHDRAWALS__ITEM__SHARES_LENGTH,
    LRCQW_WITHDRAWALS__ITEM__SHARES__ITEMS,

    LRCQW_TOKENS_LENGTH,
    LRCQW_TOKENS__OFFSET_ITEMS,

    LRCQW_TOKENS__ITEM__LENGTH,
    LRCQW_TOKENS__ITEM__ITEMS,

    LRCQW_MIDDLEWARE_TIMES_INDEXES_LENGTH,
    LRCQW_MIDDLEWARE_TIMES_INDEXES__ITEMS,

    LRCQW_RECEIVE_AS_TOKENS_LENGTH,
    LRCQW_RECEIVE_AS_TOKENS__ITEMS,
} lr_complete_queued_withdrawals_parameters;

typedef enum {
    LR_DELEGATE_TO_UNEXPECTED_PARAMETER = 0,
    LR_DELEGATE_TO_OPERATOR,
    LR_DELEGATE_TO_SIGNATURE_OFFSET,
    LR_DELEGATE_TO_APPROVER_SALT,
    LR_DELEGATE_TO_SIGNATURE_SIG_OFFSET,
    LR_DELEGATE_TO_SIGNATURE_EXPIRY,
    LR_DELEGATE_TO_SIGNATURE_SIG_LENGTH,
    LR_DELEGATE_TO_SIGNATURE_SIG_ITEMS,
} lr_delegate_to_parameters;

// ****************************************************************************

// Parsing structures

typedef struct {
    uint16_t current_item_count;
} v1_withdraw_funcs_t;

typedef struct {
    uint8_t amount[INT256_LENGTH];
} v2_request_exit_t;

typedef struct {
    // -- utils
    uint16_t cask_ids_offset;

    uint16_t current_item_count;
} v2_claim_t;

typedef struct {
    // -- utils
    uint16_t ticket_ids_offset;
    uint16_t cask_ids_offset;
    uint8_t checksum_preview[CX_KECCAK_256_SIZE];
    uint8_t checksum_value[CX_KECCAK_256_SIZE];
    uint32_t cached_offset;

    uint16_t parent_item_count;
    uint16_t current_item_count;
} v2_multiclaim_t;

typedef struct {
    int strategy_to_display;
    int erc20_to_display;
    uint8_t erc20_amount_to_display[INT256_LENGTH];
} lr_deposit_t;

typedef struct {
    // -- utils
    uint16_t current_item_count;
    // -- display
    char operator_address[ADDRESS_STR_LEN];
    bool is_kiln;
} lr_delegate_to_t;

typedef struct {
    uint8_t prev_checksum[CX_KECCAK_256_SIZE];
    uint32_t new_offset;
} checksum_offset_params_t;

typedef struct {
    //  -- utils
    uint16_t queued_withdrawals_count;
    uint16_t current_item_count;
    uint16_t shares_array_offset;
    uint8_t qwithdrawals_offsets_checksum_preview[CX_KECCAK_256_SIZE];
    uint8_t qwithdrawals_offsets_checksum_value[CX_KECCAK_256_SIZE];

    // -- display
    char withdrawer[ADDRESS_STR_LEN];
    uint8_t strategies_count;
    // list of strategies indexes **INCREMENTED BY 1** to display in the UI
    // 0 is reserved for end of array
    // UNKNOWN_LR_STRATEGY is used to display the "unknown" strategy
    // assumptions:
    // (i) in practice, we should not encounter more than
    //      LR_STRATEGIES_COUNT +~ a few potential unsupported
    //      strategies in the plugin. So * 3 should be a good enough buffer.
    // (ii) in practice there should not be more than (2 ** 8) - 2 known strategies
    uint8_t strategies[MAX_DISPLAYABLE_LR_STRATEGIES_COUNT];
} lr_queue_withdrawals_t;

// must be size of MAX_DISPLAYABLE_LR_STRATEGIES_COUNT
typedef struct {
    uint32_t bits;
} bitfield;

typedef struct {
    // -- utils
    uint16_t parent_item_count;
    uint16_t current_item_count;
    // -- total values
    uint16_t relegations_count;
    uint8_t withdrawals_count;
    uint16_t strategies_count;
    uint16_t tokens_count;
    // -- offset verification
    uint16_t tokens_offset;
    uint16_t middlewareTimesIndexes_offset;
    uint16_t receiveAsTokens_offset;
    uint16_t cached_offset;
    uint8_t withdrawals_offsets_checksum_preview[CX_KECCAK_256_SIZE];
    uint8_t withdrawals_offsets_checksum_value[CX_KECCAK_256_SIZE];

    // -- display
    uint8_t withdrawer[ADDRESS_LENGTH];
    // list of strategies indexes **INCREMENTED BY 1** to display in the UI
    // UNKNOWN_LR_STRATEGY is used to display the "unknown" strategy
    // assumptions:
    // (i) in practice, we should not encounter more than
    //      LR_STRATEGIES_COUNT +~ a few potential unsupported
    //      strategies in the plugin. So * 3 should be a good enough buffer.
    // (ii) in practice there should not be more than (2 ** 4) - 2 known strategies
    // (iii) the first 4 bytes are the withdrawal index, the next 4 bytes are the strategy index
    uint8_t strategies[MAX_DISPLAYABLE_LR_STRATEGIES_COUNT];
    // follows the strategies array indexes
    bitfield is_redelegated;
} lr_complete_queued_withdrawals_t;

// ****************************************************************************
// * DEFI
// ****************************************************************************

typedef enum {
    DEFI_DEPOSIT_UNEXPECTED_PARAMETER = 0,
    DEFI_DEPOSIT_ASSETS_AMOUNT,
    DEFI_DEPOSIT_RECEIVER_ADDRESS,
} defi_deposit_parameters;

typedef enum {
    DEFI_MINT_UNEXPECTED_PARAMETER = 0,
    DEFI_MINT_SHARES_AMOUNT,
    DEFI_MINT_RECEIVER_ADDRESS,
} defi_mint_parameters;

typedef enum {
    DEFI_WITHDRAW_UNEXPECTED_PARAMETER = 0,
    DEFI_WITHDRAW_ASSETS_AMOUNT,
    DEFI_WITHDRAW_RECEIVER_ADDRESS,
    DEFI_WITHDRAW_OWNER_ADDRESS,
} defi_withdraw_parameters;

typedef enum {
    DEFI_REDEEM_UNEXPECTED_PARAMETER = 0,
    DEFI_REDEEM_SHARES_AMOUNT,
    DEFI_REDEEM_RECEIVER_ADDRESS,
    DEFI_REDEEM_OWNER_ADDRESS,
} defi_redeem_parameters;

typedef enum {
    DEFI_APPROVE_UNEXPECTED_PARAMETER = 0,
    DEFI_APPROVE_SPENDER,
    DEFI_APPROVE_AMOUNT,
} defi_approve_parameters;

typedef enum {
    DEFI_TRANSFER_UNEXPECTED_PARAMETER = 0,
    DEFI_TRANSFER_TO,
    DEFI_TRANSFER_AMOUNT,
} defi_transfer_parameters;

typedef enum {
    DEFI_TRANSFER_FROM_UNEXPECTED_PARAMETER = 0,
    DEFI_TRANSFER_FROM_FROM,
    DEFI_TRANSFER_FROM_TO,
    DEFI_TRANSFER_FROM_AMOUNT,
} defi_transfer_from_parameters;

// ****************************************************************************

typedef struct {
    uint8_t assets_amount[INT256_LENGTH];
    char receiver_address[ADDRESS_STR_LEN];
    uint8_t vault_index;
} defi_deposit_t;

typedef struct {
    uint8_t shares_amount[INT256_LENGTH];
    char receiver_address[ADDRESS_STR_LEN];
    uint8_t vault_index;
} defi_mint_t;

typedef struct {
    uint8_t assets_amount[INT256_LENGTH];
    char receiver_address[ADDRESS_STR_LEN];
    char owner_address[ADDRESS_STR_LEN];
    uint8_t vault_index;
} defi_withdraw_t;

typedef struct {
    uint8_t shares_amount[INT256_LENGTH];
    char receiver_address[ADDRESS_STR_LEN];
    char owner_address[ADDRESS_STR_LEN];
    uint8_t vault_index;
} defi_redeem_t;

typedef struct {
    char spender[ADDRESS_STR_LEN];
    uint8_t amount[INT256_LENGTH];
    uint8_t vault_index;
} defi_approve_t;

typedef struct {
    char to[ADDRESS_STR_LEN];
    uint8_t amount[INT256_LENGTH];
    uint8_t vault_index;
} defi_transfer_t;

typedef struct {
    char from[ADDRESS_STR_LEN];
    char to[ADDRESS_STR_LEN];
    uint8_t amount[INT256_LENGTH];
    uint8_t vault_index;
} defi_transfer_from_t;

// ****************************************************************************
// * SHARED PLUGIN CONTEXT MEMORY
// ****************************************************************************

typedef struct context_t {
    uint8_t next_param;

    union {
        v1_withdraw_funcs_t v1_withdraw_funcs;

        v2_request_exit_t v2_request_exit;
        v2_claim_t v2_claim;
        v2_multiclaim_t v2_multiclaim;

        lr_deposit_t lr_deposit;
        lr_delegate_to_t lr_delegate_to;
        lr_queue_withdrawals_t lr_queue_withdrawals;
        lr_complete_queued_withdrawals_t lr_complete_queued_withdrawals;

        defi_deposit_t defi_deposit;
        defi_mint_t defi_mint;
        defi_withdraw_t defi_withdraw;
        defi_redeem_t defi_redeem;
        defi_approve_t defi_approve;
        defi_transfer_t defi_transfer;
        defi_transfer_from_t defi_transfer_from;
    } param_data;

    selector_t selectorIndex;
} context_t;

_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");
