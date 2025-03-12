from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.BNB
vault_contract = load_contract(
    "0x4d1806c26a728f2e1b82b4549b9e074dbe5940b9"
)

# Test from cast calldata "approve(address,uint256)" 0xAbC1234567890Def1234567890abcdef12345678 1234560000000000000000
def test_defi_approve(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x095ea7b3000000000000000000000000abc1234567890def1234567890abcdef12345678000000000000000000000000000000000000000000000042ecf6330552400000"
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