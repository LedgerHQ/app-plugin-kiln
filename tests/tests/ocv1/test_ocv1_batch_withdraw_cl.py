from tests.utils import run_test, load_contract

contract_ocv1 = load_contract(
    "0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270"
)

# Test from https://etherscan.io/tx/0x1c5144b2149c92b5fd55daebae877f21a0b5759011d178b62f6dc5d0db460ad3
def test_ocv1_batch_withdraw_cl(backend, navigator, test_name, wallet_addr):
    data = "0x0ffab6c200000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000030825ec28337af71359c66f853a9e0bfd47642af9a42e1f4520f20c0267181fea25cf9db043247ce4283d679c781aa09a700000000000000000000000000000000"
    run_test(
        contract_ocv1, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=0
    )