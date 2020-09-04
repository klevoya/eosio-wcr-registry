

## Name: Fake Notification Receipt

### Unique Identifier: EOSIO-WCR-107

### Vulnerability Rating: Critical

### Relationship: [CWE-1021: Improper Restriction of Rendered UI Layers or Frames (Clickjacking)](https://cwe.mitre.org/data/definitions/1021.html)

## Background

### Summary

A vulnerable smart contract does not check the **to** or **from** parameters of the transfer notification to check if it is involved in the transfer, allowing an attacker to mislead the victim contract into believing it has received tokens through a fake forwarded incoming transfer notification.

## Vulnerability

A malicious contract sends real tokens (tokens from the actual desired token contract) to another one of their own malicious contract accounts and reflects the notification to a victim contract.

### Sample Code

```cpp
[[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (from == get_self())
      return;
    // getting here does not imply that to == get_self() because of fake notifications

    // check code paramter (get_first_receiver) for correct token contract
    check(get_first_receiver() == EOS_SYMBOL.get_contract(), "fake EOS not accepted");
    check(quantity.amount > 0, "must be positive");
    check(quantity.symbol == EOS_SYMBOL.get_symbol(), "EOS only!");
    // ...
}
```

### Test Case

> [Test for Fake Notification Receipt by Klevoya™](../test_cases/wcr-107/)

## Remediation

Check the `from` and `to` parameters of the transfer notification to see if the contract is receiving or sending tokens.

### Sample Code

```cpp
  [[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (from == get_self())
      return;
    check(to == get_self(), "contract not involved in this tranfer");

    // check code paramter (get_first_receiver) for correct token contract
    check(get_first_receiver() == EOS_SYMBOL.get_contract(), "fake EOS not accepted");
    check(quantity.amount > 0, "must be positive");
    check(quantity.symbol == EOS_SYMBOL.get_symbol(), "EOS only!");
    // ...
```


## References
- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
