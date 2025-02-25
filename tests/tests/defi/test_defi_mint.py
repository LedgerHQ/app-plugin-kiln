from tests.utils import run_test, load_contract
from eth_typing import ChainId

CHAIN_ID = ChainId.BNB
vault_contract = load_contract(
    "0x4d1806c26a728f2e1b82b4549b9e074dbe5940b9"
)

# Test from cast calldata "mint(uint256,address)" 123456 0xAbC1234567890Def1234567890abcdef12345678   
def test_defi_mint(backend, firmware, navigator, test_name, wallet_addr):
    data = "0x94bf804d000000000000000000000000000000000000000000000000000000000001e240000000000000000000000000abc1234567890def1234567890abcdef12345678"
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