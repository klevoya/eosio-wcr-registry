<br/>

## Name: Haphazard Multi-index Migration

### Unique Identifier: EOSIO-WCR-110

### Vulnerability Rating: Low

### Relationship: [CWE-669: Incorrect Resource Transfer Between Spheres](https://cwe.mitre.org/data/definitions/669.html)

## Background

EOS currently stores persistent smart contract data onto a shared memory database for sharing across different actions. For storage of more complex data structures that need to be queryable by multiple indexes, a **multi_index table** need to be constructed.

### Summary

EOS currently does not allow modification of the table properties once it gets deployed. For example, to store a list of Pokémon owned by a trainer in a EOS game, a _Pokémon_ class is defined with private variables acting as the properties of each _Pokémon_. If the smart contract is migrated to mainnet and players start catching and building out their Pokémon teams, it becomes very hairy if the app developer suddenly realizes that the game requires an additional property, which is currently missing, for e.g. _Health_


## Detailed Description

Let's say the app developer writes the following properties for a basic pokémon.

### Sample Code 
```c++
#ifndef POKEMON
#define POKEMON

class [[eosio::table,  eosio::contract("pokédex")]] pokémon
{
private:    
    uint64_t _id;
    eosio::name _owner;
    eosio::name _pokémon_name;
    uint64_t _age;
    eosio::name _type;

public:    
    pokémon_t(){}

    pokémon_t(
        uint64_t const _id, 
        eosio::name const & _owner, 
        eosio::name const & _pokémon_name, 
        uint64_t const _age, 
        eosio::name const & _type
```

However he soon realizes that to make the game more engaging he needs to add in the following properties for each monster: _Health_, _Attack_, _Defence_, _Speed_. If he simply tries to update the existing table in his local c++ smart contract file, with this additional feature, the system will throw an **eosio_assert_message** _assertion failure_ error. 

## Vulnerability

Haphazard data migration from the deprecated table to a newly upgraded table with a richer parameter set, may result in loss of data and a loss of trust/reputation in the game/system.

## Remediation

A new table with a different name needs to be created, for example _Pokémon+_ and extreme care needs to be taken when migrating from the old __Pokémon__ table to the new __Pokémon+__ one ensuring no rows are left behind. 

### Risk Reduction

Each object and class in the smart contract needs to be thoroughly thought out in terms of system design, architecture and queryable scalability before the deployment of multi-index tables. This should be done with a team of developers through entity-relationship (ER) diagrams using tools like Gliffy, since retrospective table redesign is costly and exisiting data migration is a security risk

## References

- [The Ultimate Guide to EOS Contract Security](https://blockgeeks.com/guides/eos-smart-contract-security/)