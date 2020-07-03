<br/>

## Name: Fake Notification Receipt

### Unique Identifier: EOSIO-WCR-107

### Vulnerability Rating: Critical

### Relationship: [CWE-1021: Improper Restriction of Rendered UI Layers or Frames (Clickjacking)](https://cwe.mitre.org/data/definitions/1021.html)

### Background

### Summary
A vulnerable smart contract does not check the **to** and **self** parameters of the transfer action for equality, allowing an attacker to mislead the victim contract into believing it has received EOS tokens through a fake forwarded incoming transfer notification, when in actuality, they have transferred the EOS tokens to an intermediate contract they themselves control.

