from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.BNB
vault_contract = load_contract(
    "0x4d1806c26a728f2e1b82b4549b9e074dbe5940b9"
)

# Test from https://bscscan.com/tx/0xc1a188556fb88e0fcb9f961648f94679f622ce86518d564fdb18440ef6de52e8
def test_defi_redeem(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xba08765200000000000000000000000000000000000000000000000ab299f7f182cfe2050000000000000000000000000fe00764bec32ffea3f4e4e057124c718c5b94280000000000000000000000000fe00764bec32ffea3f4e4e057124c718c5b9428"
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