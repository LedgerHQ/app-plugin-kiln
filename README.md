# Badges

[![Code style check](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/lint-workflow.yml/badge.svg)](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/lint-workflow.yml)
[![Compilation & tests](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/ci-workflow.yml/badge.svg)](https://github.com/LedgerHQ/app-plugin-kiln/actions/workflows/ci-workflow.yml)

# app-plugin-kiln

![Kiln Logo](https://uploads-ssl.webflow.com/625db3caa8abd6c22d5f0ce3/6260572336892a801afd71e3_Kiln_Logo-Transparent-Dark.svg)

This plug-in is compatible with Nano S / X devices, it enables to
interact in a secure way with the Kiln On-Chain v1, v2 and EigenLayer Liquid Restaking contracts.

## Prerequisite

Clone the plugin to a new folder.

```shell
git clone https://github.com/LedgerHQ/app-plugin-kiln.git
```

Then in the same folder clone two more repositories, which is the plugin-tools and app-ethereum.

```shell
git clone https://github.com/LedgerHQ/plugin-tools.git                          #plugin-tools
git clone --recurse-submodules https://github.com/LedgerHQ/app-ethereum.git     #app-ethereum
```

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

Go to the plugin-tools folder and run the "./start" script.

```shell
cd plugin-tools  # go to plugin folder
./start.sh       # run the script start.sh
```

The script will build a docker image and attach a console.
When the docker image is running go to the "app-plugin-kiln" folder and build the ".elf" files.

```shell
cd app-plugin-kiln/tests        # go to the tests folder in app-plugin-kiln
./build_local_test_elfs.sh      # run the script build_local_test_elfs.sh
```

### Tests

The tests are in the `tests/src` folder. They are written in javascript and use the `@ledgerhq/hw-app-eth` library to simulate the device.

The tests are run using the `yarn test` command.

The output of the tests is a series of screenshots stored in the `tests/snapshots-tmp` folder that are compared to the expected screenshots in the `tests/snapshots` folder. The latter are stored in the git repository to make sure we expect the correct outputs.

To test the plugin go to the tests folder from the "app-plugin-kiln/tests" and run the script "test"

```shell
cd app-plugin-kiln/tests        # go to the tests folder in app-plugin-kiln
yarn                            # install dependencies
yarn test                       # run the script test
```

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

## Clearsigning

### KILN ON-CHAIN v1 - Deposit

Function: `deposit()`
Clearsigned:

- tx.value

![](/tests/snapshots/nanos_deposit/00000.png) ![](/tests/snapshots/nanos_deposit/00001.png) ![](/tests/snapshots/nanos_deposit/00002.png) ![](/tests/snapshots/nanos_deposit/00003.png) ![](/tests/snapshots/nanos_deposit/00004.png) ![](/tests/snapshots/nanos_deposit/00005.png)

![](/tests/snapshots/nanox_deposit/00000.png) ![](/tests/snapshots/nanox_deposit/00001.png) ![](/tests/snapshots/nanox_deposit/00002.png) ![](/tests/snapshots/nanox_deposit/00003.png) ![](/tests/snapshots/nanox_deposit/00004.png) ![](/tests/snapshots/nanox_deposit/00005.png)

### KILN ON-CHAIN v1 - Withdraw

Function `withdraw(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_withdraw/00000.png) ![](/tests/snapshots/nanos_withdraw/00001.png) ![](/tests/snapshots/nanos_withdraw/00002.png) ![](/tests/snapshots/nanos_withdraw/00003.png) ![](/tests/snapshots/nanos_withdraw/00004.png) ![](/tests/snapshots/nanos_withdraw/00005.png)

![](/tests/snapshots/nanox_withdraw/00000.png) ![](/tests/snapshots/nanox_withdraw/00001.png) ![](/tests/snapshots/nanox_withdraw/00002.png) ![](/tests/snapshots/nanox_withdraw/00003.png) ![](/tests/snapshots/nanox_withdraw/00004.png) ![](/tests/snapshots/nanox_withdraw/00005.png)

### KILN ON-CHAIN v1 - Withdraw EL

Function `withdrawEL(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_withdrawEL/00000.png) ![](/tests/snapshots/nanos_withdrawEL/00001.png) ![](/tests/snapshots/nanos_withdrawEL/00002.png) ![](/tests/snapshots/nanos_withdrawEL/00003.png) ![](/tests/snapshots/nanos_withdrawEL/00004.png) ![](/tests/snapshots/nanos_withdrawEL/00005.png)

![](/tests/snapshots/nanox_withdrawEL/00000.png) ![](/tests/snapshots/nanox_withdrawEL/00001.png) ![](/tests/snapshots/nanox_withdrawEL/00002.png) ![](/tests/snapshots/nanox_withdrawEL/00003.png) ![](/tests/snapshots/nanox_withdrawEL/00004.png) ![](/tests/snapshots/nanox_withdrawEL/00005.png)

### KILN ON-CHAIN v1 - Withdraw CL

Function `withdrawCL(bytes validatorPubkey)`
Clearsigned:

- none as BLS key is not digest to display on device and only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_withdrawCL/00000.png) ![](/tests/snapshots/nanos_withdrawCL/00001.png) ![](/tests/snapshots/nanos_withdrawCL/00002.png) ![](/tests/snapshots/nanos_withdrawCL/00003.png) ![](/tests/snapshots/nanos_withdrawCL/00004.png) ![](/tests/snapshots/nanos_withdrawCL/00005.png)

![](/tests/snapshots/nanox_withdrawCL/00000.png) ![](/tests/snapshots/nanox_withdrawCL/00001.png) ![](/tests/snapshots/nanox_withdrawCL/00002.png) ![](/tests/snapshots/nanox_withdrawCL/00003.png) ![](/tests/snapshots/nanox_withdrawCL/00004.png) ![](/tests/snapshots/nanox_withdrawCL/00005.png)

### KILN ON-CHAIN v1 - Batch Withdraw

Function `batchWithdraw(bytes validatorPubkeys)`

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_batchWithdraw/00000.png) ![](/tests/snapshots/nanos_batchWithdraw/00001.png) ![](/tests/snapshots/nanos_batchWithdraw/00002.png) ![](/tests/snapshots/nanos_batchWithdraw/00003.png) ![](/tests/snapshots/nanos_batchWithdraw/00004.png) ![](/tests/snapshots/nanos_batchWithdraw/00005.png)

![](/tests/snapshots/nanox_batchWithdraw/00000.png) ![](/tests/snapshots/nanox_batchWithdraw/00001.png) ![](/tests/snapshots/nanox_batchWithdraw/00002.png) ![](/tests/snapshots/nanox_batchWithdraw/00003.png) ![](/tests/snapshots/nanox_batchWithdraw/00004.png) ![](/tests/snapshots/nanox_batchWithdraw/00005.png)

### KILN ON-CHAIN v1 - Batch Withdraw EL

Function `batchWithdrawEL(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_batchWithdrawEL/00000.png) ![](/tests/snapshots/nanos_batchWithdrawEL/00001.png) ![](/tests/snapshots/nanos_batchWithdrawEL/00002.png) ![](/tests/snapshots/nanos_batchWithdrawEL/00003.png) ![](/tests/snapshots/nanos_batchWithdrawEL/00004.png) ![](/tests/snapshots/nanos_batchWithdrawEL/00005.png)

![](/tests/snapshots/nanox_batchWithdrawEL/00000.png) ![](/tests/snapshots/nanox_batchWithdrawEL/00001.png) ![](/tests/snapshots/nanox_batchWithdrawEL/00002.png) ![](/tests/snapshots/nanox_batchWithdrawEL/00003.png) ![](/tests/snapshots/nanox_batchWithdrawEL/00004.png) ![](/tests/snapshots/nanox_batchWithdrawEL/00005.png)

### KILN ON-CHAIN v1 - Batch WithdrawCL

Function `batchWithdrawCL(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can withdraw on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_batchWithdrawCL/00000.png) ![](/tests/snapshots/nanos_batchWithdrawCL/00001.png) ![](/tests/snapshots/nanos_batchWithdrawCL/00002.png) ![](/tests/snapshots/nanos_batchWithdrawCL/00003.png) ![](/tests/snapshots/nanos_batchWithdrawCL/00004.png) ![](/tests/snapshots/nanos_batchWithdrawCL/00005.png)

![](/tests/snapshots/nanox_batchWithdrawCL/00000.png) ![](/tests/snapshots/nanox_batchWithdrawCL/00001.png) ![](/tests/snapshots/nanox_batchWithdrawCL/00002.png) ![](/tests/snapshots/nanox_batchWithdrawCL/00003.png) ![](/tests/snapshots/nanox_batchWithdrawCL/00004.png) ![](/tests/snapshots/nanox_batchWithdrawCL/00005.png)

### KILN ON-CHAIN v1 - Request Exits

Function `requestValidatorsExit(bytes validatorPubkeys)`
Clearsigned:

- none as validatorPubkeys can represent many BLS keys that we can't store on the device. Also only owner of the validator can request exit on its behalf so no attack vector possible.

![](/tests/snapshots/nanos_requestValidatorsExit/00000.png) ![](/tests/snapshots/nanos_requestValidatorsExit/00001.png) ![](/tests/snapshots/nanos_requestValidatorsExit/00002.png) ![](/tests/snapshots/nanos_requestValidatorsExit/00003.png) ![](/tests/snapshots/nanos_requestValidatorsExit/00004.png) ![](/tests/snapshots/nanos_requestValidatorsExit/00005.png)

![](/tests/snapshots/nanox_requestValidatorsExit/00000.png) ![](/tests/snapshots/nanox_requestValidatorsExit/00001.png) ![](/tests/snapshots/nanox_requestValidatorsExit/00002.png) ![](/tests/snapshots/nanox_requestValidatorsExit/00003.png) ![](/tests/snapshots/nanox_requestValidatorsExit/00004.png) ![](/tests/snapshots/nanox_requestValidatorsExit/00005.png)

### KILN ON-CHAIN v2 - Stake

Function: `stake()`
Clearsigned:

- tx.value

![](/tests/snapshots/nanos_stakev2/00000.png) ![](/tests/snapshots/nanos_stakev2/00001.png) ![](/tests/snapshots/nanos_stakev2/00002.png) ![](/tests/snapshots/nanos_stakev2/00003.png) ![](/tests/snapshots/nanos_stakev2/00004.png) ![](/tests/snapshots/nanos_stakev2/00005.png)

![](/tests/snapshots/nanox_stakev2/00000.png) ![](/tests/snapshots/nanox_stakev2/00001.png) ![](/tests/snapshots/nanox_stakev2/00002.png) ![](/tests/snapshots/nanox_stakev2/00003.png) ![](/tests/snapshots/nanox_stakev2/00004.png) ![](/tests/snapshots/nanox_stakev2/00005.png)

### KILN ON-CHAIN v2 - RequestExit

Function: `requestExit(uint256 shares)`
Clearsigned:

- none as shares is not 1:1 with ETH value, it would confuse users

![](/tests/snapshots/nanos_requestExitv2/00000.png) ![](/tests/snapshots/nanos_requestExitv2/00001.png) ![](/tests/snapshots/nanos_requestExitv2/00002.png) ![](/tests/snapshots/nanos_requestExitv2/00003.png) ![](/tests/snapshots/nanos_requestExitv2/00004.png) ![](/tests/snapshots/nanos_requestExitv2/00005.png)

![](/tests/snapshots/nanox_requestExitv2/00000.png) ![](/tests/snapshots/nanox_requestExitv2/00001.png) ![](/tests/snapshots/nanox_requestExitv2/00002.png) ![](/tests/snapshots/nanox_requestExitv2/00003.png) ![](/tests/snapshots/nanox_requestExitv2/00004.png) ![](/tests/snapshots/nanox_requestExitv2/00005.png)

### KILN ON-CHAIN v2 - MultiClaim

Function: `multiClaim(address[] exitQueues, uint256[][] ticketIds, uint32[][] casksIds)`
Clearsigned:

- none

![](/tests/snapshots/nanos_multiClaimv2/00000.png) ![](/tests/snapshots/nanos_multiClaimv2/00001.png) ![](/tests/snapshots/nanos_multiClaimv2/00002.png) ![](/tests/snapshots/nanos_multiClaimv2/00003.png) ![](/tests/snapshots/nanos_multiClaimv2/00004.png) ![](/tests/snapshots/nanos_multiClaimv2/00005.png)

![](/tests/snapshots/nanox_multiClaimv2/00000.png) ![](/tests/snapshots/nanox_multiClaimv2/00001.png) ![](/tests/snapshots/nanox_multiClaimv2/00002.png) ![](/tests/snapshots/nanox_multiClaimv2/00003.png) ![](/tests/snapshots/nanox_multiClaimv2/00004.png) ![](/tests/snapshots/nanox_multiClaimv2/00005.png)

### KILN ON-CHAIN v2 - Claim

Function: `claim(uint256[] ticketIds, uint32[] caskIds, uint16 maxClaimDepth)`
Clearsigned:

- none

![](/tests/snapshots/nanos_claimv2/00000.png) ![](/tests/snapshots/nanos_claimv2/00001.png) ![](/tests/snapshots/nanos_claimv2/00002.png) ![](/tests/snapshots/nanos_claimv2/00003.png) ![](/tests/snapshots/nanos_claimv2/00004.png) ![](/tests/snapshots/nanos_claimv2/00005.png)

![](/tests/snapshots/nanox_claimv2/00000.png) ![](/tests/snapshots/nanox_claimv2/00001.png) ![](/tests/snapshots/nanox_claimv2/00002.png) ![](/tests/snapshots/nanox_claimv2/00003.png) ![](/tests/snapshots/nanox_claimv2/00004.png) ![](/tests/snapshots/nanox_claimv2/00005.png)

### EigenLayer Strategy Manager - LR Deposit Into Strategy

Function: `depositIntoStrategy(address strategy, address erc20, uint256 amount)`
Clearsigned:

- strategy: using mapping in `contracts.c`, it is either recognized or displayed as "UNKOWN"
- erc20: using mapping in `contracts.c`, it is either recognized or displayed as "UNKOWN"
- amount

![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00000.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00001.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00002.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00003.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00004.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00005.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00006.png) ![](/tests/snapshots/nanos_lrDepositIntoStrategyNormal/00007.png)

![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00000.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00001.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00002.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00003.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00004.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00005.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00006.png) ![](/tests/snapshots/nanox_lrDepositIntoStrategyNormal/00007.png)

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

![](/tests/snapshots/nanos_delegate_to_is_kiln/00000.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00001.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00002.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00003.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00004.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00005.png) ![](/tests/snapshots/nanos_delegate_to_is_kiln/00006.png)

![](/tests/snapshots/nanox_delegate_to_is_kiln/00000.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00001.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00002.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00003.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00004.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00005.png) ![](/tests/snapshots/nanox_delegate_to_is_kiln/00006.png)

### EigenLayer Delegation Manager - Undelegate

Function: `undelegate(address staker)`
Clearsigned:

- none as there is a staker == msg.sender requirement in the contract

![](/tests/snapshots/nanos_lr_undelegate/00000.png) ![](/tests/snapshots/nanos_lr_undelegate/00001.png) ![](/tests/snapshots/nanos_lr_undelegate/00002.png) ![](/tests/snapshots/nanos_lr_undelegate/00003.png) ![](/tests/snapshots/nanos_lr_undelegate/00004.png) ![](/tests/snapshots/nanos_lr_undelegate/00005.png)

![](/tests/snapshots/nanox_lr_undelegate/00000.png) ![](/tests/snapshots/nanox_lr_undelegate/00001.png) ![](/tests/snapshots/nanox_lr_undelegate/00002.png) ![](/tests/snapshots/nanox_lr_undelegate/00003.png) ![](/tests/snapshots/nanox_lr_undelegate/00004.png) ![](/tests/snapshots/nanox_lr_undelegate/00005.png)

### EigenLayer Strategy Manager - Queue Withdrawals

Function:

```solidity
struct QueuedWithdrawalParams {
    address[] strategies;
    uint256[] shares;
    address withdrawer;
}

function queueWithdrawals(
    QueuedWithdrawalParams[] queuedWithdrawalParams
)
```

Clearsigned:

- strategies: using mapping in `contracts.c`, they are either recognized or displayed as "UNKOWN"
- withdrawer

Shares are confusing for users and would be hard to store properly on the device for display.

![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00000.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00001.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00002.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00003.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00004.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00005.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00006.png) ![](/tests/snapshots/nanos_lrQueueWithdrawals_1_2_dimension/00007.png)

![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00000.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00001.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00002.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00003.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00004.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00005.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00006.png) ![](/tests/snapshots/nanox_lrQueueWithdrawals_1_2_dimension/00007.png)

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

![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00000.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00001.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00002.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00003.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00004.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00005.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00006.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00007.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00008.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00009.png) ![](/tests/snapshots/nanos_lrCompleteQueuedWithdrawals/00010.png)

![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00000.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00001.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00002.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00003.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00004.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00005.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00006.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00007.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00008.png) ![](/tests/snapshots/nanox_lrCompleteQueuedWithdrawals/00009.png)
