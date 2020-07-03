<br/>

## Name: Forged Token Transfer

### Unique Identifier: EOSIO-WCR-106

### Vulnerability Rating: High

### Relationship: [CWE-352: Cross-Site Request Forgery (CSRF)](https://cwe.mitre.org/data/definitions/352.html)

### Background
The ABI dispatcher/forwarder is the entry point which allows EOS smart contracts to listen to incoming token transfer events, like EOS tokens from the _eosio.token_ contract account, as well as normal interactions with the smart contract. A vulnerable smart contract that does not bind _action_ and _code_ to meet security requirements, fails to avoid abnormal and illegal calls.

### Summary
An implementation of the _(apply)_ function in a **victim** smart contract that does not verify the code parameter properly, and is **deceived** into **executing** smart-contract logic of **monetary value** on receipt of fake _EOS_ tokens, where the _transfer_ action roots from a token issued by an entity **other than** _eosio.token_. There are many similar tokens on both the EOS mainnet and other EOSIO side chains like WAX/TELOS, that are susceptible to the same attack.

### Diagram
![token transfer](images/token_transfer.png)

> **Figure 1.** Lifecycle of a token transfer
<br/>

## Detailed Description
In EOSIO, a token contract can be deployed on any account and the uniqueness of token symbols is not required. This means the _ticker namespace_ is _independent_ from the _account namespace_ and *not globally unique*. The same token symbol can be created multiple times because token symbols are only unique locally within one contract but not all contracts. Only, the extended_symbol, that combines a _symbol code_ with its _precision_ within a contract account is globally unique. 

There are currently multiple EOS, BTC and EOSDAC tokens on chain. The _eosio.token_ contract does not enforce unique tickers, even for the network token. Each account which runs an eosio.token contract forms a separate namespace. This is to say that [EOS@eosio.token](https://eosauthority.com/account/eosio.token?network=eos) is different from [EOS@eosioleoteam](https://eosauthority.com/account/eosioleoteam) and [EOS@indianastate](https://eosauthority.com/account/indianastate?network=eos) as shown below:
 <br/>
 <br/>
| Issuer | URL | Max Supply | Issued | 
| ------ | ------ | ------ | ------ |
| eosio.token | [/account/eosio.token?network=eos](https://eosauthority.com/account/eosio.token?network=eos) | 1,020,411,943 EOS | 933,711,931 EOS |
| eosioleoteam | [/account/eosioleoteam?network=eos](https://eosauthority.com/account/eosioleoteam?network=eos) | 1,000,000,000 EOS | 900,010,000 EOS |
| indianastate | [/account/indianastate?network=eos](https://eosauthority.com/account/indianastate?network=eos) | 10,000,000,000 EOS | 1,000,000,000 EOS |

> Forged Token Examples 

Source: [EOS Monitor](https://eosmonitor.io/account/eosio.saving)

 <br/>
Here is an example of an account creating transaction for a replica EOS token creation: 
<br/>
<br/>

[EOS Park Transaction](https://eospark.com/tx/89a0085987fdb407c628303aa091334c6f28a1c48efe4b0e6e0e4d3bacd0b40e)


## Vulnerability
EOS Smart contracts that use generic notification handlers (using the * wildcard to handle transfer notifications from any contract) often forget to check the received token symbol’s contract account to prevent against this token forgery attack. 

### Sample Code 

```c++
 // wildcard * used in on_notify, allows the 'transfer' action from any contract!

[[eosio::on_notify("*::transfer")]] 

void on_transfer(name from, name to, asset quantity, string memo) 
{
	// not sufficient to just check token symbol!
	check(quantity.symbol == EOS_SYMBOL, "wrong symbol");
} 
```

## Attack 

### Replication 
The attacker duplicates the open source eosio.token contract and issues a token using the same name as the original EOS, whose balance table is maintained in the **forged** _eosio.token_. By transferring in his newly created fake EOS tokens to the DApp, the attacker can then directly invoke the transfer function of the DApp. In vulnerable smart contracts, with no other security checks in place, the condition **code == receiver** will be satisfied. 

This is because the vulnerable DApp’s (apply) function, **doesn't verify** the issuer of the forged EOS tokens sent in to be _eosio.token_ (the official eos token issued by Block.One holding monetary value), instead of some _doppelganger_ account. Instead, it directly _allows code entry_ into the smart contract’s _transfer_ call. 

The result is that the attacker’s forged EOS tokens are deemed sufficient, to continue triggering the remaining code logic of the requested action. This means the attacker has, in essence, tricked the DApp’s (apply) function by sending in **worthless** tokens, in exchange for some presumably **valuable** action.

### Top Known
| Attacker Account | Victim Account | Tokens Stolen | Dollars Lost  
| ------ | ------ | ------ | ------
| re***e | eoscastdmgb1 | 63,014.10 EOS | $ 327,673.32
| aa***fg | eosbetdice11 | 44,427.43 EOS | $ 234,132.56
| my***er | eosluckydice | 4,508.00 EOS | $ 23,757.16
| ha***a2 | eosbaccasino | 1,613.40 EOS | $ 4,388.45
| wa***er | eosbaccasino | 1,613.40 EOS | $ 4,388.45


### Severity & Financial Impact
| Number of Verified Attacks | Attacker / Victim Ratio | Total Financial Loss
| ------ | ------ | ------
| 8 / 9 (88.88%) | 10 : 9 | $ 652,428.48

## Detection

### Steps
A successful attack definition is where true EOS tokens are extracted from a vulnerable smart contract in return for fake EOS tokens as identified through transaction records stored in token issuance tables. 

1. **Filter** out all EOS token transfer transactions by date
2. **Group** all transactions on the basis of the following definitions:
	* transactions that **send fake** EOS tokens 
	* transactions that **send true** EOS tokens
	* transactions that **receive true** EOS tokens
3. **Any** sequence where a fake-sending transaction is followed by a true-receiving transaction within the same day is defined to be a suspected attack
4. **Examine** the input-output ratio between the attacker and the vulnerable contract, for all suspected attacks, to determine those that have gained more true EOS tokens than they spent
5. **Verify** whether the vulnerable smart contract resumed normal execution (such as running a lottery for the player) after receiving the fake EOS token from the suspected attack sequence and if so the smart contract is _tagged_ to have a **Forged Token Transfer** vulnerability


### Methodology
#### Method 1
> EOSAFE → LocateFuncs + ExecDetector
1. **Locate** the _(apply)_ function as a suspicious target 
2. **Traverse** all feasible paths generated by symbolic execution of the _(apply)_ function
3. **Accelerate** analysis, by _eliminating_ irrelevant paths for any constraint group containing the condition action is equal to not transfer _(action == ¬ transfer)_
4. **Verify** if both of the following conditions are met for constraint groups in each feasible path:
   * the **action** variable points to _transfer_ function _(action == transfer)_
   * the **code** variable is not used to perform any checking 
5. **Filter** out only valuable _transfer_ functions if both of the above conditions are met, since not all transfer functions are able to affect on-chain state, and if such a function exists, the smart contract is _tagged_ to have a **Forged Token Transfer** vulnerability

#### Method 2
> EvulHunter → WASM Simulator + Detector Engine

##### WASM Simulator
1. **Pop** items from stack and assign it to local variables
2. **Simulate** execution and determine details about the local variables, such as whether they constitute a base32 encoded 64-bit integer that represents the _eosio.token_ in context
3. **Accelerate** analysis, by _eliminating_ irrelevant paths for any constraint group containing the condition action is equal to not transfer _(action == ¬ transfer)_

4. **Determine** the index numbers of these indirect call functions within the apply handler function

##### Detector Engine
5. **Invoking** any smart contract function such as a _transfer_ action predefined by the developer, where the invocation is not authorized by _eosio.token_, causes the smart contract to be _tagged_ to have a **Forged Token Transfer** vulnerability

### Algorithm
```c++
Description: It implements the interface LocateFuncs
	procedure LOCATEFUNCS(instructions)
	Init(targetFuncs)
	apply <- ExtractApply(instructions)
	Update(targetFuncs, (apply, "apply"))
	return targetFuncs
```

```c++
Description: It implements the interface ExecDetector
	procedure EXECDETECTOR(cfg, pathTree, label)
	constraints, _ <- ExtractFrom(pathTree)
	vul <- False
	for all c from constraints do
		transfer <- IsTransfer(c)
		codeCheck <- CodeChecked(c)
		if transfer ^ ¬ codeCheck then
			cond <- PathToTransfer()
		if cond ^ ValuableTransfer(cfg) then
			vul <- True
		return vul
```
> Figure 2: This is the detection algorithm used by EOSafe in Method 1

### Efficacy
| Vulnerable Smart Contracts | Vulnerable Smart Contract Versions
| ------ | ------
| 272 / 5, 574 (4.88%) | 1,457 / 53,666 (2.71%)


## Remediation
### Risk Reduction
To reduce the risk of this particular issue, DApps can narrow down the actions of accepted code as shown on _line 9_ below where the _transfer_ action is **not allowed**.

```c++
void apply(uint64_t receiver, uint64_t code, uint64_t action) 
{
	if(action == "onerror"_n.value)
	{
		// onerror is only valid if it is for 
		// the eosio code account and authorized
		// by eosio's active permission
		check(code == "eosio"_n.value, "onerror actions 
			are only valid from `eosio` system account");		
	}
	auto self = receiver;
	if(code == self && action != "transfer"_n.value)
	{
		switch(action)
		{
			EOSIO_DISPATCH_HELPER(TYPE, MEMBERS)
		}
		// does not allow destructor of this contract to
		// run eosio_exit(0);
	}
}
```

### Risk Mitigation
The token contract of the received token needs to be checked with the **get_first_receiver** function call inside the _apply_ code entry point


```c++
void apply(uint64_t receiver, uint64_t code, uint64_t action)
{
	// main code removed for brevity...

	check(get_first_receiver() == name("eosio.token"), "forged token");
}
```

## References

- [EOSIO Vulnerability Detection Patterns by Klevoya](https://docs.google.com/document/d/1QrXKrDzmIAModdIMk2Cq3gVa5Zmll9ZUZvZCZ7z-KeQ/edit#heading=h.b62lza9e4c5i)
- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
- [Detecting Fake Transfer Vulnerabilities
for EOSIO’s Smart Contracts at Webassembly-level by EVulHunter](https://arxiv.org/pdf/1906.10362)