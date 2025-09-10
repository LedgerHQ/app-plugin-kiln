from tests.utils import run_test, load_contract

contract_ocv1 = load_contract(
    "0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270"
)

# Test from cast calldata 'withdrawELFee(bytes)' '0x933ad9491b62059dd065b560d256d8957a8c402cc6e8d8ee7290ae11e8f7329267a8811c397529dac52ae1342ba58c95'
def test_ocv1_withdraw_el(backend, navigator, test_name, wallet_addr):
    data = "0xbf509bd400000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000030933ad9491b62059dd065b560d256d8957a8c402cc6e8d8ee7290ae11e8f7329267a8811c397529dac52ae1342ba58c9500000000000000000000000000000000"
    run_test(
        contract_ocv1, 
        data, 
        backend, 
        navigator, 
        test_name, 
        wallet_addr,
        value=0
    )