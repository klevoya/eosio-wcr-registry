# EOSIO Weakness Classification Registry

The EOSIO Smart Contract Weakness Classification registry is a classification system for weaknesses and vulnerabilities in EOSIO smart contracts.

It is loosely aligned to the terminologies and structure used in the Common Weakness Classification ([CWE](https://cwe.mitre.org/)) and the Ethereum Smart Contract Weakness Classification ([SWC](https://github.com/SmartContractSecurity/SWC-registry)) Registry whilst paying attention to weakness variants that are specific to EOSIO smart contracts.

## EOS Publicly Known Vulnerabilites

| ID            | Name                                                        | CWE Equivalent                                                                                                                       |
| ------------- | ----------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| EOSIO-WCR-104 | [Missing Assertion Check](entries/EOSIO-WCR-104.md)         | [CWE-754: Improper Check for Exceptional Condition](https://cwe.mitre.org/data/definitions/754.html)                                 |
| EOSIO-WCR-105 | [Missing Authorisation Check](entries/EOSIO-WCR-105.md)     | [CWE-862: Missing Authorization](https://cwe.mitre.org/data/definitions/862.html)                                                    |
| EOSIO-WCR-106 | [Forged Token Transfer](entries/EOSIO-WCR-106.md)           | [CWE-352: Cross-Site Request Forgery](https://cwe.mitre.org/data/definitions/352.html)                                               |
| EOSIO-WCR-107 | [Fake Notification Receipt](entries/EOSIO-WCR-107.md)       | [CWE-1021: Improper Restriction of Rendered Frames](https://cwe.mitre.org/data/definitions/1021.html)                                |
| EOSIO-WCR-108 | [Transaction Rollback](entries/EOSIO-WCR-108.md)            | [CWE-471: Modification of Assumed-Immutable Data](https://cwe.mitre.org/data/definitions/471.html)                                   |
| EOSIO-WCR-109 | [Unchecked Action Arguments](entries/EOSIO-WCR-109.md)      | [CWE-749: Exposed Dangerous Method](https://cwe.mitre.org/data/definitions/749.html)                                                 |
| EOSIO-WCR-110 | [Haphazard Multi-Index Migration](entries/EOSIO-WCR-110.md) | [CWE-669: Incorrect Resource Transfer Between Spheres](https://cwe.mitre.org/data/definitions/669.html)                              |
| EOSIO-WCR-111 | [Integer Overflow](entries/EOSIO-WCR-111.md)                | [CWE-190: Integer Overflow](https://cwe.mitre.org/data/definitions/190.html)                                                         |
| EOSIO-WCR-112 | [Bad Randomness](entries/EOSIO-WCR-112.md)                  | [CWE-338: Use of Cryptographically Weak Pseudorandom Number Generator](https://cwe.mitre.org/data/definitions/338.html)              |
| EOSIO-WCR-113 | [Re-entrancy Attack](entries/EOSIO-WCR-113.md)              | [CWE-1265: Unintended Reentrant Invocation of Non-reentrant Code Via Nested Calls](https://cwe.mitre.org/data/definitions/1265.html) |
| EOSIO-WCR-114 | [Denial of Service](entries/EOSIO-WCR-114.md)               | [CWE-400: Uncontrolled Resource Consumption](https://cwe.mitre.org/data/definitions/400.html)                                        |
| EOSIO-WCR-115 | [Dangling Pointer](entries/EOSIO-WCR-115.md)                | [CWE-825: Expired Pointer Dereference](https://cwe.mitre.org/data/definitions/825.html)                                              |

## Create a new EOSIO WCR entry

Make sure that there is no matching weakness in the registry. Ideally, also coordinate with the community in [EOSIO WCR-registry Telegram channel](https://t.me/klevoya) to prevent conflicting entries. Create a file with a new EOSIO WCR ID in the [entries](./entries) directory. Use the [template](./entries/EOSIO-WCR-TEMPLATE.md) and describe all weakness attributes.

```
# Title
Pick a meaningful title.

## Relationships
Link a CWE Base or Class type to the CWS variant.
e.g.  [CWE-682: Incorrect Calculation](https://cwe.mitre.org/data/definitions/682.html)

## Description
Describe the nature and potential impact of the weakness on the contract system.

## Remediation
Describe ways on how to fix the weakness.

## References
Link to external references that contain useful additional information on the issue.

```

## Create a new test case

Test cases should be as varied as possible and include both simple test cases and real-world samples of vulnerable smart contracts. The test cases are grouped into subdirectories based on a single weakness variant or based on more complex real world contract systems that can contain various weakness variants. Use the [template directory](./test_cases/template). A single test case consists of the following structure:

1. A directory named `WCR-<number>` that contains all files belonging to a single test case
2. A vulnerable contract and a contract where the vulnerability has been fixed for each issue.
3. Add your contracts for compilation to the [CMakeLists root file](./test_cases/CMakeLists.txt)
4. Optional unit tests showing how to exploit the vulnerable contract. Add your contract to [hydra.yml](./test_cases/hydra.yml)

### Compiling contracts

After adding your contracts for compilation to the [CMakeLists root file](./test_cases/CMakeLists.txt):

```bash
cd test_cases
cmake .
make
```

### Running unit tests

```bash
cd test_cases
# If this is the first time running Hydra in this directory then install dependencies
npm i
# run all
npm test
# run only wcr-105 tests
npm test -- -t 'wcr-105'
```

One more thing, make sure the credit the author and mention the source if you don't write the contract sample yourself.

```
/*
 * @source: <link>
 * @author: <name>
 */
```

## Contributing

Before you create a PR for the first time make sure you have read:

- the sections [Create a new EOSIO WCR entry](#create-a-new-eosio-wcr-entry) and [Create a new test case](#create-a-new-test-case).
- read several existing EOSIO WCR definitions and their test cases.

## Scope of Weaknesses

EOSIO WCRs should be concerned with weaknesses that can be identified within the code of a smart contract, typically C++.
Weaknesses in 'smart contract adjacent' code should not be included.

## Contact

This repository is maintained by the [Klevoya](https://klevoya.com) team.
