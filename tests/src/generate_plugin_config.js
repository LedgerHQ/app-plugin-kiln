const pluginFolder = 'kiln';

function serialize_data(pluginName, contractAddress, selector) {
  const len = Buffer.from([pluginName.length]);
  const name = Buffer.from(pluginName);
  const address = Buffer.from(contractAddress.slice(2), 'hex');
  const methodid = Buffer.from(selector.slice(2), 'hex');

  // Taking .slice(2) to remove the "0x" prefix
  return Buffer.concat([len, name, address, methodid]);
}

function assert(condition, message) {
  if (!condition) {
    throw message || 'Assertion failed';
  }
}

// Function to generate the plugin configuration.
function generate_plugin_config() {
  var path = require('path');
  var fs = require('fs');

  // Define the base directory for the chain folders
  const baseDir = 'cal';
  const chains = fs.readdirSync(baseDir); // Read the chain folders (e.g., arbitrum, ethereum, etc.)

  // `contracts_to_abis` holds a mapping of contract addresses to ABIs
  let contracts_to_abis = {};

  for (let chain of chains) {
    let abiDir = path.join(baseDir, chain, 'abis'); // Construct the path for each chain's 'abis' folder
    if (fs.existsSync(abiDir)) {
      let files = fs.readdirSync(abiDir); // Read ABI files in the chain folder

      for (let abiFileName of files) {
        assert(
          abiFileName.toLocaleLowerCase() == abiFileName,
          `FAILED: File ${abiFileName} should be lower case.`
        );

        // Strip ".json" suffix
        let contractAddress = abiFileName.slice(
          0,
          abiFileName.length - '.json'.length
        );

        // Load abi
        let abi = require(path.join('..', abiDir, abiFileName));

        // Add it to contracts, using both chain and contractAddress as keys
        contracts_to_abis[`${chain}:${contractAddress}`] = abi;
      }
    }
  }

  let b2c = [];

  for (let chain of chains) {
    let b2cFilePath = path.join(baseDir, chain, 'b2c.json'); // Construct the path to the b2c.json file for each chain

    if (fs.existsSync(b2cFilePath)) {
      b2c.push(require(`../${b2cFilePath}`)); // Load and add the b2c.json file to the array
    } else {
      console.warn(`b2c.json not found for chain: ${chain}`);
    }
  }

  let res = {};

  // Place holder signature
  const PLACE_HOLDER_SIGNATURE =
    '3045022100f6e1a922c745e244fa3ed9a865491672808ef93f492ee0410861d748c5de201f0220160d6522499f3a84fa3e744b3b81e49e129e997b28495e58671a1169b16fa777';

  for (let chain of b2c) {
    // Iterate through contracts in b2c.json file
    for (let contract of chain['contracts']) {
      let methods_info = {};
      const contractAddress = contract['address'];
      assert(
        contractAddress.toLowerCase() == contractAddress,
        `FAILED: Contract Address ${contractAddress} should be lower case`
      );

      for (let [selector, values] of Object.entries(contract['selectors'])) {
        assert(
          selector.toLowerCase() == selector,
          `FAILED: Selector ${selector} should be lower case`
        );

        // Gather up the info needed for the end result
        const pluginName = values['plugin'];
        const serializedData = serialize_data(
          pluginName,
          contractAddress,
          selector
        );
        const signature = PLACE_HOLDER_SIGNATURE;

        methods_info[selector] = {
          plugin: pluginName,
          serialized_data: serializedData,
          signature: signature,
        };
      }
      // Add the abi to methods_info
      methods_info['abi'] = contracts_to_abis[contractAddress];
      // Add the methods_info to the end result
      res[contractAddress] = methods_info;
    }
  }

  assert(
    res.length == contracts_to_abis.length,
    `FAILED: ${res.length} contracts in b2c.json and ${contracts_to_abis.length} contracts in abis/ folder`
  );

  return res;
}

module.exports = {
  generate_plugin_config,
};
