#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

static constexpr symbol EOS_SYMBOL = symbol("EOS", 4);
class [[eosio::contract("vulnerable")]] vulnerable : public contract {
public:
  using contract::contract;
  vulnerable(eosio::name receiver, eosio::name code,
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

    auto total_amount = amount * tos.size();
    check(from_balance->balance.amount >= total_amount, "overdrawn balance");
    add_balance(from, -total_amount);
    for (name to : tos) {
      add_balance(to, amount);
    }
  }

  void add_balance(name account, uint64_t amount) {
    balances_t balances_table(get_self(), get_self().value);
    auto bitr = balances_table.find(account.value);
    if (bitr == balances_table.end()) {
      balances_table.emplace(get_self(), [&](auto &x) {
        x.account = account;
        x.balance = asset(amount, EOS_SYMBOL);
      });

    } else {
      balances_table.modify(bitr, same_payer,
                            [&](auto &x) { x.balance.amount += amount; });
    }
  }

  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (from == get_self())
      return;
    check(to == get_self(), "not involved");
    check(quantity.symbol == EOS_SYMBOL, "must be EOS");
    check(quantity.amount > 0, "must be positive");
    add_balance(from, quantity.amount);
  }
};
