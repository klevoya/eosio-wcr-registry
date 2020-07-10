<br/>

## Name: Fake Notification Receipt

### Unique Identifier: EOSIO-WCR-107

### Vulnerability Rating: Critical

### Relationship: [CWE-1021: Improper Restriction of Rendered UI Layers or Frames (Clickjacking)](https://cwe.mitre.org/data/definitions/1021.html)

## Background

### Summary
A vulnerable smart contract does not check the **to** and **self** parameters of the transfer action for equality, allowing an attacker to mislead the victim contract into believing it has received EOS tokens through a fake forwarded incoming transfer notification, when in actuality, they have transferred the EOS tokens to an intermediate contract they themselves control.

<br/>

### Methodology
#### EOSSAFE Evaluation Benchmark

| Total Samples | Vulnerable : Safe | Precision | Recall | F1-measure 
| ------ | ------ | ------ | ------ | ------ 
| 10 | 5 : 5 | 100.00 % | 100.00 % | 100.00 %

<br/>

### Efficacy
| Vulnerable Smart Contracts | Vulnerable Smart Contract Versions
| ------ | ------
| **2,192** / 5,574 (39.33%) | **7,143** / 53,666 (13.31%)

<br/>

#### Patching Statistics
| Patch % | Patch Time
| ------ | ------
| **457** / 2,192 (20.85%) | **24.01** days

<br/>

## References
- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
