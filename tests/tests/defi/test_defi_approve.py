from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.ETH
vault_contract = load_contract(
    "0x7DAEBa3F217614E409F85d3014D33923a6b03630"  # seUSDS vault from contracts.c
)

# Test generated using cast calldata "approve(address,uint256)" 0x7DAEBa3F217614E409F85d3014D33923a6b03630 1000000000000000000
def test_defi_approve(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x095ea7b30000000000000000000000007daeba3f217614e409f85d3014d33923a6b036300000000000000000000000000000000000000000000000000de0b6b3a7640000"
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