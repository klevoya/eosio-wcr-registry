<br/>

## Name: Untested Token Method

### Unique Identifier: EOSIO-WCR-109

### Vulnerability Rating: Low

### Relationship: [CWE-749: Exposed Dangerous Method or Function](https://cwe.mitre.org/data/definitions/749.html)

## Background

App developers often need to code in innovative smart contract features to improve [token velocity](https://investmentbank.com/token-velocity/) of their custom EOSIO token. One such way is to offer cryptocurrency **airdrops**, free coins that are dropped directly into a contributing user's wallet. Airdrops are _calculated marketing maneuvers_ which actually save blockchain companies a lot of money in the long run. To code in airdrop functionality, the system eosio.token contract needs have additional logic.

### Summary

Any poorly tested method call within a custom eosio.token contract that exhibits unexpected token behaviour, that may result in funds being inappropriately added or removed from a user's account 

## Detailed Description

The EOSIO DApp called **Se7ens** _(now deprecated)_ declared a new method inside their eosio.token contract for airdropping their tokens into user accounts. The _Se7ens_ smart contract did not require the _issue_ or the _transfer_ action of the eosio.token contract to be called in order for the fund increment to be available on the user's account. The funds in fact, appeared magically on the accounts of the users.

## Vulnerability

The _Se7ens_ DApp forgot to verify the amount in the custom _Airdrop_ method before the transfer took place, allowing an anonymous hacker to claim _1 billion_ of their tokens when they went live on mainnet. 

## Remediation
### Risk Mitigation

Apart from changing the maximum supply and the token symbol, it is inadvisable to modify the token contract code by adding custom functionality as bugs in the eosio.token contract can be fatal. For example, in order to facilitate airdrop functionality securely, transfer the airdrop tokens to a **separate eosio account** and distribute the funds further from there. This allows you to freeze the account and implement emergency patches, if a logic related bug is realized after the fact.

## References

- [The Ultimate Guide to EOS Contract Security](https://blockgeeks.com/guides/eos-smart-contract-security/)
