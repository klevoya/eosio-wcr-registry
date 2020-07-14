<br/>

## Name: Missing Assertion Check

### Unique Identifier: EOSIO-WCR-104

### Vulnerability Rating: Medium

### Relationship: [CWE-754: Improper Check for Unusual or Exceptional Conditions](https://cwe.mitre.org/data/definitions/754.html)

## Background

**eosio.assert** roots from the EOSIO Assert Contract which is a security feature made to reduce the need for users to trust blockchain apps when a user signs a transaction for a trusted blockchain network with a trusted wallet application

### Summary

Smart contracts rely on a set of assumptions which require assertions to be met before users or external contracts are allowed to execute actions. Failure to use **eosio_assert** will mean the conditions required beforehand will be bypassed and be powerless to stop the execution of the specific action if the assertions fails

<br/>

## Vulnerability

### Sample Code 

The assert statement in the sample code below makes an assumption that **roll_under** integer is greater than 2 and less than 96. But if it doesn’t, throw the above message and stop the execution. Failing to discover corner cases like the above could become catastrophic for the house setting the rules.

```c++
void token::transfer( account_name from, account_name to, asset quantity)
{
    void assert_roll_under(const uint8_t& roll_under) 
    {
        eosio_assert(roll_under >= 2 && roll_under <= 96,
                "roll under overflow, must be greater than 2 and less than 96");
    }
}
```

## References
- [EOSIO Assert Contract](https://github.com/EOSIO/eosio.assert)
- [The Ultimate Guide to EOS Contract Security](https://blockgeeks.com/guides/eos-smart-contract-security/)