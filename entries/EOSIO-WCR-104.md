<br/>

## Name: Missing Assertion Check

### Unique Identifier: EOSIO-WCR-104

### Vulnerability Rating: Low

### Relationship: [CWE-754: Improper Check for Unusual or Exceptional Conditions](https://cwe.mitre.org/data/definitions/754.html)

## Background

There are going to be assumptions which will require assertions while execution of the contract. Using eosio_assert will take care of the conditions beforehand and stop the execution of the specific action if the assertions fails.

### Summary

The assert statement above makes an assumption that roll_under integer is greater than 2 & less than 96. But if it doesn’t, throw the above message and stop the execution. Failing to discover corner cases like the above could become catastrophic for the house setting the rules.
<br/>

## Vulnerability

### Sample Code 

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