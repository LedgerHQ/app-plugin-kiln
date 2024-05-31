import 'core-js/stable';
import 'regenerator-runtime/runtime';
import {
  waitForAppScreen,
  kilnJSON,
  zemu,
  genericTx,
  nano_models,
  SPECULOS_ADDRESS,
  txFromEtherscan,
} from './test.fixture';
import { ethers } from 'ethers';
import { parseEther, parseUnits } from 'ethers/lib/utils';
import { ledgerService } from '@ledgerhq/hw-app-eth';

const contractAddr = '0x858646372cc42e1a627fce94aa7a7033e7cf075a'; // strategy manager

const pluginName = 'Kiln';
const abi_path = `../cal/abis/${contractAddr}.json`;
const abi = require(abi_path);
const right_clicks = 6;

nano_models.forEach(function (model) {
  test(
    '[Nano ' + model.letter + '] LR Deposit Into Strategy Normal',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.depositIntoStrategy(
        '0x9d7eD45EE2E8FC5482fa2428f15C971e6369011d', // ETHx strat
        '0xA35b1B31Ce002FBF2058D22F30f95D405200A15b', // ETHx erc20
        '420000000000000'
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

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrDepositIntoStrategyNormal',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );

  test(
    '[Nano ' + model.letter + '] Deposit Into Strategy Unknown strategy',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.depositIntoStrategy(
        '0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270', // unknown strat
        '0xac3E018457B222d93114458476f3E3416Abbe38F', // sfrxETH erc20
        '420000000000000'
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

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrDepositIntoStrategyUnknownStrategy',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );

  test(
    '[Nano ' + model.letter + '] Deposit Into Strategy Unknown erc20',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.depositIntoStrategy(
        '0x298aFB19A105D59E74658C4C334Ff360BadE6dd2', // mETH strategy
        '0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270', // unknown erc20
        '420000000000000'
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

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots(
        '.',
        model.name + '_lrDepositIntoStrategyUnknownERC20',
        [right_clicks, 0]
      );
      await tx;
    }),
    30000
  );
});
