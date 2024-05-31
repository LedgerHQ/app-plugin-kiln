import 'core-js/stable';
import 'regenerator-runtime/runtime';
import { waitForAppScreen, zemu, genericTx, nano_models } from './test.fixture';
import { ethers } from 'ethers';
import { parseEther } from 'ethers/lib/utils';
import { ledgerService } from '@ledgerhq/hw-app-eth';

const contractAddr = '0x858646372cc42e1a627fce94aa7a7033e7cf075a'; // strategy manager

const abi_path = `../cal/abis/${contractAddr}.json`;
const abi = require(abi_path);

nano_models.forEach(function (model) {
  test(
    '[Nano ' + model.letter + '] LR Complete Queued Withdrawals 2',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);
      //
      //  struct Withdrawal {
      // 	    address staker;
      // 	    address delegatedTo;
      // 	    address withdrawer;
      // 	    uint256 nonce;
      // 	    uint32 startBlock;
      // 	    address[] strategies;
      // 	    uint256[] shares;
      // }
      //
      // function completeQueuedWithdrawals(
      // 	    Withdrawal[] calldata withdrawals,
      // 	    address[][] calldata tokens,
      // 	    uint256[] calldata middlewareTimesIndexes,
      // 	    bool[] calldata receiveAsTokens
      // ) external
      //
      const { data } =
        await contract.populateTransaction.completeQueuedWithdrawals(
          [
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 0,
              startBlock: 0,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
                '0x93c4b944D05dfe6df7645A86cd2206016c51564D', // stETH
              ],
              shares: [parseEther('0.1'), parseEther('0.2')],
            },
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 1,
              startBlock: 1,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
                '0x93c4b944D05dfe6df7645A86cd2206016c51564D', // stETH
              ],
              shares: [parseEther('2'), parseEther('3')],
            },
          ],
          [
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
              '0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84', // stETH
            ],
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
              '0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84', // stETH
            ],
          ],
          [0, 0],
          [false, false]
        );

      let unsignedTx = genericTx;

      unsignedTx.to = contractAddr;
      unsignedTx.data = data;
      unsignedTx.value = parseEther('0');

      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);
      const resolution = await ledgerService.resolveTransaction(
        serializedTx,
        eth.loadConfig,
        {
          externalPlugins: true,
        }
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx, resolution);
      const right_clicks = model.letter === 'S' ? 12 : 9;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrCompleteQueuedWithdrawals',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );

  test(
    '[Nano ' + model.letter + '] LR Complete Queued Withdrawals Redelegate 1',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);
      //
      //  struct Withdrawal {
      // 	    address staker;
      // 	    address delegatedTo;
      // 	    address withdrawer;
      // 	    uint256 nonce;
      // 	    uint32 startBlock;
      // 	    address[] strategies;
      // 	    uint256[] shares;
      // }
      //
      // function completeQueuedWithdrawals(
      // 	    Withdrawal[] calldata withdrawals,
      // 	    address[][] calldata tokens,
      // 	    uint256[] calldata middlewareTimesIndexes,
      // 	    bool[] calldata receiveAsTokens
      // ) external
      //
      const { data } =
        await contract.populateTransaction.completeQueuedWithdrawals(
          [
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 0,
              startBlock: 0,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
              ],
              shares: [parseEther('0.1')],
            },
          ],
          [
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
            ],
          ],
          [0],
          [false]
        );

      let unsignedTx = genericTx;

      unsignedTx.to = contractAddr;
      unsignedTx.data = data;
      unsignedTx.value = parseEther('0');

      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);
      const resolution = await ledgerService.resolveTransaction(
        serializedTx,
        eth.loadConfig,
        {
          externalPlugins: true,
        }
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx, resolution);
      const right_clicks = model.letter === 'S' ? 9 : 6;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrCompleteQueuedWithdrawals_redelegate_1',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );

  test(
    '[Nano ' + model.letter + '] LR Complete Queued Withdrawals 1',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);
      //
      //  struct Withdrawal {
      // 	    address staker;
      // 	    address delegatedTo;
      // 	    address withdrawer;
      // 	    uint256 nonce;
      // 	    uint32 startBlock;
      // 	    address[] strategies;
      // 	    uint256[] shares;
      // }
      //
      // function completeQueuedWithdrawals(
      // 	    Withdrawal[] calldata withdrawals,
      // 	    address[][] calldata tokens,
      // 	    uint256[] calldata middlewareTimesIndexes,
      // 	    bool[] calldata receiveAsTokens
      // ) external
      //
      const { data } =
        await contract.populateTransaction.completeQueuedWithdrawals(
          [
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 0,
              startBlock: 0,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
              ],
              shares: [parseEther('0.1')],
            },
          ],
          [
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
            ],
          ],
          [0],
          [true]
        );

      let unsignedTx = genericTx;

      unsignedTx.to = contractAddr;
      unsignedTx.data = data;
      unsignedTx.value = parseEther('0');

      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);
      const resolution = await ledgerService.resolveTransaction(
        serializedTx,
        eth.loadConfig,
        {
          externalPlugins: true,
        }
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx, resolution);
      const right_clicks = model.letter === 'S' ? 9 : 6;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrCompleteQueuedWithdrawals_1',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );

  test(
    '[Nano ' + model.letter + '] LR Complete Queued Withdrawals 8',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);
      //
      //  struct Withdrawal {
      // 	    address staker;
      // 	    address delegatedTo;
      // 	    address withdrawer;
      // 	    uint256 nonce;
      // 	    uint32 startBlock;
      // 	    address[] strategies;
      // 	    uint256[] shares;
      // }
      //
      // function completeQueuedWithdrawals(
      // 	    Withdrawal[] calldata withdrawals,
      // 	    address[][] calldata tokens,
      // 	    uint256[] calldata middlewareTimesIndexes,
      // 	    bool[] calldata receiveAsTokens
      // ) external
      //
      const { data } =
        await contract.populateTransaction.completeQueuedWithdrawals(
          [
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 0,
              startBlock: 0,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
                '0x93c4b944D05dfe6df7645A86cd2206016c51564D', // stETH
                '0x57ba429517c3473B6d34CA9aCd56c0e735b94c02', // osETH
                '0x7CA911E83dabf90C90dD3De5411a10F1A6112184', // wBETH
              ],
              shares: [
                parseEther('0.1'),
                parseEther('0.2'),
                parseEther('0.3'),
                parseEther('0.4'),
              ],
            },
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 1,
              startBlock: 1,
              strategies: [
                '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
                '0x93c4b944D05dfe6df7645A86cd2206016c51564D', // stETH
                '0xae78736Cd615f374D3085123A210448E74Fc6393', // UNKNOWN
              ],
              shares: [parseEther('2'), parseEther('3'), parseEther('4')],
            },
            {
              staker: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              delegatedTo: '0x1f8C8b1d78d01bCc42ebdd34Fae60181bD697662',
              withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
              nonce: 2,
              startBlock: 1,
              strategies: [
                '0xa4C637e0F704745D182e4D38cAb7E7485321d059', // OETH
              ],
              shares: [parseEther('17')],
            },
          ],
          [
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
              '0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84', // stETH
              '0xf1C9acDc66974dFB6dEcB12aA385b9cD01190E38', // osETH
              '0xa2E3356610840701BDf5611a53974510Ae27E2e1', // wBETH
            ],
            [
              '0xBe9895146f7AF43049ca1c1AE358B0541Ea49704', // cbETH
              '0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84', // stETH
              '0xae78736Cd615f374D3085123A210448E74Fc6393', // rETH
            ],
            [
              '0x856c4Efb76C1D1AE02e20CEB03A2A6a08b0b8dC3', // OETH
            ],
          ],
          [0, 0, 1],
          [false, true, true]
        );

      let unsignedTx = genericTx;

      unsignedTx.to = contractAddr;
      unsignedTx.data = data;
      unsignedTx.value = parseEther('0');

      const serializedTx = ethers.utils
        .serializeTransaction(unsignedTx)
        .slice(2);
      const resolution = await ledgerService.resolveTransaction(
        serializedTx,
        eth.loadConfig,
        {
          externalPlugins: true,
        }
      );
      const tx = eth.signTransaction("44'/60'/0'/0", serializedTx, resolution);
      const right_clicks = model.letter === 'S' ? 16 : 13;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrCompleteQueuedWithdrawals_8',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );
});
