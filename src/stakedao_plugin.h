#pragma once

#include "eth_plugin_interface.h"
#include <stdbool.h>

#define PARAMETER_LENGTH 32
#define SELECTOR_SIZE 4
#define MAX_STRATEGY_TICKER_LEN 16
#define NUM_STAKEDAO_SELECTORS 26
#define NUM_STAKEDAO_STRATEGIES 19
#define NUM_CURVE_POOLS 4

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
    PALACE_WITHDRAW,
    REWARDS_CLAIM,
    NFT_STAKE,
    NFT_UNSTAKE,
    ANGLE_GET_REWARD,
    CURVE_ADD_L_2,
    CURVE_ADD_L_3,
    CURVE_ADD_L_3_UNDER
} stakedaoSelector_t;

typedef enum {
    AMOUNT,
    TOKEN,
    VAULT,
    PID,
    MIN_AMOUNT,
    NFT_ID,
    MERKLE_INDEX,
    INDEX,
    MERKLE_PROOF,
    USER,
    TOKEN_1,
    TOKEN_2,
    TOKEN_3
} selectorField;

extern const uint8_t *const STAKEDAO_SELECTORS[NUM_STAKEDAO_SELECTORS];

typedef struct stakedaoStrategy_t {
    uint8_t address[ADDRESS_LENGTH];
    char strategy[MAX_STRATEGY_TICKER_LEN];
    char want[MAX_STRATEGY_TICKER_LEN];
    char vault[MAX_STRATEGY_TICKER_LEN];
    uint8_t decimals;
} stakedaoStrategy_t;

typedef struct stakedaoCrvPool_t {
    uint8_t address[ADDRESS_LENGTH];
    char want[MAX_STRATEGY_TICKER_LEN];
    char pool[MAX_STRATEGY_TICKER_LEN];
} stakedaoCrvPool_t;

extern stakedaoStrategy_t const STAKEDAO_STRATEGIES[NUM_STAKEDAO_STRATEGIES];
extern stakedaoCrvPool_t const CURVE_POOLS[NUM_CURVE_POOLS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct stakedao_parameters_t {
    uint8_t amount[INT256_LENGTH];
    uint8_t min_amount[INT256_LENGTH];
    uint8_t address[ADDRESS_LENGTH];
    char strategy[MAX_STRATEGY_TICKER_LEN];
    char want[MAX_STRATEGY_TICKER_LEN];
    char vault[MAX_STRATEGY_TICKER_LEN];
    uint8_t pid[MAX_STRATEGY_TICKER_LEN];
    uint8_t decimals;
    uint8_t next_param;
    stakedaoSelector_t selectorIndex;
} stakedao_parameters_t;

extern uint8_t const STAKEDAO_NFT_BOOST[ADDRESS_LENGTH];

_Static_assert(sizeof(stakedao_parameters_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_query_contract_id(void *parameters);