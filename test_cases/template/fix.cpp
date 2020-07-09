#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

class [[eosio::contract("fix")]] fix : public contract {
public:
  using contract::contract;
  fix(eosio::name receiver, eosio::name code,
        eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  TABLE dummy {
    uint64_t value;

    uint64_t primary_key() const { return value; }
  };

  typedef eosio::multi_index<"dummy"_n, dummy> dummy_t;

  ACTION insert(uint64_t value) {
    dummy_t dummy_table(get_self(), get_self().value);
    dummy_table.emplace(get_self(), [&](auto &x) { x.value = value; });
  }

  [[eosio::on_notify("*::transfer")]] void on_transfer(
      name from, name to, eosio::asset quantity, std::string memo) {}
};

// extern "C" {
//    void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
//    }
// }
