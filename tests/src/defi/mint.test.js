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
} from '../test.fixture';
import { ethers } from 'ethers';
import { parseEther, parseUnits } from 'ethers/lib/utils';
import { ledgerService } from '@ledgerhq/hw-app-eth';

const contractAddr = '0xca8f5dbc4c90678763b291217e6dddfca00341d0';

const pluginName = 'Kiln';
const abi_path = `../../cal/arbitrum/abis/${contractAddr}.json`;
const abi = require(abi_path);

nano_models.forEach(function (model) {
  test(
    '[Nano ' + model.letter + '] DEFI Mint ERC20',
    zemu(model, async (sim, eth) => {
      const contract = new ethers.Contract(contractAddr, abi);

      const { data } = await contract.populateTransaction.mint(
        42000000,
        '0x5db5235b5c7e247488784986e58019fffd98fda4'
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
      const right_clicks = 5;

      await waitForAppScreen(sim);
      await sim.navigateAndCompareSnapshots('.', model.name + '_defi_mint', [
        right_clicks,
        0,
      ]);
      await tx;
    }),
    30000
  );
});
