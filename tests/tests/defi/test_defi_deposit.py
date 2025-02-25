from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.BNB
vault_contract = load_contract(
    "0x4d1806c26a728f2e1b82b4549b9e074dbe5940b9"
)

# Test from https://bscscan.com/tx/0xb3d13b3e7cb911d700b4bb5d602a02ad5ebefea822096472bf913c764005cefa
def test_defi_deposit(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x6e553f650000000000000000000000000000000000000000000001f533784b4957a3a000000000000000000000000000c005d9619869ef16884e713b0e739b2ed4a62aab"
    run_test(
        vault_contract, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr,
        chain_id=CHAIN_ID,
        value=0
    )