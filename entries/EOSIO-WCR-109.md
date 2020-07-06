<br/>

## Name: Missing Permission Check

### Unique Identifier: EOSIO-WCR-109

### Vulnerability Rating: Low

### Relationship: [CWE-306: Missing Authentication for Critical Function (Improper Authentication)](https://cwe.mitre.org/data/definitions/306.html)

<br/>

## Background
In EOSIO, a developer can use 3 functions to control authorization of smart contract actions by checking whether the caller of the action _equals_ the account that has the proper privileges.
	* require_auth(account)
	* require_auth2(account permission)
	* has_auth(account) 

<br/>

### Summary
A smart contract that **grants permission** to **untrusted accounts** to run **critical code** where granting such **privilege is unsecure**.

<br/>

## Vulnerability
EOS Smart contracts with no authority requirement allow **untrusted** accounts to call and enter actions effortlessly and perform critical operations such as modifying the underlying database or calling functions of other contracts on behalf of the vulnerable contract. This absence of permission authorization is tagged as a **missing permission check** vulnerability.

### Methodology
#### EOSSAFE Evaluation Benchmark

| Total Samples | Vulnerable : Safe | Precision | Recall | F1-measure 
| ------ | ------ | ------ | ------ | ------ 
| 10 | 6 : 4 | 100.00 % | 100.00 % | 100.00 %

<br/>

### Efficacy
| Vulnerable Smart Contracts | Vulnerable Smart Contract Versions
| ------ | ------
| **662** / 5,574 (11.88%) | **8,373** / 53,666 (15.60%)

<br/>

#### Patching Statistics
| Patch % | Patch Time
| ------ | ------
| **349** / 662 (53.72%) | **4.38** days

<br/>

## References
- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
