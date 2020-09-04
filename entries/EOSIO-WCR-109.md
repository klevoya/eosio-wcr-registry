

## Name: Unchecked Action Arguments

### Unique Identifier: EOSIO-WCR-109

### Vulnerability Rating: Low

### Relationship: [CWE-749: Exposed Dangerous Method or Function](https://cwe.mitre.org/data/definitions/749.html)

## Background

EOSIO actions can take several arguments, referred to as the _action data_. This is data that is controlled by the transaction creator.

### Summary

Any poorly tested EOSIO action that relies on action arguments but does not correctly check them is vulnerable.

## Detailed Description

The EOSIO DApp called **Se7ens** _(now deprecated)_ declared a new `airdrop` action inside their `eosio.token` contract for airdropping their tokens into user accounts.

## Vulnerability

The _Se7ens_ DApp forgot to verify the amount in the custom `airdrop` action before the transfer took place, allowing an anonymous hacker to claim _1 billion_ of their tokens when they went live on mainnet.

## Remediation

### Risk Mitigation

It is inadvisable to modify the token contract code by adding custom functionality as bugs in the `eosio.token` contract can be fatal. For example, in order to facilitate airdrop functionality securely, transfer the airdrop tokens to a **separate eosio account** and distribute the funds further from there. This allows you to freeze the account and implement emergency patches, if a logic related bug is realized after the fact.

## References

- [The Ultimate Guide to EOS Contract Security](https://blockgeeks.com/guides/eos-smart-contract-security/)
