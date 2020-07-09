#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

static constexpr symbol EOS_SYMBOL = symbol("EOS", 4);

class [[eosio::contract("vulnerable")]] vulnerable : public contract {
public:
  using contract::contract;
  vulnerable(eosio::name receiver, eosio::name code,
             eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  [[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (to != get_self())
      return;

    check(quantity.amount > 0, "must be positive");
    // only check symbol not token contract (code parameter)
    check(quantity.symbol == EOS_SYMBOL, "EOS only!");

    // do something of value, f.i., pay out half of the quantity to sender
    asset promotion = asset(0.01 * quantity.amount, quantity.symbol);
    if (promotion.amount > 0) {
      // uses real eosio.token here
      action(permission_level{get_self(), name("active")}, name("eosio.token"),
             name("transfer"),
             make_tuple(get_self(), from, promotion, std::string("1% cashback")))
          .send();
    }
  }

  // such that an ABI is created
  ACTION dummy() {}
};
