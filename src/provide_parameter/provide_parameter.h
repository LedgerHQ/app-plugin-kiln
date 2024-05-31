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

void handle_lr_deposit_into_strategy(ethPluginProvideParameter_t *msg, context_t *context);
void handle_lr_queue_withdrawals(ethPluginProvideParameter_t *msg, context_t *context);
void handle_lr_complete_queued_withdrawals(ethPluginProvideParameter_t *msg, context_t *context);
void handle_lr_delegate_to(ethPluginProvideParameter_t *msg, context_t *context);
