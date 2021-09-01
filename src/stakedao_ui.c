#include "stakedao_plugin.h"

// Prepend `dest` with `ticker`.
// Dest must be big enough to hold `ticker` + `dest` + `\0`.
static void prepend_ticker(char *dest, uint8_t destsize, char *ticker) {
    if (dest == NULL || ticker == NULL) {
        THROW(0x6503);
    }
    // Add extra 1 for space
    uint8_t ticker_len = strlen(ticker) + 1;
    uint8_t dest_len = strlen(dest);

    if (dest_len + ticker_len >= destsize) {
        THROW(0x6503);
    }

    // Right shift the string by `ticker_len` bytes.
    while (dest_len != 0) {
        dest[dest_len + ticker_len] = dest[dest_len];  // First iteration will copy the \0
        dest_len--;
    }
    // Don't forget to null terminate the string.
    dest[ticker_len] = dest[0];

    // Copy the ticker to the beginning of the string.
    memcpy(dest, ticker, ticker_len - 1);
    // Add space after ticker name
    dest[ticker_len - 1] = ' ';
}

// Set UI for the "Deposit" screen.
static void set_deposit_ui(ethQueryContractUI_t *msg, stake_dao_parameters_t *context) {
    switch (context->selectorIndex) {
        case DEPOSIT:
            strlcpy(msg->title, "Deposit", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    if (!(context->tokens_found & TOKEN_SENT_FOUND)) {
        strlcpy(msg->msg, "Unknown token", msg->msgLength);
        return;
    }

    adjustDecimals((char *) context->amount_deposited,
                   strnlen((char *) context->amount_deposited, sizeof(context->amount_deposited)),
                   msg->msg,
                   msg->msgLength,
                   context->decimals_sent);

    prepend_ticker(msg->msg, msg->msgLength, context->ticker_underlying);
}

// Set UI for "Receive" screen.
static void set_shares_ui(ethQueryContractUI_t *msg, stake_dao_parameters_t *context) {
    switch (context->selectorIndex) {
        case DEPOSIT:
            strlcpy(msg->title, "Receive Shares", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    if (!(context->tokens_found & TOKEN_RECEIVED_FOUND)) {
        strlcpy(msg->msg, "Unknown token", msg->msgLength);
        return;
    }

    adjustDecimals((char *) context->shares_received,
                   strnlen((char *) context->shares_received, sizeof(context->shares_received)),
                   msg->msg,
                   msg->msgLength,
                   context->decimals_received);

    prepend_ticker(msg->msg, msg->msgLength, context->ticker_shares);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(ethQueryContractUI_t *msg, stake_dao_parameters_t *context) {
    uint8_t index = msg->screenIndex;

    if (index == 0) {
        return DEPOSIT_SCREEN;
    } else if (index == 1) {
        return WITHDRAW_SCREEN;
    }

    return ERROR;
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    stake_dao_parameters_t *context = (stake_dao_parameters_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    screens_t screen = get_screen(msg, context);
    switch (screen) {
        case DEPOSIT_SCREEN:
            set_deposit_ui(msg, context);
            break;
        case WITHDRAW_SCREEN:
            set_shares_ui(msg, context);
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}