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
 * (c) 2022-2025 Kiln
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

// DEFI - ERC20 Strategies
// --- cast sig "deposit(uint256,address)"
static const uint32_t KILN_DEFI_DEPOSIT_SELECTOR = 0x6e553f65;
// --- cast sig "mint(uint256,address)"
static const uint32_t KILN_DEFI_MINT_SELECTOR = 0x94bf804d;
// --- cast sig "withdraw(uint256,address,address)"
static const uint32_t KILN_DEFI_WITHDRAW_SELECTOR = 0xb460af94;
// --- cast sig "redeem(uint256,address,address)"
static const uint32_t KILN_DEFI_REDEEM_SELECTOR = 0xba087652;
// --- cast sig "approve(address,uint256)"
static const uint32_t KILN_DEFI_APPROVE_SELECTOR = 0x095ea7b3;
// --- cast sig "transfer(address,uint256)"
static const uint32_t KILN_DEFI_TRANSFER_SELECTOR = 0xa9059cbb;
// -- cast sig "transferFrom(address,address,uint256)"
static const uint32_t KILN_DEFI_TRANSFER_FROM_SELECTOR = 0x23b872dd;

const char ocv2_exit_queues[OCV2_MAX_EXIT_QUEUES][ADDRESS_STR_LEN] = {
    "0x8d6Fd650500f82c7D978a440348e5a9b886943bF",  // Kiln
    "0x86358F7B33b599c484e0335B8Ee4f7f7f92d8b60"   // Coinbase
};

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

// ****************************************************************************
// For DeFi, there is no way to get the 4626 asset() address in the kiln plugin
// because it is not passed in the calldata cf ERC4626 standard interface.
// To counter this, we have to hardcode the address and ticker of the asset()
// of each Kiln DeFi 4626 vault.
// ----------------------------------------------------------------------------
// for the following 6 arrays, indexes are always matching.

// note: we do not check chain ids because we trust the manifest of the app to do it
// and the deployer of the contract to not deploy on similar addresses cross chain.

// TODO: THESE ARE PLACEHOLDER CONTRACTS WAITING FOR LEDGER DEPLOYMENTS. WILL
// BE REPLACED WITH REAL ADDRESSES ONCE DEPLOYED AND REVIEWED BY AUDITORS.
const char defi_vaults_addresses[DEFI_VAULTS_COUNT][ADDRESS_STR_LEN] = {
    "0x03441c89e7B751bb570f9Dc8C92702b127c52C51",
    "0x6f15CDA2D68B00311614294A2b9b17400636133C",
    "0x4d1806C26A728f2e1b82b4549b9E074DBE5940B9"};

const char defi_vaults_names[DEFI_VAULTS_COUNT][ADDRESS_STR_LEN] = {"Cool Wallet AAVEv3 USDT",
                                                                    "BITNOVO AAVE v3 USDC",
                                                                    "Cool Wallet AAVEv3 USDT"};

const char defi_shares_names[DEFI_VAULTS_COUNT][MAX_TICKER_LEN] = {"cwAaveUSDT",
                                                                   "cwAaveUSDC",
                                                                   "cwAaveUSDT"};

const uint8_t defi_shares_decimals[DEFI_VAULTS_COUNT] = {18, 6, 18};

const char defi_assets_names[DEFI_VAULTS_COUNT][MAX_TICKER_LEN] = {"USDT", "USDC", "USDT"};

const uint8_t defi_assets_decimals[DEFI_VAULTS_COUNT] = {18, 6, 18};

// ****************************************************************************

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
    // DEFI
    KILN_DEFI_DEPOSIT_SELECTOR,
    KILN_DEFI_MINT_SELECTOR,
    KILN_DEFI_WITHDRAW_SELECTOR,
    KILN_DEFI_REDEEM_SELECTOR,
    KILN_DEFI_APPROVE_SELECTOR,
    KILN_DEFI_TRANSFER_SELECTOR,
    KILN_DEFI_TRANSFER_FROM_SELECTOR,
};
