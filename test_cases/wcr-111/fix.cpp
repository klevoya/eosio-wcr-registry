#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

static constexpr symbol EOS_SYMBOL = symbol("EOS", 4);
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

  ACTION batchtransfr(name from, uint64_t amount,
                      const std::vector<name> &tos) {
    require_auth(from);
    check(amount > 0, "only positive amount");

    balances_t balances_table(get_self(), get_self().value);
    auto from_balance =
        balances_table.require_find(from.value, "no from balance");

    auto quantity = asset(amount, EOS_SYMBOL);
    // fails here in overflow check
    asset total_quantity = quantity * tos.size();
    check(from_balance->balance >= total_quantity, "overdrawn balance");
    add_balance(from, -total_quantity);
    for (name to : tos) {
      add_balance(to, quantity);
    }
  }

  void add_balance(name account, asset balance) {
    balances_t balances_table(get_self(), get_self().value);
    auto bitr = balances_table.find(account.value);
    if (bitr == balances_table.end()) {
      check(balance.amount > 0, "negative balance");
      balances_table.emplace(get_self(), [&](auto &x) {
        x.account = account;
        x.balance = balance;
      });
    } else {
      check(bitr->balance.amount > 0, "negative balance");
      balances_table.modify(bitr, same_payer,
                            [&](auto &x) { x.balance += balance; });
    }
  }

  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (from == get_self())
      return;
    check(to == get_self(), "not involved");
    check(quantity.symbol == EOS_SYMBOL, "must be EOS");
    check(quantity.amount > 0, "must be positive");
    add_balance(from, quantity);
  }
};
