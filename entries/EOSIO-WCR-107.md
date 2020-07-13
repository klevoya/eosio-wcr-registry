<br/>

## Name: Fake Notification Receipt

### Unique Identifier: EOSIO-WCR-107

### Vulnerability Rating: Critical

### Relationship: [CWE-1021: Improper Restriction of Rendered UI Layers or Frames (Clickjacking)](https://cwe.mitre.org/data/definitions/1021.html)

## Background

### Summary
A vulnerable smart contract does not check the **to** and **self** parameters of the transfer action for equality, allowing an attacker to mislead the victim contract into believing it has received EOS tokens through a fake forwarded incoming transfer notification, when in actuality, they have transferred the EOS tokens to an intermediate contract they themselves control.

<br/>

## Vulnerability
### Test Case
> [Test for Fake Notification Receipt by Klevoya™](../test_cases/wcr-107/)

<br/>

## Attack 

### Top Known
| Attacker Account | Victim Account | Tokens Stolen | Dollars Lost  
| ------ | ------ | ------ | ------
| il***23, wh***r1 | eosbetdice11 | 138,319.80  EOS | $ 756,609.30
| re***om, re***et | nkpaymentcap | 53,641.71 EOS | $ 200,619.98 
| be***s1, be***s2 | epsdcclassic | 17,388.73 EOS | $ 41,559.07
| po***ok, bi***os, he***bc | gambaccarats | 2,479.23 EOS | $ 9,346.70

### Severity & Financial Impact
| Number of Verified Attacks | Attacker / Victim Ratio | Total Financial Loss
| ------ | ------ | ------
| **5** / 27 (18.52%) | 28 : 17 | $ 1,020,831.94

<br/>

## Detection
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
