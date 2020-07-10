<br/>

## Name: Missing Authorization Check

### Unique Identifier: EOSIO-WCR-109

### Vulnerability Rating: Low

### Relationship: [CWE-285: Missing Authorization)](https://cwe.mitre.org/data/definitions/862.html)

## Background
In EOSIO, a developer can use 3 functions to control authorization of smart contract actions by checking whether the caller of the action _equals_ the account that has the proper privileges.
	* require_auth(account)
	* require_auth2(account permission)
	* has_auth(account) 



### Summary
Due to missing authorization controls, a vulnerable smart contract **grants authorization** to **untrusted accounts** such as malicious parties, to access privileged contract resources e.g. **contract tables** or _perform_ mission-critical contract actions e.g. **token withdrawals**, where granting such **privilege is unsecure**

<br/>

## Vulnerability
EOS Smart contracts with no authority requirement allow **untrusted** accounts to call and enter actions effortlessly and perform critical operations such as modifying the underlying database or calling functions of other contracts on behalf of the vulnerable contract. This absence of permission authorization is tagged as a **missing authorization check** vulnerability.

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

## Remediation
### Risk Reduction
Implement controls so **token withdrawals** can **only** be triggered by **authorized** parties or according to the specifications of the smart contract system

## References
- [EOS Smart Contract Development Security Best Practices - Authorization Check
](https://github.com/slowmist/eos-smart-contract-security-best-practices/blob/master/README_EN.md#authorization-check)

- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
