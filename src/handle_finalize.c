#include "stakedao_plugin.h"

// case
// VAULT_DEPOSIT -> strategy + token + amount (crvlp) + vault addr
// VAULT_WITHDRAW -> strategy + token + amount (sd share) + vault addr
// VAULT_DEPOSIT_ALL -> strategy + token + amount (ALL) + vault addr
// OPT_DEPOSIT_ETH -> strategy + token + amount (ETH) + vault addr
// OPT_WITHDRAW_ETH -> strategy + token + amount (sd opt share) + vault addr
// OPT_DEPOSIT_UNDERLYING -> strategy + token + amount (underlying) + vault addr
// OPT_DEPOSIT_CRVLP -> strategy + token + amount (crvLP) + vault addr
// OPT_WITHDRAW_UNDERLYING -> strategy + token + amount (sd share) + vault addr
// OPT_WITHDRAW_CRVLP -> strategy + token + amount (sd share) + vault addr
// PREMIUM_STAKE -> strategy + token + amount (SDT) + nft id 
// PREMIUM_WITHDRAW -> strategy + token + amount (SDT)
// PREMIUM_GETREWARD -> strategy + token (WAVAX)
// PREMIUM_EXIT -> strategy
// LP_DEPOSIT -> strategy + pid/token + amount
// LP_WITHDRAW -> strategy + pid/token + amount
// SANCTUARY_ENTER -> strategy + token + amount (SDT)
// SANCTUARY_LEAVE -> strategy + token + amount (xSDT)
// PALACE_STAKE -> strategy + token + amount (xSDT)
// PALACE_WITHDRAW -> strategy + token + amount (xSDT)

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    stakedao_parameters_t *context = (stakedao_parameters_t *) msg->pluginContext;
    msg->numScreens = 1; // at least 1 for the premium exit

    switch (context->selectorIndex) {
        case PREMIUM_EXIT:
            break;
        case PREMIUM_GETREWARD:
            msg->numScreens += 1;
            break;
        default:
            msg->numScreens += 3;
            break;
    }
    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}
