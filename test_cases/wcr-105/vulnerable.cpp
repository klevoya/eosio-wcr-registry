#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/system.hpp>

using namespace eosio;

class [[eosio::contract("vulnerable")]] vulnerable : public contract {
public:
  using contract::contract;
  vulnerable(eosio::name receiver, eosio::name code,
             eosio::datastream<const char *> ds)
      : contract(receiver, code, ds) {}

  TABLE user {
    name username;
    std::string display_name;

    uint64_t primary_key() const { return username.value; }
  };

  typedef eosio::multi_index<"user"_n, user> users_t;

  ACTION insert(name user, std::string display_name) {
    users_t users_table(get_self(), get_self().value);
    users_table.emplace(user, [&](auto &x) {
      x.username = user;
      x.display_name = display_name;
    });
  }

  ACTION update(name user, std::string display_name) {
    users_t users_table(get_self(), get_self().value);
    auto user_itr = users_table.require_find(user.value, "user does not exist");
    users_table.modify(user_itr, eosio::same_payer, [&](auto &x) {
      x.username = user;
      x.display_name = display_name;
    });
  }
};
