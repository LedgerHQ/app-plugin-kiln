# Technical Specification

## About

This documentation describes the smart contracts and functions supported by the kiln plugin.

## Smart Contracts

Smart contracts covered by the plugin:

| Network | Version | Smart Contract                        | Address                                      |
| ------- | ------- | ------------------------------------- | -------------------------------------------- |
| Mainnet | v1.2    | OCV1 - Kiln On-Chain v1               | `0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270` |
| Mainnet | v2.2    | OCV2-N20 - Kiln On-Chain v2 Native 20 | `0x5db5235b5c7e247488784986e58019fffd98fda4` |
| Mainnet | v2.2    | OCV2-EQ - Kiln On-Chain v2 Exit Queue | `0x8d6fd650500f82c7d978a440348e5a9b886943bf` |
| Mainnet | M2      | EL-SM - EigenLayer Strategy Manager   | `0x858646372cc42e1a627fce94aa7a7033e7cf075a` |
| Mainnet | M2      | EL-DM - EigenLayer Delegation Manager | `0x39053d51b77dc0d36036fc1fcc8cb819df8ef37a` |

## Functions

For the smart contracts implemented, the functions covered by the plugin are:

| Contract | Function                  | Selector     | Displayed Parameters                                                                                                                                                                                   |
| -------- | ------------------------- | ------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| OCV1     | deposit                   | `0xd0e30db0` | <table><tbody> <tr><td><code>uint tx.value</code></td></tr> </tbody></table>                                                                                                                           |
| OCV1     | withdraw                  | `0x0968f264` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | withdrawELFee             | `0xbf509bd4` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | withdrawCLFee             | `0x2ba03a79` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | batchWithdraw             | `0xe8a0c121` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | batchWithdrawELFee        | `0xef5e4682` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | batchWithdrawCLFee        | `0x0ffab6c2` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV1     | requestValidatorsExit     | `0xb6b06dec` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV2-N20 | stake                     | `0x3a4b66f1` | <table><tbody> <tr><td><code>uint tx.value</code></td></tr> </tbody></table>                                                                                                                           |
| OCV2-N20 | requestExit               | `0x721c6513` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV2-N20 | multiClaim                | `0xb7ba18c7` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| OCV2-EQ  | claim                     | `0xadcf1163` | <table><tbody> <tr><td>n/r</td></tr> </tbody></table>                                                                                                                                                  |
| EL-SM    | depositIntoStrategy       | `0xe7a050aa` | <table><tbody> <tr><td><code>address strategy</code></td></tr> <tr><td><code>address ERC20</code></td></tr><tr><td><code>deposited amount</code></td></tr></tbody></table>                             |
| EL-SM    | queueWithdrawals          | `0x0dd8dd02` | <table><tbody> <tr><td><code>queuedWithdrawalParams.strategies[]</code> as string using an internal mapping</td></tr><tr><td><code>queuedWithdrawalParams.withdrawer</code></td></tr> </tbody></table> |
| EL-SM    | completeQueuedWithdrawals | `0x33404396` | <table><tbody> <tr><td>strategies as string using an internal mapping</td></tr> <tr><td>redelegated or withdrawal action</tr></td> </tbody></table>                                                    |
| EL-DM    | delegateTo                | `0xeea9064b` | <table><tbody> <tr><td><code>address operator</code> or <code>"KILN"</code></td></tr> </tbody></table>                                                                                                 |
| EL-DM    | undelegate                | `0xda8be864` | <table><tbody> </tbody></table>                                                                                                                                                                        |
