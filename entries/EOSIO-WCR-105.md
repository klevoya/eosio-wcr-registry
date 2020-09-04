
## Name: Missing Authorisation Check

### Unique Identifier: EOSIO-WCR-109

### Vulnerability Rating: Medium

### Relationship: [CWE-862: Missing Authorisation](https://cwe.mitre.org/data/definitions/862.html)

## Background

In EOSIO, a developer can use two functions to control authorisation of smart contract actions by checking whether the declared authorisation of the action _equals_ the account that should be able to run the action.
	* require_auth(account [, permission])
	* has_auth(account) 

### Summary

Due to missing authorisation controls, a vulnerable smart contract **grants authorisation** to **untrusted accounts** such as malicious parties, to access privileged contract resources e.g. **contract tables** or _perform_ mission-critical contract actions e.g. **token withdrawals**, where granting such **privilege is insecure**.

## Vulnerability

EOS Smart contracts with no authority requirement allow **untrusted** accounts to call and enter actions effortlessly and perform critical operations such as modifying the underlying database or calling functions of other contracts on behalf of the vulnerable contract. This absence of permission authorisation is tagged as a **missing authorisation check** vulnerability.

### Sample Code

The vulnerable smart contract transfer action allows anyone to call the action. 

```c++
void token::transfer( account_name from, account_name to, asset quantity)
{
   auto sym = quantity.symbol.name();
   require_recipient( from );
   require_recipient( to );
   auto payer = has_auth( to ) ? to : from;
   sub_balance( from, quantity );
   add_balance( to, quantity, payer );
}
```
In order to resolve it, the **`require_auth(from);`** statement is needed to authorize the payer to call the action

### Test Case

> [Test for Missing authorisation Check by Klevoya](../test_cases/wcr-105/)


## Remediation

### Risk Reduction

Implement controls so **token withdrawals** can **only** be triggered by **authorized** parties or according to the specifications of the smart contract system

## References

- [EOS Smart Contract Development Security Best Practices - authorisation Check](https://github.com/slowmist/eos-smart-contract-security-best-practices/blob/master/README_EN.md#authorisation-check)

- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
