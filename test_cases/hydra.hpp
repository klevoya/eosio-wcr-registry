#pragma once
#include <eosio/eosio.hpp>
#include <eosio/print.hpp>

// multi_index does not expose indices_type, value_type and tableName
// need to pass all three tablename, row type, and table definition to template
// for now even though they are theoretically already part of table definition
template <eosio::name::raw TableName, typename RowType, typename MultiIndexType>
void hydra_insert_row(const eosio::name &_self, const eosio::name &table_name,
                      const eosio::name &scope,
                      const std::vector<char> &row_data) {
  MultiIndexType table(_self, scope.value);
  const RowType unpacked = eosio::unpack<RowType>(row_data);
  table.emplace(_self, [&](auto &obj) { obj = unpacked; });
}

#define STR(x) #x
#define TONAME(aname)                                                          \
  eosio::name { STR(aname) }

#define POPULATE_TABLE(r, dummy, field)                                        \
  case TONAME(BOOST_PP_SEQ_ELEM(0, field)).value: {                            \
    hydra_insert_row<TONAME(BOOST_PP_SEQ_ELEM(0, field)),                      \
                     BOOST_PP_SEQ_ELEM(1, field),                              \
                     BOOST_PP_SEQ_ELEM(2, field)>(get_self(), row.table_name,  \
                                                  row.scope, row.row_data);    \
    break;                                                                     \
  }

struct hydraload_payload {
  eosio::name table_name;
  eosio::name scope;
  std::vector<char> row_data;
};
// define this for production use
#ifdef HYDRA_SKIP_HELPERS
#define HYDRA_FIXTURE_ACTION(TABLES)
#define HYDRA_APPLY_FIXTURE_ACTION(CONTRACTNAME)
#else
#define HYDRA_FIXTURE_ACTION(TABLES)                                           \
  ACTION hydraload(const std::vector<hydraload_payload> payload) {             \
    require_auth(eosio::name("eosio"));                                        \
    for (auto row : payload) {                                                 \
      switch (row.table_name.value) {                                          \
        BOOST_PP_SEQ_FOR_EACH(POPULATE_TABLE, DUMMY_MACRO, TABLES)             \
      default:                                                                 \
        eosio::check(false, "Unknown table to load fixture");                  \
      }                                                                        \
    }                                                                          \
  }

#define HYDRA_APPLY_FIXTURE_ACTION(CONTRACTNAME)                               \
  if (code == receiver && action == eosio::name("hydraload").value)            \
    eosio::execute_action(eosio::name(receiver), eosio::name(code),            \
                          &CONTRACTNAME::hydraload);
#endif