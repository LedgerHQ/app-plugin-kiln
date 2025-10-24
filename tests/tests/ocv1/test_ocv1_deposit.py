from tests.utils import run_test, load_contract

contract_ocv1 = load_contract(
    "0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270"
)

# Test from https://etherscan.io/tx/0xea17de5f3cc81f6638c714a7be936e37d652c080c66f333df765fbe0760eaec2
def test_ocv1_deposit(backend, navigator, test_name, wallet_addr):
    data = "0xd0e30db0"
    run_test(
        contract_ocv1, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=32000000000000000000
    )