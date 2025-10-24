from tests.utils import run_test, load_contract

contract_ocv2_ledger = load_contract(
    "0x5db5235b5c7e247488784986e58019fffd98fda4"
)

# Test from https://etherscan.io/tx/0x17bbd5ab85b7823f1aa27ea0a0e63a9f9608e9e2600bd7bb8fee86a447da5012
def test_ocv2_request_exit(backend, navigator, test_name, wallet_addr):
    data = "0x721c6513000000000000000000000000000000000000000000000000d48bbfdce0b15e51"
    run_test(
        contract_ocv2_ledger, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=0
    )