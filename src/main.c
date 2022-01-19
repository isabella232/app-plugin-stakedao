/*******************************************************************************
 *   Ethereum 2 Deposit Application
 *   (c) 2020 Ledger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 ********************************************************************************/

#include "os.h"
#include "stakedao_plugin.h"

// Vault signatures
// function deposit(uint256 _amount)
static const uint8_t STAKEDAO_VAULT_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0xb6, 0xb5, 0x5f, 0x25};
// function withdraw(uint256 _shares)
static const uint8_t STAKEDAO_VAULT_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x1a, 0x7d, 0x4d};
// function depositAll()
static const uint8_t STAKEDAO_VAULT_DEPOSIT_ALL_SELECTOR[SELECTOR_SIZE] = {0xde, 0x5f, 0x62, 0x68};

// Options ETH Call signatures
// function depositETH(uint256)
static const uint8_t STAKEDAO_OPT_DEPOSIT_ETH_SELECTOR[SELECTOR_SIZE] = {0x53, 0x58, 0xfb, 0xda};
// function withdrawETH(uint256,uint256)
static const uint8_t STAKEDAO_OPT_WITHDRAW_ETH_SELECTOR[SELECTOR_SIZE] = {0xc7, 0xcd, 0xea, 0x37};

// Options signatures
// function depositUnderlying(uint256,uint256)
static const uint8_t STAKEDAO_OPT_DEPOSIT_UNDERLYING_SELECTOR[SELECTOR_SIZE] = {0x4d, 0x56, 0x93, 0xab};
// function depositCrvLP(uint256)
static const uint8_t STAKEDAO_OPT_DEPOSIT_CRVLP_SELECTOR[SELECTOR_SIZE] = {0xcc, 0x86, 0x90, 0xac};
// function withdrawUnderlying(uint256,uint256)
static const uint8_t STAKEDAO_OPT_WITHDRAW_UNDERLYING_SELECTOR[SELECTOR_SIZE] = {0xc2, 0x2f, 0xb4, 0xef};
// function withdrawCrvLp(uint256)
static const uint8_t STAKEDAO_OPT_WITHDRAW_CRVLP_SELECTOR[SELECTOR_SIZE] = {0xec, 0xfe, 0x05, 0x21};

// Premiums signatures
// function stake(uint256,uint256)
static const uint8_t STAKEDAO_PREMIUM_STAKE_SELECTOR[SELECTOR_SIZE] = {0x7b, 0x04, 0x72, 0xf0};
// function withdraw(uint256) same as vault withdraw but used another selector to be more precise
static const uint8_t STAKEDAO_PREMIUM_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x1a, 0x7d, 0x4d};
// function getReward()
static const uint8_t STAKEDAO_PREMIUM_GETREWARD_SELECTOR[SELECTOR_SIZE] = {0x3d, 0x18, 0xb9, 0x12};
// function exit()
static const uint8_t STAKEDAO_PREMIUM_EXIT_SELECTOR[SELECTOR_SIZE] = {0xe9, 0xfa, 0xd8, 0xee};

// LPs
// function deposit(uint256,uint256)
static const uint8_t STAKEDAO_LP_DEPOSIT_SELECTOR[SELECTOR_SIZE] = {0xe2, 0xbb, 0xb1, 0x58};
// function withdraw(uint256,uint256)
static const uint8_t STAKEDAO_LP_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x44, 0x1a, 0x3e, 0x70};

// Sanctuary
// function enter(uint256)
static const uint8_t STAKEDAO_SANCTUARY_ENTER_SELECTOR[SELECTOR_SIZE] = {0xa5, 0x9f, 0x3e, 0x0c};
// function leave(uint256)
static const uint8_t STAKEDAO_SANCTUARY_LEAVE_SELECTOR[SELECTOR_SIZE] = {0x67, 0xdf, 0xd4, 0xc9};

// Palace
// function stake(uint256)
static const uint8_t STAKEDAO_PALACE_STAKE_SELECTOR[SELECTOR_SIZE] = {0xa6, 0x94, 0xfc, 0x3a};
// function withdraw(uint256) same as vault withdraw but used another selector to be more precise
static const uint8_t STAKEDAO_PALACE_WITHDRAW_SELECTOR[SELECTOR_SIZE] = {0x2e, 0x1a, 0x7d, 0x4d};



// Array of all the different stakedao selectors.
const uint8_t *const STAKEDAO_SELECTORS[NUM_STAKEDAO_SELECTORS] = {
    STAKEDAO_VAULT_DEPOSIT_SELECTOR,
    STAKEDAO_VAULT_WITHDRAW_SELECTOR,
    STAKEDAO_VAULT_DEPOSIT_ALL_SELECTOR,
    STAKEDAO_OPT_DEPOSIT_ETH_SELECTOR,
    STAKEDAO_OPT_WITHDRAW_ETH_SELECTOR,
    STAKEDAO_OPT_DEPOSIT_UNDERLYING_SELECTOR,
    STAKEDAO_OPT_DEPOSIT_CRVLP_SELECTOR,
    STAKEDAO_OPT_WITHDRAW_UNDERLYING_SELECTOR,
    STAKEDAO_OPT_WITHDRAW_CRVLP_SELECTOR,
    STAKEDAO_PREMIUM_STAKE_SELECTOR,
    STAKEDAO_PREMIUM_WITHDRAW_SELECTOR,
    STAKEDAO_PREMIUM_GETREWARD_SELECTOR,
    STAKEDAO_PREMIUM_EXIT_SELECTOR,
    STAKEDAO_LP_DEPOSIT_SELECTOR,
    STAKEDAO_LP_WITHDRAW_SELECTOR,
    STAKEDAO_SANCTUARY_ENTER_SELECTOR,
    STAKEDAO_SANCTUARY_LEAVE_SELECTOR,
    STAKEDAO_PALACE_STAKE_SELECTOR,
    STAKEDAO_PALACE_WITHDRAW_SELECTOR
};

const stakedaoStrategy_t STAKEDAO_STRATEGIES[NUM_STAKEDAO_STRATEGIES] = {
    {{0xbc, 0x10, 0xc4, 0xf7, 0xb9, 0xfe, 0x0b, 0x30, 0x5e, 0x86,
      0x39, 0xb0, 0x4c, 0x53, 0x66, 0x33, 0xa3, 0xdb, 0x70, 0x65},
     "Passive stETH",
     "steCRV",
     "sdsteCRV",
     18},
    {{0x5a, 0xf1, 0x5d, 0xa8, 0x4A, 0x4a, 0x6e, 0xdf, 0x2d, 0x9f,
      0xa6, 0x72, 0x0d, 0xe9, 0x21, 0xe1, 0x02, 0x6e, 0x37, 0xb7},
     "Passive FRAX",
     "FRAX3CRV-f",
     "sdFRAX3CRV-f",
     18},
    {{0xa2, 0x76, 0x1b, 0x05, 0x39, 0x37, 0x4e, 0xb7, 0xaf, 0x21,
      0x55, 0xf7, 0x6e, 0xb0, 0x98, 0x64, 0xaf, 0x07, 0x52, 0x50},
     "Passive ETH",
     "eCRV",
     "sdeCRV",
     18},
    {{0xb1, 0x76, 0x40, 0x79, 0x6e, 0x4c, 0x27, 0xa3, 0x9a, 0xf5, 
      0x18, 0x87, 0xaf, 0xf3, 0xf8, 0xdc, 0x0d, 0xaf, 0x95, 0x67},
     "Passive USD",
     "3Crv",
     "sd3Crv",
     18},
    {{0xcd, 0x69, 0x97, 0x33, 0x48, 0x67, 0x72, 0x8b, 0xa1, 0x4d,
      0x79, 0x22, 0xf7, 0x2c, 0x89, 0x3f, 0xce, 0xe7, 0x0e, 0x84},
     "Passive EUR",
     "eursCRV",
     "sdeursCRV",
     18},
    {{0x24, 0x12, 0x9b, 0x93, 0x5a, 0xff, 0x07, 0x1c, 0x4f, 0x05,
      0x54, 0x88, 0x2c, 0x0d, 0x95, 0x73, 0xf4, 0x97, 0x5f, 0xed},
     "Passive BTC",
     "crvRenWSBTC",
     "sdcrvRenWSBTC",
     18},
    {{0x9b, 0x8f, 0x14, 0x55, 0x4f, 0x40, 0x70, 0x5d, 0xe7, 0x90,
      0x88, 0x79, 0xe2, 0x22, 0x8d, 0x2a, 0xc9, 0x4f, 0xde, 0x1a},
     "ETH Call",
     "ETH",
     "sdETHCovCall",
     18},
    {{0x22, 0x7e, 0x46, 0x35, 0xc5, 0xfe, 0x22, 0xD1, 0xe3, 0x6d,
      0xab, 0x1c, 0x92, 0x1b, 0x62, 0xf8, 0xac, 0xc4, 0x51, 0xb9},
     "BTC Call",
     "BTC",
     "sdWbtcCovCall",
     18},
    {{0x83, 0x9a, 0x98, 0x9b, 0xe4, 0x0f, 0x2d, 0x60, 0xf0, 0x0b, 
      0xee, 0xb6, 0x48, 0x90, 0x37, 0x32, 0xc0, 0x41, 0xcb, 0xd7},
     "ETH Put",
     "FRAX",
     "sdETH-FRAXPut",
     18},
    {{0xac, 0x14, 0x86, 0x4c, 0xe5, 0xa9, 0x8a, 0xf3, 0x24, 0x8f,
      0xfb, 0xf5, 0x49, 0x44, 0x1b, 0x04, 0x42, 0x12, 0x47, 0xd3},
     "Sanctuary",
     "SDT",
     "xSDT",
     18},
    {{0x22, 0x17, 0x38, 0xf7, 0x3f, 0xa4, 0xbf, 0xca, 0x91, 0x91, 
      0x8e, 0x77, 0xd1, 0x12, 0xb8, 0x7d, 0x91, 0x8c, 0x75, 0x1f},
     "Palace",
     "xSDT",
     "Points",
     18}
};

void dispatch_plugin_calls(int message, void *parameters) {
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

#define RUN_APPLICATION 1

void call_app_ethereum() {
    unsigned int libcall_params[3];
    libcall_params[0] = (unsigned int) "Ethereum";
    libcall_params[1] = 0x100;
    libcall_params[2] = RUN_APPLICATION;
    os_lib_call((unsigned int *) &libcall_params);
}

__attribute__((section(".boot"))) int main(int arg0) {
    // exit critical section
    __asm volatile("cpsie i");

    // ensure exception will work as planned
    os_boot();

    BEGIN_TRY {
        TRY {
            check_api_level(CX_COMPAT_APILEVEL);

            if (!arg0) {
                // called from dashboard, launch Ethereum app
                call_app_ethereum();
                return 0;
            } else {
                // regular call from ethereum
                unsigned int *args = (unsigned int *) arg0;

                if (args[0] != ETH_PLUGIN_CHECK_PRESENCE) {
                    dispatch_plugin_calls(args[0], (void *) args[1]);
                }
                os_lib_end();
            }
        }
        FINALLY {
        }
    }
    END_TRY;

    return 0;
}
