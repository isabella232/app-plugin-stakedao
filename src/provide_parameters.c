#include "stakedao_plugin.h"

void printf_hex_array(const char *title __attribute__((unused)),
                      int len __attribute__((unused)),
                      const uint8_t *data __attribute__((unused))) {
    PRINTF(title);
    for (int i = 0; i < len; ++i) {
        PRINTF("%02x", data[i]);
    };
    PRINTF("\n");
}

// Store the amount deposited in the form of a string, without decimals. These will be
// added when displaying.
static void handle_amount_deposited(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    memset(context->amount_deposited, 0, sizeof(context->amount_deposited));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,
                   "",
                   (char *) context->amount_deposited,
                   sizeof(context->amount_deposited));
    PRINTF("AMOUNT DEPOSITED: %s\n", context->amount_deposited);
}

// Store the shares received in the form of a string, without decimals. These will be
// added when displaying.
static void handle_shares_received(ethPluginProvideParameter_t *msg,
                                   stake_dao_parameters_t *context) {
    memset(context->shares_received, 0, sizeof(context->shares_received));

    // Convert to string.
    amountToString(msg->parameter,
                   PARAMETER_LENGTH,
                   0,   // No decimals
                   "",  // No ticker
                   (char *) context->shares_received,
                   sizeof(context->shares_received));
    PRINTF("SHARES RECEIVED: %s\n", context->shares_received);
}

static void handle_beneficiary(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    memset(context->beneficiary, 0, sizeof(context->beneficiary));
    memcpy(context->beneficiary,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->beneficiary));
    printf_hex_array("BENEFICIARY: ", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_token_deposited(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    memset(context->contract_address_underlying, 0, sizeof(context->contract_address_underlying));
    memcpy(context->contract_address_underlying,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           ADDRESS_LENGTH);
    printf_hex_array("TOKEN DEPOSITED: ", ADDRESS_LENGTH, context->contract_address_underlying);
}

static void handle_shares_burned(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    memset(context->contract_address_underlying, 0, sizeof(context->contract_address_underlying));
    memcpy(context->contract_address_underlying,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           ADDRESS_LENGTH);
    printf_hex_array("TOKEN DEPOSITED: ", ADDRESS_LENGTH, context->contract_address_underlying);
}

static void handle_token_shares(ethPluginProvideParameter_t *msg,
                                  stake_dao_parameters_t *context) {
    memset(context->contract_address_shares, 0, sizeof(context->contract_address_shares));
    memcpy(context->contract_address_shares,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           ADDRESS_LENGTH);
    printf_hex_array("SHARES RECEIVED: ", ADDRESS_LENGTH, context->contract_address_shares);
}

static void handle_flags(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    context->flags = msg->parameter[PARAMETER_LENGTH - 1];
}

static void handle_deposit(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_UNDER:
            handle_token_deposited(msg, context);
            context->next_param = TOKEN_SHARES;
            break;
        case TOKEN_SHARES:
            handle_token_shares(msg, context);
            context->next_param = AMOUNT_DEPOSITED;
            break;
        case AMOUNT_DEPOSITED: 
            handle_amount_deposited(msg, context);
            context->next_param = SHARES_RECEIVED;
            break;
        case SHARES_RECEIVED:
            handle_shares_received(msg, context);
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

static void handle_withdraw(ethPluginProvideParameter_t *msg, stake_dao_parameters_t *context) {
    switch (context->next_param) {
        case TOKEN_UNDER:
            handle_token_deposited(msg, context);
            context->next_param = TOKEN_SHARES;
            break;
        case TOKEN_SHARES:
            handle_token_shares(msg, context);
            context->next_param = SHARES_BURNED;
            break;
        case SHARES_BURNED:
            handle_shares_burned(msg, context);
            context->next_param = AMOUNT_WITHDRAWN;
            break;
        case AMOUNT_WITHDRAWN:
            handle_shares_burned(msg, context);
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;
    printf_hex_array("stakeDAO plugin provide parameter: ", PARAMETER_LENGTH, msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (context->skip) {
        // Skip this step, and don't forget to decrease skipping counter.
        context->skip--;
    } else {
        if ((context->offset) && msg->parameterOffset != context->checkpoint + context->offset) {
            PRINTF("offset: %d, checkpoint: %d, parameterOffset: %d\n",
                   context->offset,
                   context->checkpoint,
                   msg->parameterOffset);
            return;
        }

        context->offset = 0;  // Reset offset
        switch (context->selectorIndex) {
            case DEPOSIT: {
                handle_deposit(msg, context);
                break;
            }

            case WITHDRAW: {
                handle_withdraw(msg, context);
                break;
            }

            default:
                PRINTF("Selector Index %d not supported\n", context->selectorIndex);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
    }
}