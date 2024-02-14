#include "kiln_plugin.h"
#include "plugin_utils.h"

void handle_init_contract(ethPluginInitContract_t *msg) {
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    memset(context, 0, sizeof(*context));

    size_t index;
    if (!find_selector(U4BE(msg->selector, 0), KILN_SELECTORS, NUM_SELECTORS, &index)) {
        PRINTF("Error: selector not found!\n");
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }
    context->selectorIndex = index;
    // check for overflow
    if ((size_t) context->selectorIndex != index) {
        PRINTF("Error: overflow detected on selector index!\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case KILN_V1_DEPOSIT:
            break;

        case KILN_V1_WITHDRAW:
        case KILN_V1_WITHDRAW_EL:
        case KILN_V1_WITHDRAW_CL:
            break;

        case KILN_V1_BATCH_WITHDRAW:
        case KILN_V1_BATCH_WITHDRAW_EL:
        case KILN_V1_BATCH_WITHDRAW_CL:
            break;

        case KILN_V1_REQUEST_EXIT:
            break;

        case KILN_V2_STAKE:
        case KILN_V2_REQUEST_EXIT:
        case KILN_V2_MULTICLAIM:
        case KILN_V2_CLAIM:
            break;

        case KILN_LR_DEPOSIT_INTO_STRATEGY:
            context->next_param = LR_DEPOSIT_INTO_STRATEGY_STRATEGY;
            break;
        case KILN_LR_QUEUE_WITHDRAWAL:
            context->next_param = LR_QUEUE_WITHDRAWAL_STRATEGY_INDEXES_OFFSET;
            break;
        case KILN_LR_COMPLETE_QUEUED_WITHDRAWAL:
            context->next_param = LR_COMPLETE_QUEUED_WITHDRAWAL_QUEUEDWITHDRAWAL_OFFSET;
            break;

        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
