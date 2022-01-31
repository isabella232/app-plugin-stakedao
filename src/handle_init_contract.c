#include "stakedao_plugin.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(stakedao_parameters_t)) {
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    stakedao_parameters_t *context = (stakedao_parameters_t *) msg->pluginContext;
    memset(context, 0, sizeof(*context));

    uint8_t i;
    for (i = 0; i < NUM_STAKEDAO_SELECTORS; i++) {
        if (memcmp(PIC(STAKEDAO_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            if (i == 17) {
                ethPluginSharedRO_t *pluginSharedRO = (ethPluginSharedRO_t *) msg->pluginSharedRO;
                if(memcmp(pluginSharedRO->txContent->destination, STAKEDAO_NFT_BOOST, ADDRESS_LENGTH) == 0) {
                    context->selectorIndex = 20;
                    break;
                }
            }
            context->selectorIndex = i;
            break;
        }
    }
    if (i == NUM_STAKEDAO_SELECTORS) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case VAULT_DEPOSIT_ALL:
        case OPT_DEPOSIT_ETH:
        case PREMIUM_EXIT:
        case PREMIUM_GETREWARD:
        case NFT_UNSTAKE:
            break;
        case VAULT_DEPOSIT:
        case VAULT_WITHDRAW:
        case OPT_WITHDRAW_ETH:
        case OPT_DEPOSIT_UNDERLYING:
        case OPT_DEPOSIT_CRVLP:
        case OPT_WITHDRAW_UNDERLYING:
        case OPT_WITHDRAW_CRVLP:
        case PREMIUM_STAKE:
        case PREMIUM_WITHDRAW:
        case SANCTUARY_ENTER:
        case SANCTUARY_LEAVE:
        case PALACE_STAKE:
        case PALACE_WITHDRAW:
            context->next_param = AMOUNT;
            break;
        case LP_DEPOSIT:
        case LP_WITHDRAW:
            context->next_param = PID;
            context->decimals = 18;
            break;
        case REWARDS_CLAIM:
            context->next_param = MERKLE_INDEX;
            break;
        case NFT_STAKE:
            context->next_param = NFT_ID;
            break;
        case ANGLE_GET_REWARD:
            context->next_param = USER;
            break;
        default:
            PRINTF("Missing selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
