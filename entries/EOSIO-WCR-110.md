

## Name: Haphazard Multi-Index Migration

### Unique Identifier: EOSIO-WCR-110

### Vulnerability Rating: Low

### Relationship: [CWE-669: Incorrect Resource Transfer Between Spheres](https://cwe.mitre.org/data/definitions/669.html)

## Background

EOSIO currently stores persistent smart contract data onto a shared memory database for sharing data across different actions. For storage of more complex data structures that need to be queryable by multiple indexes, a **multi_index table** needs to be constructed.

### Summary

EOSIO currently does not allow arbitrary modification of the table structure once the contract is deployed and data is already stored.
This leads to an inconsistent state for the old data when deserializing it with the new table structure.


## Detailed Description

For example, to store a list of Pokémon owned by a trainer in an EOSIO game, a _Pokémon_ class is defined with private variables acting as the properties of each _Pokémon_. If the smart contract is migrated to mainnet and players start catching and building out their Pokémon teams, it becomes very hairy if the app developer suddenly realizes that the game requires an additional property, which is currently missing, for e.g. _Health_
Let's say the app developer writes the following properties for a basic pokémon.

### Sample Code 

```c++
#pragma once

class [[eosio::table,  eosio::contract("pokedex")]] pokemon
{
private:    
    uint64_t _id;
    eosio::name _owner;
    eosio::name _pokemon_name;
    uint64_t _age;
    eosio::name _type;

public:    
    pokemon_t(){}

    pokemon_t(
        uint64_t const _id, 
        eosio::name const & _owner, 
        eosio::name const & _pokemon_name, 
        uint64_t const _age, 
        eosio::name const & _type
```

However, the developer soon realizes that to make the game more engaging he needs to add in the following properties for each monster: _Health_, _Attack_, _Defence_, _Speed_.
Simply adding the new fields to the existing table structure will break the smart contract when trying to read a table row that was stored in the old format: `assertion failure with message: read`.

## Vulnerability

Haphazard data migration from the deprecated table to a newly upgraded table may result in loss of data and a loss of trust/reputation in the game/system.

## Remediation

The old data needs to be migrated to the new table structure first.
A new table with a different name needs to be created, for example, pokemon2_ and extreme care needs to be taken when migrating from the old __Pokémon__ table to the new _pokemon2__ one ensuring no rows are left behind.
After all data is stored in the new format the old table can be deleted.

Appending fields to an existing table is possible by using the [eosio::binary_extensions](https://github.com/EOSIO/eosio.cdt/blob/a6b8d3fc289d46f4612588cdd7223a3d549238f6/docs/09_tutorials/01_binary-extension.md) type.
Note that the new fields must be added to the end of the table structure and no previous field may be rearranged or removed.

### Risk Reduction

Each object and class in the smart contract needs to be thoroughly thought out in terms of system design, architecture and queryable scalability before the deployment of multi-index tables. This should be done with a team of developers through entity-relationship (ER) diagrams using tools like Gliffy since retrospective table redesign is costly and existing data migration is a security risk

## References

- [The Ultimate Guide to EOS Contract Security](https://blockgeeks.com/guides/eos-smart-contract-security/)
- [EOSIO Binary Extensions](https://github.com/EOSIO/eosio.cdt/blob/a6b8d3fc289d46f4612588cdd7223a3d549238f6/docs/09_tutorials/01_binary-extension.md)
