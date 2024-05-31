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

#pragma once

#include <ctype.h>
#include <stdbool.h>

#include "kiln_plugin.h"

// On-Chain v1 UI
bool stake_ui(ethQueryContractUI_t *msg);
bool withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context);
bool withdraw_ui(ethQueryContractUI_t *msg, context_t *context);
bool batch_withdraw_rewards_ui(ethQueryContractUI_t *msg, context_t *context);
bool batch_withdraw_ui(ethQueryContractUI_t *msg, context_t *context);

// On-Chain v2 UI
bool stake_in_pool_ui(ethQueryContractUI_t *msg);
bool request_pooling_exit_ui(ethQueryContractUI_t *msg);
bool request_exit_ui(ethQueryContractUI_t *msg);
bool multiclaim_ui(ethQueryContractUI_t *msg);
bool claim_ui(ethQueryContractUI_t *msg);

// EigenLayer UI
bool deposit_into_strategy_ui(ethQueryContractUI_t *msg, context_t *context);
bool queue_withdrawals_ui(ethQueryContractUI_t *msg, context_t *context);
bool complete_queued_withdrawals_ui(ethQueryContractUI_t *msg, context_t *context);
bool delegate_to_ui(ethQueryContractUI_t *msg, context_t *context);
bool undelegate_ui(ethQueryContractUI_t *msg);