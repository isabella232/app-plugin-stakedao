#include "stakedao_plugin.h"

static void copy_amount(uint8_t *dst, size_t dst_len, uint8_t *src) {
    size_t len = MIN(dst_len, PARAMETER_LENGTH);
    memcpy(dst, src, len);
}

static void copy_pid(uint8_t *dst, size_t dst_len, uint8_t *src) {
    size_t offset = PARAMETER_LENGTH - MAX_STRATEGY_TICKER_LEN;
    size_t len = MIN(dst_len, ADDRESS_LENGTH);
    memcpy(dst, &src[offset], len);
}

static void copy_nft_id(uint8_t *dst, size_t dst_len, uint8_t *src) {
    size_t offset = PARAMETER_LENGTH - MAX_STRATEGY_TICKER_LEN;
    size_t len = MIN(dst_len, ADDRESS_LENGTH);
    memcpy(dst, &src[offset], len);
}

static void handle_no_params(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        // no params
        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_vault(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_amount(context->amount, sizeof(context->amount), msg->parameter);
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_opt_min_amount(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_amount(context->amount, sizeof(context->amount), msg->parameter);
            context->next_param = MIN_AMOUNT;
            break;
        case MIN_AMOUNT:
            copy_amount(context->min_amount, sizeof(context->min_amount), msg->parameter);
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_premium_stake(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        case AMOUNT:
            copy_amount(context->amount, sizeof(context->amount), msg->parameter);
            context->next_param = NFT_ID;
            break;
        case NFT_ID:
            copy_nft_id(context->pid, sizeof(context->pid), msg->parameter);
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_lp(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        case PID:
            copy_pid(context->pid, sizeof(context->pid), msg->parameter);
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_amount(context->amount, sizeof(context->amount), msg->parameter);
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_rewards_claim(ethPluginProvideParameter_t *msg, stakedao_parameters_t *context) {
    switch (context->next_param) {
        case MERKLE_INDEX:
            context->next_param = INDEX;
            break;
        case INDEX:
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_amount(context->amount, sizeof(context->amount), msg->parameter);
            context->next_param = MERKLE_PROOF;
            break;
        case MERKLE_PROOF:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    stakedao_parameters_t *context = (stakedao_parameters_t *) msg->pluginContext;
    PRINTF("plugin provide parameter %d %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    switch (context->selectorIndex) {
        case VAULT_DEPOSIT_ALL:
        case PREMIUM_GETREWARD:
        case PREMIUM_EXIT:
            handle_no_params(msg, context);
            break;
        case LP_DEPOSIT:
        case LP_WITHDRAW:
            handle_lp(msg, context);
            break;
        case OPT_WITHDRAW_ETH:
        case OPT_DEPOSIT_UNDERLYING:
        case OPT_WITHDRAW_UNDERLYING:
            handle_opt_min_amount(msg, context);
            break;
        case PREMIUM_STAKE:
            handle_premium_stake(msg, context);
            break;
        case VAULT_DEPOSIT:
        case VAULT_WITHDRAW:
        case OPT_DEPOSIT_ETH:
        case OPT_DEPOSIT_CRVLP:
        case OPT_WITHDRAW_CRVLP:
        case PREMIUM_WITHDRAW:
        case SANCTUARY_ENTER:
        case SANCTUARY_LEAVE:
        case PALACE_STAKE:
        case PALACE_WITHDRAW:
            handle_vault(msg, context);
            break;
        case REWARDS_CLAIM:
            handle_rewards_claim(msg, context);
            break;
        default:
            PRINTF("Selector Index %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}