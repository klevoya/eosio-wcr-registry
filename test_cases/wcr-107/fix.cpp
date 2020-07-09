#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;
using namespace std;

static constexpr extended_symbol EOS_SYMBOL =
    extended_symbol(symbol("EOS", 4), name("eosio.token"));

class [[eosio::contract("fix")]] fix : public contract {
public:
  using contract::contract;
  fix(eosio::name receiver, eosio::name code,
             eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  [[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (from == get_self())
      return;
    check(to == get_self(), "contract not involved in this tranfer");

    // check code paramter (get_first_receiver) for correct token contract
    check(get_first_receiver() == EOS_SYMBOL.get_contract(), "fake EOS not accepted");
    check(quantity.amount > 0, "must be positive");
    check(quantity.symbol == EOS_SYMBOL.get_symbol(), "EOS only!");

    // do something of value, f.i., pay out half of the quantity to sender
    asset promotion = asset(0.01 * quantity.amount, quantity.symbol);
    if (promotion.amount > 0) {
      action(
          permission_level{get_self(), name("active")}, EOS_SYMBOL.get_contract(),
          name("transfer"),
          make_tuple(get_self(), from, promotion, std::string("1% cashback")))
          .send();
    }
  }

  // such that an ABI is created
  ACTION dummy() {}
};

