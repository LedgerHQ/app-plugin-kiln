from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.BNB
vault_contract = load_contract(
    "0x4d1806c26a728f2e1b82b4549b9e074dbe5940b9"
)

# Test from https://bscscan.com/tx/0x3f883e32d86e754c6942a390bd812ce313480000729d8880d1ac86266935e302
def test_defi_withdraw(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xb460af9400000000000000000000000000000000000000000000001b1ae4d6e2ef5000000000000000000000000000004ca8502acdd4007ae21d0938ad6e25d6d47388590000000000000000000000004ca8502acdd4007ae21d0938ad6e25d6d4738859"
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