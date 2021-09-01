#pragma once

#include <string.h>
#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include "debug_write.h"

#define PARAMETER_LENGTH 32
#define SELECTOR_SIZE    4

#define RUN_APPLICATION 1

#define NUM_STAKE_DAO_SELECTORS 2
#define SELECTOR_SIZE          4

#define PLUGIN_NAME "stakedao"

#define TOKEN_SENT_FOUND     1
#define TOKEN_RECEIVED_FOUND 1 << 1

typedef enum {
    DEPOSIT,
    WITHDRAW,
} stakeDAOSelector_t;

#define PARTIAL_FILL 1

typedef enum {
    DEPOSIT_SCREEN,
    WITHDRAW_SCREEN,
    ERROR,
} screens_t;

// Would've loved to make this an enum but we don't have enough room because enums are `int` and not
// `uint8_t`.

// Deposit Actions
#define AMOUNT_DEPOSITED 0  // Amount deposited by the user to the strategy.
#define SHARES_RECEIVED 1  // Share sent by the strategy to the user.

// Withdraw Actions
#define SHARES_BURNED 2 // shares to burn
#define AMOUNT_WITHDRAWN 3 // amount withdrawn

// Common Actions
#define TOKEN_UNDER 4  // Address of the token the user is sending.
#define TOKEN_SHARES 5  // Address of the token sent to the user.

#define NONE 6  // Placeholder variant to be set when parsing is done but data is still being sent.

// Number of decimals used when the token wasn't found in the CAL.
#define DEFAULT_DECIMAL WEI_TO_ETHER

// Ticker used when the token wasn't found in the CAL.
#define DEFAULT_TICKER ""

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct stake_dao_parameters_t {
    uint8_t amount_deposited[INT256_LENGTH];
    uint8_t shares_received[INT256_LENGTH];
    uint8_t beneficiary[ADDRESS_LENGTH];
    uint8_t contract_address_shares[ADDRESS_LENGTH];
    uint8_t contract_address_underlying[ADDRESS_LENGTH];
    char ticker_shares[MAX_TICKER_LEN];
    char ticker_underlying[MAX_TICKER_LEN];
    
    // TODO (recalculate the final size)

    // 32 * 2 + 20 * 3 + 12 * 2 == 64 + 60 + 24 == 144
    // 32 * 5 == 160 bytes so there are 160 - 144 == 16 bytes left.

    uint16_t offset;
    uint16_t checkpoint;
    uint8_t next_param;
    uint8_t tokens_found;
    uint8_t valid;
    uint8_t decimals_sent;
    uint8_t decimals_received;
    uint8_t selectorIndex;
    uint8_t flags;
    uint8_t skip;
    // 4 * 1 + 2 * 2 + 7 * 1 == 8 + 7 == 15 bytes. There are 16 - 15 == 1 byte left.
} stake_dao_parameters_t;

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void stake_dao_plugin_call(int message, void *parameters);
void printf_hex_array(const char *title, int len, const uint8_t *data);