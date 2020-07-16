#include "../eosio.token.hpp"
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

static constexpr extended_symbol EOS_SYMBOL =
    extended_symbol(symbol("EOS", 4), name("eosio.token"));

class [[eosio::contract("vulnerable")]] vulnerable : public contract {
public:
  using contract::contract;
  vulnerable(eosio::name receiver, eosio::name code,
             eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  uint64_t get_secure_random_roll() {
    // assume this is coming from a secure unpredictable source
    // it would still be a vulnerability if payout happens in same tx
    return 4;
  }

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

  // such that an ABI is created
  ACTION dummy() {}
};
