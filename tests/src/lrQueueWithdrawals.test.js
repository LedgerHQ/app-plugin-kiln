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
    '[Nano ' + model.letter + '] LR Queue Withdrawals 1-2 dimension',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.queueWithdrawals([
        {
          strategies: [
            '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
            '0x13760F50a9d7377e4F20CB8CF9e4c26586c658ff', // ankrETH
          ],
          shares: [parseEther('0.1'), parseEther('0.2')],
          withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
        },
      ]);

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
      const right_clicks = model.letter === 'S' ? 9 : 7;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrQueueWithdrawals_1_2_dimension',
        [right_clicks, 0]
      );
      await tx;
    }),
    10000
  );

  test(
    '[Nano ' + model.letter + '] LR Queue Withdrawals 2-3 dimension',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.queueWithdrawals([
        {
          strategies: [
            '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
            '0x13760F50a9d7377e4F20CB8CF9e4c26586c658ff', // ankrETH
            '0x57ba429517c3473B6d34CA9aCd56c0e735b94c02', // osETH
          ],
          shares: [parseEther('0.1'), parseEther('0.2'), parseEther('0.3')],
          withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
        },
        {
          strategies: [
            '0x298aFB19A105D59E74658C4C334Ff360BadE6dd2', // mETH
            '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
            '0xa4C637e0F704745D182e4D38cAb7E7485321d059', // OETH
          ],
          shares: [parseEther('0.5'), parseEther('0.1'), parseEther('0.2')],
          withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
        },
      ]);

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
      const right_clicks = model.letter === 'S' ? 13 : 11;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrQueueWithdrawals_2_3_dimension',
        [right_clicks, 0]
      );
      await tx;
    }),
    10000
  );

  test(
    '[Nano ' + model.letter + '] LR Queue Withdrawals unknown strategy',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.queueWithdrawals([
        {
          strategies: [
            '0x54945180dB7943c0ed0FEE7EdaB2Bd24620256bc', // cbETH
            '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045', // unkwown strategy
            '0x57ba429517c3473B6d34CA9aCd56c0e735b94c02', // osETH
          ],
          shares: [parseEther('0.1'), parseEther('0.2'), parseEther('0.3')],
          withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
        },
        {
          strategies: [
            '0x298aFB19A105D59E74658C4C334Ff360BadE6dd2', // mETH
            '0xa4C637e0F704745D182e4D38cAb7E7485321d059', // OETH
            '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045', // unkwown strategy
          ],
          shares: [parseEther('0.5'), parseEther('0.1'), parseEther('0.2')],
          withdrawer: '0xd8dA6BF26964aF9D7eEd9e03E53415D37aA96045',
        },
      ]);

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
      const right_clicks = model.letter === 'S' ? 13 : 11;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrQueueWithdrawals_2_3_dimension_with_unknowns',
        [right_clicks, 0]
      );
      await tx;
    }),
    10000
  );
});
