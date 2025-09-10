from tests.utils import run_test, load_contract

contract_ocv2_ledger = load_contract(
    "0x5db5235b5c7e247488784986e58019fffd98fda4"
)

# Test from https://etherscan.io/tx/0x52e383bda403549d7184a7ce4f6514b93418d10c4921459332d3716aef860d17
def test_ocv2_stake(backend, navigator, test_name, wallet_addr):
    data = "0x3a4b66f1"
    run_test(
        contract_ocv2_ledger, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=10332000000000000000
    )