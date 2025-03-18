# Badges

[![Code style check](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/lint-workflow.yml/badge.svg)](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/lint-workflow.yml)
[![Compilation & tests](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/ci-workflow.yml/badge.svg)](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/ci-workflow.yml)

# app-plugin-kiln

![Kiln Logo](https://uploads-ssl.webflow.com/625db3caa8abd6c22d5f0ce3/6260572336892a801afd71e3_Kiln_Logo-Transparent-Dark.svg)

This plug-in is compatible with Nano S / X / SP devices, it enables to
interact in a secure way with the Kiln On-Chain v1, v2, EigenLayer Liquid Restaking contracts, and DeFi vault contracts.

## Documentation

### Plugin codebase

This plugin follows the structure of a Ledger Ethereum app plugin described [here](https://developers.ledger.com/docs/device-app/develop/code/plugin).

It receives data from the Ethereum app, processes it and displays (ie "clearsign") on the device screen.

All the plugin code is in the `src` folder.

The plugin codebase follows the 6 steps of the plugin flow:

1. **initialize** (in `src/handle_init_contract.c`): plugin receives the selector to manage, and init the correct parser initial state for the function (`context->next_param`).
2. **provide parameter** (in `src/provide_parameter/handle_provide_parameter.c`): selected parser receives parameters 32 bytes by 32 bytes, and parse them to the correct state. See files like `src/provide_parameter/eigenlayer.c` for the different parsers.
3. **finalize** (in `src/handle_finalize.c`): plugin returns the number of screens to display
4. **provide_token** not used here
5. **query_contract_id** (in `src/handle_query_contract_id.c`): plugin returns the contract name as the first displayed screen on the device
6. **query_contract_ui** (in `src/query_contract_ui/handle_query_contract_ui.c`): plugin returns the screens to display on the device, each product has its own file like `src/query_contract_ui/eigenlayer.c`.

Other important files:

- `src/contracts.c` holds all the constant representing contract functions, parameters or known contracts mappings.
- `src/kiln_plugin.h` is where we define all the parser states and the plugin context structure.

### Plugin context

The plugin context is defined in `src/kiln_plugin.h` and holds the state of the plugin. It is passed to all the plugin functions.

It is defined using a union of all the possible parser parameters of the plugin, and a `next_param` field to know which parser state to use next.

The plugin context is memset to 0 everytime init_contract is called (beginning of a clearsigning flow), so we make sure that the plugin context union is in a clean state before using it in the parsers.

### Provide parameter parsers

The provide parameter parsers are defined in `src/provide_parameter/` and are called by the `handle_provide_parameter` function.

Each parser is a state machine that receives 32 bytes of data at a time (abi format), and updates the plugin context state accordingly.

### Build

```shell
$ git clone --recurse-submodules https://github.com/kilnfi/app-plugin-kiln-v2.git
$ cd app-plugin-kiln-v2/
$ docker run --rm -it \
    -v "$(realpath .):/app" \
    --user $(id -u $USER):$(id -g $USER) \
    ghcr.io/ledgerhq/ledger-app-builder/ledger-app-dev-tools:latest \
    make
```

### Tests

The tests are in the `tests/tests` folder. They are written in python and use the ragger framework developed by Ledger.

The output of the tests is a series of screenshots stored in the `tests/snapshots-tmp` folder that are compared to the expected screenshots in the `tests/snapshots` folder. The latter are stored in the git repository to make sure we expect the correct outputs.

The recommended way to test the plugin is to use the Ledger vscode extension to build and run the tests.

### Fuzzing

The plugin is fuzzed using the `fuzzing` folder following ledger boilerplate.

### Continuous Integration

The flow processed in [GitHub Actions](https://github.com/features/actions) is the following:

- Code formatting with [clang-format](http://clang.llvm.org/docs/ClangFormat.html)
- Compilation of the application for Ledger Nano S in [ledger-app-builder](https://github.com/LedgerHQ/ledger-app-builder)

### Supported Smart Contracts

Smart contracts covered by this plugin are:

| Network | Name                          | Address                                      |
| ------- | ----------------------------- | -------------------------------------------- |
| Mainnet | Kiln On-Chain v1              | `0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270` |
| Mainnet | Kiln On-Chain v2 Native20     | `0x5db5235b5c7e247488784986e58019fffd98fda4` |
| Mainnet | Kiln On-Chain v2 ExitQueue    | `0x8d6fd650500f82c7d978a440348e5a9b886943bf` |
| Mainnet | EigenLayer Strategy Manager   | `0x858646372cc42e1a627fce94aa7a7033e7cf075a` |
| Mainnet | EigenLayer Delegation Manager | `0x39053d51b77dc0d36036fc1fcc8cb819df8ef37a` |
| Mainnet | Cool Wallet AAVEv3 USDT       | `0x03441c89e7B751bb570f9Dc8C92702b127c52C51` |
| Mainnet | BITNOVO AAVE v3 USDC          | `0x6f15CDA2D68B00311614294A2b9b17400636133C` |
| Mainnet | Cool Wallet AAVEv3 USDT       | `0x4d1806C26A728f2e1b82b4549b9E074DBE5940B9` |

## Clearsigning

### KILN ON-CHAIN v1 - Deposit

Function: `deposit()`
Clearsigned:

- tx.value

![](/tests/snapshots/nanosp/test_ocv1_deposit/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_deposit/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_deposit/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_deposit/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_deposit/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_deposit/00005.png)

### KILN ON-CHAIN v1 - Withdraw

Function `withdraw(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_withdraw/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw/00005.png)

### KILN ON-CHAIN v1 - Withdraw EL

Function `withdrawEL(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_el/00005.png)

### KILN ON-CHAIN v1 - Withdraw CL

Function `withdrawCL(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_withdraw_cl/00005.png)

### KILN ON-CHAIN v1 - Batch Withdraw

Function `batchWithdraw(bytes validatorPubkeys)`

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw/00005.png)

### KILN ON-CHAIN v1 - Batch Withdraw EL

Function `batchWithdrawEL(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_el/00005.png)

### KILN ON-CHAIN v1 - Batch WithdrawCL

Function `batchWithdrawCL(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_batch_withdraw_cl/00005.png)

### KILN ON-CHAIN v1 - Request Exits

Function `requestValidatorsExit(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can request exit on its behalf so no attack vector possible.

![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00000.png) ![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00001.png) ![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00002.png) ![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00003.png) ![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00004.png) ![](/tests/snapshots/nanosp/test_ocv1_request_validators_exit/00005.png)

### KILN ON-CHAIN v2 - Stake

Function: `stake()`
Clearsigned:

- tx.value

![](/tests/snapshots/nanosp/test_ocv2_stake/00000.png) ![](/tests/snapshots/nanosp/test_ocv2_stake/00001.png) ![](/tests/snapshots/nanosp/test_ocv2_stake/00002.png) ![](/tests/snapshots/nanosp/test_ocv2_stake/00003.png) ![](/tests/snapshots/nanosp/test_ocv2_stake/00004.png) ![](/tests/snapshots/nanosp/test_ocv2_stake/00005.png)

### KILN ON-CHAIN v2 - RequestExit

Function: `requestExit(uint256 shares)`
Clearsigned:

- none as shares is not 1:1 with ETH value, it would confuse users

![](/tests/snapshots/nanosp/test_ocv2_request_exit/00000.png) ![](/tests/snapshots/nanosp/test_ocv2_request_exit/00001.png) ![](/tests/snapshots/nanosp/test_ocv2_request_exit/00002.png) ![](/tests/snapshots/nanosp/test_ocv2_request_exit/00003.png) ![](/tests/snapshots/nanosp/test_ocv2_request_exit/00004.png) ![](/tests/snapshots/nanosp/test_ocv2_request_exit/00005.png)

### KILN ON-CHAIN v2 - MultiClaim

Function: `multiClaim(address[] exitQueues, uint256[][] ticketIds, uint32[][] casksIds)`
Clearsigned:

- none

![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00000.png) ![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00001.png) ![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00002.png) ![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00003.png) ![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00004.png) ![](/tests/snapshots/nanosp/test_ocv2_multiclaim/00005.png)

### KILN ON-CHAIN v2 - Claim

Function: `claim(uint256[] ticketIds, uint32[] caskIds, uint16 maxClaimDepth)`
Clearsigned:

- none

![](/tests/snapshots/nanosp/test_ocv2_claim/00000.png) ![](/tests/snapshots/nanosp/test_ocv2_claim/00001.png) ![](/tests/snapshots/nanosp/test_ocv2_claim/00002.png) ![](/tests/snapshots/nanosp/test_ocv2_claim/00003.png) ![](/tests/snapshots/nanosp/test_ocv2_claim/00004.png) ![](/tests/snapshots/nanosp/test_ocv2_claim/00005.png)

### EigenLayer Strategy Manager - LR Deposit Into Strategy

Function: `depositIntoStrategy(address strategy, address erc20, uint256 amount)`
Clearsigned:

- strategy: using mapping in `contracts.c`, it is either recognized or displayed as "UNKOWN"
- erc20: using mapping in `contracts.c`, it is either recognized or displayed as "UNKOWN"
- amount

![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00000.png) ![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00001.png) ![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00002.png) ![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00003.png) ![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00004.png) ![](/tests/snapshots/nanosp/test_lr_deposit_into_strategy/00005.png)

### EigenLayer Delegation Manager - Delegate To

Function:

```solidity
struct SignatureWithExpiry {
    bytes signature;
    uint256 expiry;
}

function delegateTo(
    address operator,
    SignatureWithExpiry memory approverSignatureAndExpiry,
    bytes32 approverSalt
)
```

Clearsigned:

- operator: using mapping in `contracts.c`, it is either recognized or displayed as address

Shares are confusing for users and would be hard to store properly on the device for display.

![](/tests/snapshots/nanosp/test_lr_delegate_to/00000.png) ![](/tests/snapshots/nanosp/test_lr_delegate_to/00001.png) ![](/tests/snapshots/nanosp/test_lr_delegate_to/00002.png) ![](/tests/snapshots/nanosp/test_lr_delegate_to/00003.png) ![](/tests/snapshots/nanosp/test_lr_delegate_to/00004.png) ![](/tests/snapshots/nanosp/test_lr_delegate_to/00005.png)

### EigenLayer Delegation Manager - Undelegate

Function: `undelegate(address staker)`
Clearsigned:

- none as there is a staker == msg.sender requirement in the contract

![](/tests/snapshots/nanosp/test_lr_undelegate/00000.png) ![](/tests/snapshots/nanosp/test_lr_undelegate/00001.png) ![](/tests/snapshots/nanosp/test_lr_undelegate/00002.png) ![](/tests/snapshots/nanosp/test_lr_undelegate/00003.png) ![](/tests/snapshots/nanosp/test_lr_undelegate/00004.png) ![](/tests/snapshots/nanosp/test_lr_undelegate/00005.png)

### EigenLayer Strategy Manager - Queue Withdrawals

Function:

```solidity
struct QueuedWithdrawalParams {
    address[] strategies;
    uint256[] shares;
    address withdrawer;


function queueWithdrawals(
    QueuedWithdrawalParams[] queuedWithdrawalParams
)
```

Clearsigned:

- strategies: using mapping in `contracts.c`, they are either recognized or displayed as "UNKOWN"
- withdrawer

Shares are confusing for users and would be hard to store properly on the device for display.

![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00000.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00001.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00002.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00003.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00004.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00005.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00006.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00007.png) ![](/tests/snapshots/nanosp/test_lr_queue_withdrawls/00008.png)

### EigenLayer Strategy Manager - LR Complete Queued Withdrawals

Function:

```solidity
struct Withdrawal {
    address staker;
    address delegatedTo;
    address withdrawer;
    uint256 nonce;
    uint32 startBlock;
    address[] strategies;
    uint256[] shares;
}

function completeQueuedWithdrawals(
    Withdrawal[] withdrawals,
    address[][] tokens,
    uint256[] middlewareTimesIndexes,
    bool[] receiveAsTokens
)
```

Clearsigned:

- strategies: using mapping in `contracts.c`, they are either recognized or displayed as "UNKOWN"
- if each withdrawal corresponding receiveAsTokens is true, then the corresponding token is displayed as "Withdraw" else as "Redelegation

Shares are confusing for users and would be hard to store properly on the device for display.

![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00000.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00001.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00002.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00003.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00004.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00005.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00006.png) ![](/tests/snapshots/nanosp/test_lr_complete_queued_withdrawals/00007.png)

### DeFi - Deposit

Function: `deposit(uint256 assets, address receiver)`
Clearsigned:

- assets: amount of assets to deposit
- receiver: address that will receive the shares

![](/tests/snapshots/nanosp/test_defi_deposit/00000.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00001.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00002.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00003.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00004.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00005.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00006.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00007.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00008.png) ![](/tests/snapshots/nanosp/test_defi_deposit/00009.png)

### DeFi - Mint

Function: `mint(uint256 shares, address receiver)`
Clearsigned:

- shares: amount of shares to mint
- receiver: address that will receive the shares

![](/tests/snapshots/nanosp/test_defi_mint/00000.png) ![](/tests/snapshots/nanosp/test_defi_mint/00001.png) ![](/tests/snapshots/nanosp/test_defi_mint/00002.png) ![](/tests/snapshots/nanosp/test_defi_mint/00003.png) ![](/tests/snapshots/nanosp/test_defi_mint/00004.png) ![](/tests/snapshots/nanosp/test_defi_mint/00005.png) ![](/tests/snapshots/nanosp/test_defi_mint/00006.png) ![](/tests/snapshots/nanosp/test_defi_mint/00007.png) ![](/tests/snapshots/nanosp/test_defi_mint/00008.png) ![](/tests/snapshots/nanosp/test_defi_mint/00009.png)

### DeFi - Withdraw

Function: `withdraw(uint256 assets, address receiver, address owner)`
Clearsigned:

- assets: amount of assets to withdraw
- receiver: address that will receive the assets
- owner: address that owns the shares

![](/tests/snapshots/nanosp/test_defi_withdraw/00000.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00001.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00002.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00003.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00004.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00005.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00006.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00007.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00008.png) ![](/tests/snapshots/nanosp/test_defi_withdraw/00009.png)

### DeFi - Redeem

Function: `redeem(uint256 shares, address receiver, address owner)`
Clearsigned:

- shares: amount of shares to redeem
- receiver: address that will receive the assets
- owner: address that owns the shares

![](/tests/snapshots/nanosp/test_defi_redeem/00000.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00001.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00002.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00003.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00004.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00005.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00006.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00007.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00008.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00009.png) ![](/tests/snapshots/nanosp/test_defi_redeem/00010.png)

### DeFi - Approve

Function: `approve(address spender, uint256 amount)`
Clearsigned:

- spender: address to allow spending tokens
- amount: amount of tokens that can be spent

![](/tests/snapshots/nanosp/test_defi_approve/00000.png) ![](/tests/snapshots/nanosp/test_defi_approve/00001.png) ![](/tests/snapshots/nanosp/test_defi_approve/00002.png) ![](/tests/snapshots/nanosp/test_defi_approve/00003.png) ![](/tests/snapshots/nanosp/test_defi_approve/00004.png) ![](/tests/snapshots/nanosp/test_defi_approve/00005.png) ![](/tests/snapshots/nanosp/test_defi_approve/00006.png) ![](/tests/snapshots/nanosp/test_defi_approve/00007.png) ![](/tests/snapshots/nanosp/test_defi_approve/00008.png) ![](/tests/snapshots/nanosp/test_defi_approve/00009.png)

### DeFi - Transfer

Function: `transfer(address to, uint256 amount)`
Clearsigned:

- to: address to receive tokens
- amount: amount of tokens to transfer

![](/tests/snapshots/nanosp/test_defi_transfer/00000.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00001.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00002.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00003.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00004.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00005.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00006.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00007.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00008.png) ![](/tests/snapshots/nanosp/test_defi_transfer/00009.png)

### DeFi - TransferFrom

Function: `transferFrom(address from, address to, uint256 amount)`
Clearsigned:

- from: address to transfer tokens from
- to: address to receive tokens
- amount: amount of tokens to transfer

![](/tests/snapshots/nanosp/test_defi_transfer_from/00000.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00001.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00002.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00003.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00004.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00005.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00006.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00007.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00008.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00009.png) ![](/tests/snapshots/nanosp/test_defi_transfer_from/00010.png)
