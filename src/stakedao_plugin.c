#include "stakedao_plugin.h"
#include <string.h>

// Need more information about the interface for plugins? Please read the README.md!

// You can check stakeDAO deposit method here
//
// You can check stakeDAO withdraw method here
// deposit 0x7c025200
static const uint8_t STAKE_DAO_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0x7c, 0x02, 0x52, 0x00};
// withdraw 0x2e95b6c8
static const uint8_t STAKE_DAO_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x95, 0xb6, 0xc8};

// Array of all the different stakeDAO selectors.
const uint8_t *const STAKE_DAO_SELECTORS[NUM_STAKE_DAO_SELECTORS] = {
    STAKE_DAO_DEPOSIT_SELECTOR,
    STAKE_DAO_WITHDRAW_SELECTOR,
};

// Used to indicate that the beneficiary should be the sender.
const uint8_t NULL_ETH_ADDRESS[ADDRESS_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Called once to init.
static void handle_init_contract(void *parameters) {
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_1) {
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(stake_dao_parameters_t)) {
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;
    memset(context, 0, sizeof(*context));
    context->valid = 1;

    // Determine a function to call
    size_t i;
    for (i = 0; i < NUM_STAKE_DAO_SELECTORS; i++) {
        if (memcmp((uint8_t *) PIC(STAKE_DAO_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            context->selectorIndex = i;
            break;
        }
    }

    if (i == NUM_STAKE_DAO_SELECTORS) {
        // Selector was not found
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case DEPOSIT:
            // Skip caller, structure offset and data offset
            context->skip = 3;
            context->next_param = TOKEN_UNDER;
            break;
        case WITHDRAW:
            context->next_param = TOKEN_UNDER;
            break;
        default:
            PRINTF("Missing selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}

/*static void sent_token_eth(stake_dao_parameters_t *context) {
    context->decimals_sent = WEI_TO_ETHER;
    strlcpy(context->ticker_sent, "ETH", sizeof(context->ticker_sent));
    context->tokens_found |= TOKEN_SENT_FOUND;
}*/

/*static void received_token_eth(stake_dao_parameters_t *context) {
    context->decimals_received = WEI_TO_ETHER;
    strlcpy(context->ticker_shares, "ETH", sizeof(context->ticker_received));
    context->tokens_found |= TOKEN_RECEIVED_FOUND;
}*/

static void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;
    if (context->valid) {
        msg->numScreens = 1;
        if (context->selectorIndex == DEPOSIT) {
            // An addiitonal screen is required to display the receive and beneficiary field.
            msg->numScreens += 2;
            //if (context->flags & PARTIAL_FILL) msg->numScreens += 1;
        }

        // Address is not ETH so we will need to look up the token in the CAL.
        printf_hex_array("Setting address sent to: ",
                        ADDRESS_LENGTH,
                        context->contract_address_underlying);
        msg->tokenLookup1 = context->contract_address_underlying;

        // Address is not ETH so we will need to look up the token in the CAL.
        printf_hex_array("Setting address received to: ",
                        ADDRESS_LENGTH,
                        context->contract_address_shares);
        msg->tokenLookup2 = context->contract_address_shares;

        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Context not valid\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}

static void handle_provide_token(void *parameters) {
    ethPluginProvideToken_t *msg = (ethPluginProvideToken_t *) parameters;
    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;
    PRINTF("StakeDAO plugin provide token: 0x%p, 0x%p\n", msg->token1, msg->token2);

    if (msg->token1 != NULL) {
        context->decimals_sent = msg->token1->decimals;
        strlcpy(context->ticker_underlying, (char *) msg->token1->ticker, sizeof(context->ticker_underlying));
        context->tokens_found |= TOKEN_SENT_FOUND;
    } else {
        // CAL did not find the token and token is not ETH.
        context->decimals_sent = DEFAULT_DECIMAL;
        strlcpy(context->ticker_underlying, DEFAULT_TICKER, sizeof(context->ticker_underlying));
        // // We will need an additional screen to display a warning message.
        // msg->additionalScreens++;
    }

    if (msg->token2 != NULL) {
        context->decimals_received = msg->token2->decimals;
        strlcpy(context->ticker_shares,
                (char *) msg->token2->ticker,
                sizeof(context->ticker_shares));
        context->tokens_found |= TOKEN_RECEIVED_FOUND;
    } else {
        // CAL did not find the token and token is not ETH.
        context->decimals_received = DEFAULT_DECIMAL;
        strlcpy(context->ticker_shares, DEFAULT_TICKER, sizeof(context->ticker_shares));
        // // We will need an additional screen to display a warning message.
        // msg->additionalScreens++;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}

static void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;

    strlcpy(msg->name, PLUGIN_NAME, msg->nameLength);

    switch (context->selectorIndex) {
        case DEPOSIT:
            strlcpy(msg->version, "Deposit", msg->versionLength);
            break;
        case WITHDRAW:
            strlcpy(msg->version, "Withdraw", msg->versionLength);
            break;
        default:
            PRINTF("Selector Index :%d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    msg->result = ETH_PLUGIN_RESULT_OK;
}

void stake_dao_plugin_call(int message, void *parameters) {
    PRINTF("Handling message %d\n", message);
    switch (message) {
        case ETH_PLUGIN_INIT_CONTRACT:
            handle_init_contract(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_PARAMETER:
            handle_provide_parameter(parameters);
            break;
        case ETH_PLUGIN_FINALIZE:
            handle_finalize(parameters);
            break;
        case ETH_PLUGIN_PROVIDE_TOKEN:
            handle_provide_token(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_ID:
            handle_query_contract_id(parameters);
            break;
        case ETH_PLUGIN_QUERY_CONTRACT_UI:
            handle_query_contract_ui(parameters);
            break;
        default:
            PRINTF("Unhandled message %d\n", message);
            break;
    }
}