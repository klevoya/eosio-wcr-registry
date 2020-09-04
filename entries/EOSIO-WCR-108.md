

## Name: Transaction Rollback

### Unique Identifier: EOSIO-WCR-108

### Vulnerability Rating: High

### Relationship: [CWE-471: Modification of Assumed-Immutable Data (MAID)](https://cwe.mitre.org/data/definitions/471.html)



## Background

An EOSIO transaction can consist of several actions and is an atomic operation. When any of these actions fails the whole transaction is reverted and none of the actions is applied to the chain state.

### Summary

A vulnerable smart contract makes an important decision that allows an attacker to check for the outcome and revert the whole transaction if the outcome is undesired.

Common examples are gambling contracts that resolve the bet in the same transaction as they receive it, or auction smart contracts notifying the previous bidder in the same transaction when receiving the higher bid.


For example, consider a vulnerable gambling smart contract that relies on users to transfer EOS before a pseudo-random outcome is revealed based on an on-chain state value like the _tapos_block_prefix_. These smart contracts contain a reveal function that is reachable from the _apply_ function (entry point) and is often used by lottery DApps where the winner is a player whose pick matches the generated pseudorandom value.

An implementation of the _reveal_ function, where all _actions_ are *merged* into a *single transaction* when revealing the winner of the lottery, is vulnerable. This merged reveal, allows the attacker to utilise an evil intermediate contract between the vulnerable DApp and the official _eosio.token_ contract and abuse the _check(false)_ operation to repeatedly revert all losing guesses until a winning guess is made.

### Diagram

![token transfer](images/rollback.png)

> **Figure 1.** Attacker exploits vulnerable DApp via evil intermediate smart contract and repeated revert



## Detailed Description

The attacker invokes the _transfer_ function of their evil **intermediate** contract to **forward** a genuine EOS token transfer to the vulnerable DApp and make a successful **guess** for 1 round of the lottery.

Once the tokens have successfully transferred to the victim DApp, the intermediate contract is **notified** of a token deduction by the _eosio.token_ contract within the very same transaction.

The game then completes the round, by matching the generated pseudo-random outcome to the best guess and sending the prize _(EOS tokens)_ as winnings to the elected winner _(contract)_.

The evil intermediate contract essentially checks and **tracks** the balance **before** and **after** each round of the game, triggering the _rollback_ operation unless it detects that its balance has increased _(winning guess)_ and repeats this again and again every round.

This means, for every losing round, the attacker ensures that the smart contract states and balance table is **reverted** to the original, and the losing token transfer transaction is **not** recorded on the EOS mainnet.

This _collision-like loop_ allows the attacker to _arbitrage_ the entire prize money from the vulnerable lottery DApp at statistically **insignificant** cost.
 
 

## Vulnerability

The bet is resolved in the same transaction as it is received and the outcome can, therefore, be checked by an attacker in the same transaction, reverting on losses.

```cpp
[[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
    name from, name to, eosio::asset quantity, std::string memo) {
  if (to != get_self())
    return;

  check(quantity.symbol == EOS_SYMBOL.get_symbol(), "wrong symbol");
  check(quantity.amount > 1'0000, "must be more than 1 EOS");

  // pay out jackpot on rare event
  if (get_secure_random_roll() == 0) {
    token::transfer_action transfer_act(
        EOS_SYMBOL.get_contract(),
        permission_level{get_self(), name("active")});
    transfer_act.send(get_self(), from, quantity * 10,
                      "You won the jackpot!");
  }
}
```

### Test Case

> [Test for Dynamic Transaction Rollback by Klevoya™](../test_cases/wcr-108/)


## Remediation

Resolving the bet must happen in a different transaction than receiving the bet.

## References

- [Security Analysis of EOSIO Smart Contracts by EOSAFE](https://arxiv.org/abs/2003.06568)
