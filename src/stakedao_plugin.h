#pragma once

#include "eth_plugin_interface.h"
#include <stdbool.h>

#define PARAMETER_LENGTH 32
#define SELECTOR_SIZE 4
#define MAX_STRATEGY_TICKER_LEN 16
#define NUM_STAKEDAO_SELECTORS 19
#define NUM_STAKEDAO_STRATEGIES 14

#define PLUGIN_NAME "StakeDAO"

// Enumeration of the different selectors possible.
// Should follow the exact same order as the array declared in main.c
typedef enum {
    VAULT_DEPOSIT,
    VAULT_WITHDRAW,
    VAULT_DEPOSIT_ALL,
    OPT_DEPOSIT_ETH,
    OPT_WITHDRAW_ETH,
    OPT_DEPOSIT_UNDERLYING,
    OPT_DEPOSIT_CRVLP,
    OPT_WITHDRAW_UNDERLYING,
    OPT_WITHDRAW_CRVLP,
    PREMIUM_STAKE,
    PREMIUM_WITHDRAW,
    PREMIUM_GETREWARD,
    PREMIUM_EXIT,
    LP_DEPOSIT,
    LP_WITHDRAW,
    SANCTUARY_ENTER,
    SANCTUARY_LEAVE,
    PALACE_STAKE,
    PALACE_WITHDRAW
} stakedaoSelector_t;

typedef enum {
    AMOUNT,
    TOKEN,
    VAULT,
    PID,
    MIN_AMOUNT,
    NFT_ID
} selectorField;

extern const uint8_t *const STAKEDAO_SELECTORS[NUM_STAKEDAO_SELECTORS];

typedef struct stakedaoStrategy_t {
    uint8_t address[ADDRESS_LENGTH];
    char strategy[MAX_STRATEGY_TICKER_LEN];
    char want[MAX_STRATEGY_TICKER_LEN];
    char vault[MAX_STRATEGY_TICKER_LEN];
    uint8_t decimals;
} stakedaoStrategy_t;

extern stakedaoStrategy_t const STAKEDAO_STRATEGIES[NUM_STAKEDAO_STRATEGIES];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct stakedao_parameters_t {
    uint8_t amount[INT256_LENGTH];
    uint8_t min_amount[INT256_LENGTH];
    uint8_t address[ADDRESS_LENGTH];
    char strategy[MAX_STRATEGY_TICKER_LEN];
    char want[MAX_STRATEGY_TICKER_LEN];
    char vault[MAX_STRATEGY_TICKER_LEN];
    char pid[MAX_STRATEGY_TICKER_LEN];
    uint8_t decimals;
    uint8_t next_param;
    stakedaoSelector_t selectorIndex;
} stakedao_parameters_t;

_Static_assert(sizeof(stakedao_parameters_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_query_contract_id(void *parameters);