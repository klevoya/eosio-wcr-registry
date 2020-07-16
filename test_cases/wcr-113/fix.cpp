#include "../eosio.token.hpp"
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

static constexpr extended_symbol EOS_SYMBOL =
    extended_symbol(symbol("EOS", 4), name("eosio.token"));

class [[eosio::contract("fix")]] fix : public contract {
public:
  using contract::contract;
  fix(eosio::name receiver, eosio::name code,
             eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  TABLE balances {
    name account;
    asset balance;

    uint64_t primary_key() const { return account.value; }
  };
  typedef eosio::multi_index<"balances"_n, balances> balances_t;
  balances_t balances_table = balances_t(get_self(), get_self().value);

  ACTION withdraw(name from, asset quantity) {
    require_auth(from);
    check(quantity.amount > 0, "only positive amount");
    // balance check only done here
    auto bitr = balances_table.require_find(from.value, "no balance");
    check(bitr->balance >= quantity, "overdrawn balance");

    // update balance within same action to prevent re-entrancy
    add_balance(from, -quantity);

    // log action does a notify which allows re-entrancy
    log_action log_act(get_self(),
                       permission_level{get_self(), name("active")});
    log_act.send(from, quantity);

    // transfer executed after log action and all of its created inline actions
    // are executed
    token::transfer_action transfer_act(
        EOS_SYMBOL.get_contract(),
        permission_level{get_self(), name("active")});
    transfer_act.send(get_self(), from, quantity, "withdraw");
  }

  ACTION logwithdraw(name from, asset quantity) {
    require_auth(get_self());
    // allows re-entrancy for from
    require_recipient(from);
  }
  using log_action =
      eosio::action_wrapper<"logwithdraw"_n, &fix::logwithdraw>;

  void add_balance(name account, asset balance) {
    auto bitr = balances_table.find(account.value);
    if (bitr == balances_table.end()) {
      balances_table.emplace(get_self(), [&](auto &x) {
        x.account = account;
        x.balance = balance;
      });
    } else {
      balances_table.modify(bitr, same_payer,
                            [&](auto &x) { x.balance += balance; });
    }
  }

  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    check(from == get_self() || to == get_self(), "not involved");
    check(quantity.symbol == EOS_SYMBOL.get_symbol(), "must be EOS");
    check(quantity.amount > 0, "must be positive");

    if (to == get_self()) {
      // incoming transfer notification add to user's balance
      add_balance(from, quantity);
    }
  }
};
