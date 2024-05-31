/*******************************************************************************
 *
 * ██╗  ██╗██╗██╗     ███╗   ██╗
 * ██║ ██╔╝██║██║     ████╗  ██║
 * █████╔╝ ██║██║     ██╔██╗ ██║
 * ██╔═██╗ ██║██║     ██║╚██╗██║
 * ██║  ██╗██║███████╗██║ ╚████║
 * ╚═╝  ╚═╝╚═╝╚══════╝╚═╝  ╚═══╝
 *
 * Kiln Ethereum Ledger App
 * (c) 2022-2024 Kiln
 *
 * contact@kiln.fi
 ********************************************************************************/

#include "kiln_plugin.h"

// ONCHAIN V1 - Solo Staking
// --- cast sig "deposit()"
static const uint32_t KILN_V1_DEPOSIT_SELECTOR = 0xd0e30db0;
// --- cast sig "withdraw(bytes)"
static const uint32_t KILN_V1_WITHDRAW_SELECTOR = 0x0968f264;
// --- cast sig "withdrawELFee(bytes)"
static const uint32_t KILN_V1_WITHDRAW_EL_SELECTOR = 0xbf509bd4;
// --- cast sig "withdrawCLFee(bytes)"
static const uint32_t KILN_V1_WITHDRAW_CL_SELECTOR = 0x2ba03a79;
// --- cast sig "batchWithdraw(bytes)"
static const uint32_t KILN_V1_BATCH_WITHDRAW_SELECTOR = 0xe8a0c121;
// --- cast sig "batchWithdrawELFee(bytes)"
static const uint32_t KILN_V1_BATCH_WITHDRAW_EL_SELECTOR = 0xef5e4682;
// --- cast sig "batchWithdrawCLFee(bytes)"
static const uint32_t KILN_V1_BATCH_WITHDRAW_CL_SELECTOR = 0x0ffab6c2;
// --- cast sig "requestValidatorsExit(bytes)"
static const uint32_t KILN_V1_REQUEST_EXIT_SELECTOR = 0xb6b06dec;

// ONCHAIN V2 - Pooled Staking
// --- cast sig "stake()"
static const uint32_t KILN_V2_STAKE_SELECTOR = 0x3a4b66f1;
// --- cast sig "requestExit(uint256)"
static const uint32_t KILN_V2_REQUEST_EXIT_SELECTOR = 0x721c6513;
// --- cast sig "multiClaim(address[],uint256[][],uint32[][])"
static const uint32_t KILN_V2_MULTICLAIM_SELECTOR = 0xb7ba18c7;
// --- cast sig "claim(uint256[],uint32[],uint16)"
static const uint32_t KILN_V2_CLAIM_SELECTOR = 0xadcf1163;

// RESTAKING - Liquid Restaking
// --- cast sig "depositIntoStrategy(address,address,uint256)"
static const uint32_t KILN_LR_DEPOSIT_INTO_STRATEGY_SELECTOR = 0xe7a050aa;
// --- cast sig "queueWithdrawals((address[],uint256[],address)[])"
static const uint32_t KILN_LR_QUEUE_WITHDRAWALS_SELECTOR = 0x0dd8dd02;
// --- cast sig
// "completeQueuedWithdrawals((address,address,address,uint256,uint32,address[],uint256[])[],address[][],uint256[],bool[])"
static const uint32_t KILN_LR_COMPLETE_QUEUED_WITHDRAWALS_SELECTOR = 0x33404396;
// --- cast sig "delegateTo(address,(bytes,uint256),bytes32)"
static const uint32_t KILN_LR_DELEGATE_TO_SELECTOR = 0xeea9064b;
// --- cast sig "undelegate(address)"
static const uint32_t KILN_LR_UNDELEGATE_SELECTOR = 0xda8be864;

const char lr_strategy_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN] = {
    "0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc",  // cbETH
    "0x93c4b944D05dfe6df7645A86cd2206016c51564D",  // stETH
    "0x1BeE69b7dFFfA4E2d53C2a2Df135C388AD25dCD2",  // rETH
    "0x9d7eD45EE2E8FC5482fa2428f15C971e6369011d",  // ETHx
    "0x13760F50a9d7377e4F20CB8CF9e4c26586c658ff",  // ankrETH
    "0xa4C637e0F704745D182e4D38cAb7E7485321d059",  // OETH
    "0x57ba429517c3473B6d34CA9aCd56c0e735b94c02",  // osETH
    "0x0Fe4F44beE93503346A3Ac9EE5A26b130a5796d6",  // swETH
    "0x7CA911E83dabf90C90dD3De5411a10F1A6112184",  // wBETH
    "0x8CA7A5d6f3acd3A7A8bC468a8CD0FB14B6BD28b6",  // sfrxETH
    "0x298aFB19A105D59E74658C4C334Ff360BadE6dd2"   // mETH
};

const char lr_erc20_addresses[LR_STRATEGIES_COUNT][ADDRESS_STR_LEN] = {
    "0xBe9895146f7AF43049ca1c1AE358B0541Ea49704",  // cbETH
    "0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84",  // stETH
    "0xae78736Cd615f374D3085123A210448E74Fc6393",  // rETH
    "0xA35b1B31Ce002FBF2058D22F30f95D405200A15b",  // ETHx
    "0xE95A203B1a91a908F9B9CE46459d101078c2c3cb",  // ankrETH
    "0x856c4Efb76C1D1AE02e20CEB03A2A6a08b0b8dC3",  // OETH
    "0xf1C9acDc66974dFB6dEcB12aA385b9cD01190E38",  // osETH
    "0xf951E335afb289353dc249e82926178EaC7DEd78",  // swETH
    "0xa2E3356610840701BDf5611a53974510Ae27E2e1",  // wBETH
    "0xac3E018457B222d93114458476f3E3416Abbe38F",  // sfrxETH
    "0xd5F7838F5C461fefF7FE49ea5ebaF7728bB0ADfa",  // mETH
};

const char lr_tickers[LR_STRATEGIES_COUNT][MAX_TICKER_LEN] = {"cbETH",
                                                              "stETH",
                                                              "rETH",
                                                              "ETHx",
                                                              "ankrETH",
                                                              "OETH",
                                                              "osETH",
                                                              "swETH",
                                                              "wBETH",
                                                              "sfrxETH",
                                                              "mETH"};

const char lr_kiln_operator_address[ADDRESS_STR_LEN] = "0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662";

// Array of all supported selectors.
const uint32_t KILN_SELECTORS[NUM_SELECTORS] = {
    // V1
    KILN_V1_DEPOSIT_SELECTOR,
    KILN_V1_WITHDRAW_SELECTOR,
    KILN_V1_WITHDRAW_EL_SELECTOR,
    KILN_V1_WITHDRAW_CL_SELECTOR,
    KILN_V1_BATCH_WITHDRAW_SELECTOR,
    KILN_V1_BATCH_WITHDRAW_EL_SELECTOR,
    KILN_V1_BATCH_WITHDRAW_CL_SELECTOR,
    KILN_V1_REQUEST_EXIT_SELECTOR,
    // V2
    KILN_V2_STAKE_SELECTOR,
    KILN_V2_REQUEST_EXIT_SELECTOR,
    KILN_V2_MULTICLAIM_SELECTOR,
    KILN_V2_CLAIM_SELECTOR,
    // EigenLayer
    KILN_LR_DEPOSIT_INTO_STRATEGY_SELECTOR,
    KILN_LR_QUEUE_WITHDRAWALS_SELECTOR,
    KILN_LR_COMPLETE_QUEUED_WITHDRAWALS_SELECTOR,
    KILN_LR_DELEGATE_TO_SELECTOR,
    KILN_LR_UNDELEGATE_SELECTOR,
};
