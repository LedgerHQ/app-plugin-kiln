from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.ETH
vault_contract = load_contract(
    "0x7DAEBa3F217614E409F85d3014D33923a6b03630"  # seUSDS vault from contracts.c
)

# Test generated using cast calldata "redeem(uint256,address,address)" 1000000000000000000 0x7DAEBa3F217614E409F85d3014D33923a6b03630 0x7DAEBa3F217614E409F85d3014D33923a6b03630
def test_defi_redeem(backend, navigator, test_name, wallet_addr):
    data = "0xba0876520000000000000000000000000000000000000000000000000de0b6b3a76400000000000000000000000000007daeba3f217614e409f85d3014d33923a6b036300000000000000000000000007daeba3f217614e409f85d3014d33923a6b03630"
    run_test(
        vault_contract, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        chain_id=CHAIN_ID,
        value=0
    )