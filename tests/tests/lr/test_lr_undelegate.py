from tests.utils import run_test, load_contract

contract_delegation_manager = load_contract(
    "0x39053d51b77dc0d36036fc1fcc8cb819df8ef37a"
)

# Test from https://etherscan.io/tx/0xdbc21a553e3036bede09bde344d0671a72bb298c51c0e2336b978ec2fe55a4b5
def test_lr_undelegate(backend, navigator, test_name, wallet_addr):
    data = "0xda8be8640000000000000000000000006cca0299d7bf42afe88c0c3e39b97868601611d7"
    run_test(
        contract_delegation_manager, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr
    )