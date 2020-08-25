## Name: Bad Randomness

### Unique Identifier: EOSIO-WCR-112

### Vulnerability Rating: High

### Relationship:

- [CWE-338: Use of Cryptographically Weak Pseudo-Random Number Generator (PRNG)](https://cwe.mitre.org/data/definitions/338.html)
- [EOSIO-WCR-108: Transaction Rollback](./EOSIO-WCR-108.md)

## Background

Some smart contracts need access to a random number. Some ways of getting a “random” number are
using a secret value, using embedded secret code, basing the number off of statistics from mining or
using an external oracle. 

Since smart contract code is public on the blockchain, the first two options do
not work.
Receiving random numbers through an oracle call can work but one needs to be careful about:
- who feeds the oracle data, how decentralized and fault-tolerant it is
- if it uses a provably fair algorithm, i.e., the oracle operators can't submit any number that would profit them.


### Summary

EOS Smart contracts that generate **weak** on-chain random numbers fall victim to this attack as an attacker can **predict** them and use this predictability to gain an advantage.
Another form of this attack are algorithms that indeed generate random numbers but the system as a whole can be gamed by, for example, rerolling the randomness or even completely rolling back the transaction on an undesired outcome.


### Sample Code 

The following random number generator is a [real example](https://github.com/loveblockchain/eosdice/blob/master/eosbocai2222.hpp#L209-L218) that tries to mix several on-chain data points to create randomness.
All of these can be predicted which makes it **insecure**.

```c++
uint8_t random(account_name name, uint64_t game_id)
{
    asset pool_eos = eosio::token(N(eosio.token)).get_balance(_self, symbol_type(S(4, EOS)).name());
    auto mixd = tapos_block_prefix() * tapos_block_num() + name + game_id - current_time() + pool_eos.amount;

    const char *mixedChar = reinterpret_cast<const char *>(&mixd);

    checksum256 result;
    sha256((char *)mixedChar, sizeof(mixedChar), &result);

    uint64_t random_num = *(uint64_t *)(&result.hash[0]) + *(uint64_t *)(&result.hash[8]) + *(uint64_t *)(&result.hash[16]) + *(uint64_t *)(&result.hash[24]);
    return (uint8_t)(random_num % 100 + 1);
}
```

Note that the _tapos_ fields of a transaction are controlled by the transaction creator as well.

## Remediation

### Risk Reduction

Avoid using variables which could be controlled by players such as **anything that exists on-chain** for random number generation and avoid putting the **resolve** action and the **notify** action (receipt) in the same transaction.


### Risk Mitigation

Some common and provably-fair ways to provide on-chain random numbers are:
- Requesting a random number from an oracle using a unique identifier for each request. The oracle operators sign this value with a known public key and a signature algorithm that yields unique [signatures like RSA](https://github.com/worldwide-asset-exchange/wax-orng#the-wax-rng-native-blockchain-service).
- Using a [commit and reveal approach](https://eosio.stackexchange.com/questions/41/how-can-i-generate-random-numbers-inside-a-smart-contract/215#215) to create provably-fair random numbers between two or more mutually untrusting parties

## References

- [Provable Things](https://github.com/provable-things)
- [Defeating EOS Gambling Games: The Tech Behind Random Number Loophole](https://medium.com/@peckshield/defeating-eos-gambling-games-the-tech-behind-random-number-loophole-cf701c616dc0)
- [EOS Smart Contract Development Security Best Practices - authorisation Check](https://github.com/slowmist/eos-smart-contract-security-best-practices/blob/master/README_EN.md#authorisation-check)


