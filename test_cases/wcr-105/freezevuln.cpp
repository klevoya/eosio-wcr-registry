#include <string>
#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/print.hpp>
#include <eosio/singleton.hpp>

using namespace eosio;
using namespace std;

CONTRACT freezevuln : public eosio::contract
{
public:
    freezevuln(name receiver, name code, datastream<const char *> ds)
        : contract(receiver, code, ds), 
        frz(receiver, receiver.value) {}

    TABLE status
    {
        uint64_t frozen;
    };

    typedef eosio::singleton<"freezestatus"_n, status> frozen_t;
    // dummy for ABI generator
    typedef eosio::multi_index<"freezestatus"_n, status> abi_frozen_t;

    uint64_t getFreezeFlag()
    {
        // Gets stored freeze status if it exists, otherwise creates it
        // and bills self for the RAM used
        status f = frz.get_or_create(_self, status{.frozen = 0});

        return f.frozen;
    }

    void setFreezeFlag(const uint64_t &pFrozen)
    {
        status freeze_state;
        freeze_state.frozen = getFreezeFlag();
        freeze_state.frozen = pFrozen;
        frz.set(freeze_state, _self);
    }

    ACTION freeze()
    {
        // Only the contract owner should be able to perform this action
        require_auth(_self);
        setFreezeFlag(1);
    }

    ACTION unfreeze()
    {
        // Only the contract owner should be able to perform this action
        //require_auth(_self);
        setFreezeFlag(0);
    }

private:
    frozen_t frz;
};
