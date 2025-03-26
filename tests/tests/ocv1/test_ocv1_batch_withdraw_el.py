from tests.utils import run_test, load_contract

contract_ocv1 = load_contract(
    "0x1e68238ce926dec62b3fbc99ab06eb1d85ce0270"
)

# Test from https://etherscan.io/tx/0xffe36cfccaf003b75394531f2ae7e923ebea83faf329c1efa05922eba43d615c
def test_ocv1_batch_withdraw_el(backend, firmware, navigator, test_name, wallet_addr):
    data = "0xef5e468200000000000000000000000000000000000000000000000000000000000000200000000000000000000000000000000000000000000000000000000000000060b45bf45155613ee543ce4e85f1e4e3ef46f2b99e2fb58f20f67a9ab966486c3e42b1f43c3a79e825818b73eb6b1d84cd91a1f075acf6ac8194b5ef075a038a2fe9ba7ac7959333286c9631a10b9de84b91d4ac8687c855bcfddecf1060966e5a"
    run_test(
        contract_ocv1, 
        data, 
        backend, 
        firmware, 
        navigator, 
        test_name, 
        wallet_addr,
        value=0
    )