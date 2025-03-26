from tests.utils import run_test, load_contract

contract_strategy_manager = load_contract(
    "0x858646372cc42e1a627fce94aa7a7033e7cf075a"
)

# Test from https://etherscan.io/tx/0xef04d0d081eb61318f79ec49c77dd4699b723e92bedbe2c3766d6a39764a3557
def test_lr_deposit_into_strategy(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xe7a050aa00000000000000000000000093c4b944d05dfe6df7645a86cd2206016c51564d000000000000000000000000ae7ab96520de3a18e5e111b5eaab095312d7fe8400000000000000000000000000000000000000000000000000005af3107a4000"
    run_test(
        contract_strategy_manager, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr
    )