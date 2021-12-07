#include "stakedao_plugin.h"

// case
// VAULT_DEPOSIT -> strategy + token + amount (crvlp)
// VAULT_WITHDRAW -> strategy + token + amount (sd share)
// VAULT_DEPOSIT_ALL -> strategy + token + amount (ALL)
// OPT_DEPOSIT_ETH -> strategy + token + amount (ETH)
// OPT_WITHDRAW_ETH -> strategy + token + amount (sd share)
// OPT_DEPOSIT_UNDERLYING -> strategy + token + amount (underlying)
// OPT_DEPOSIT_CRVLP -> strategy + token + amount (crvLP)
// OPT_WITHDRAW_UNDERLYING -> strategy + token + amount (sd share)
// OPT_WITHDRAW_CRVLP -> strategy + token + amount (sd share)
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
            msg->numScreens += 2;
            break;
    }
    msg->uiType = ETH_UI_TYPE_GENERIC;
    msg->result = ETH_PLUGIN_RESULT_OK;
}
