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

  TABLE rolls {
    name from;
    asset quantity;

    uint64_t primary_key() const { return from.value; }
  };

  typedef eosio::multi_index<"rolls"_n, rolls> rolls_t;
  rolls_t rolls_table = rolls_t(get_self(), get_self().value);

  [[eosio::on_notify("eosio.token::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    if (to != get_self())
      return;

    check(quantity.symbol == EOS_SYMBOL.get_symbol(), "wrong symbol");
    check(quantity.amount > 1'0000, "must be more than 1 EOS");

    rolls_table.emplace(get_self(), [&](auto &x) {
      x.from = from;
      x.quantity = quantity;
    });
  }

  // this is called in a different transaction
  ACTION resolvebet(uint64_t random, name roller) {
    require_auth(get_self());
    auto roll = rolls_table.require_find(roller.value, "roll not found");

    // pay out jackpot on rare event
    if (random == 0) {
      token::transfer_action transfer_act(
          EOS_SYMBOL.get_contract(),
          permission_level{get_self(), name("active")});
      transfer_act.send(get_self(), roll->from, roll->quantity * 10,
                        "You won the jackpot!");
    }
  }
};
