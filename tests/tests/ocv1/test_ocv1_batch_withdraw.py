from tests.utils import run_test, load_contract

contract_ocv1 = load_contract(
    "0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270"
)

# Test from https://etherscan.io/tx/0x6836f835e605866a8998404f485b478469443aa5df5c39de96a9e94e8404a5ef
def test_ocv1_batch_withdraw(backend, navigator, test_name, wallet_addr):
    data = "0xe8a0c12100000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000030ad7bb20673d4ea3ff636803a2bb17d76ba2aed89bc51eca37e644a40345d51278742d68d6506ce9917ec6d851dba0c9b00000000000000000000000000000000"
    run_test(
        contract_ocv1, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=0
    )