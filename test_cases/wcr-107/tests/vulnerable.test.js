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
describe("wcr-107", () => {
  let blockchain = new Blockchain(config);
  let eosioToken = blockchain.createAccount(`eosio.token`);
  let vuln = blockchain.createAccount(`vulnerable`);
  let attacker = blockchain.createAccount(`attacker`);
  let attackerContract = blockchain.createAccount(`attacker2`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-107`]);
    eosioToken.setContract(blockchain.contractTemplates[`eosio.token`]);
    attackerContract.setContract(blockchain.contractTemplates[`wcr-107-attacker`]);
    [vuln, eosioToken, attackerContract].forEach((acc) =>
      acc.updateAuth(`active`, `owner`, {
        accounts: [
          {
            permission: {
              actor: vuln.accountName,
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
          balance: "100.0000 EOS",
        },
      ],
      [attacker.accountName]: [
        {
          balance: "10000.0000 EOS",
        },
      ],
    });
  });

  it("can perform the attack", async () => {
    expect.assertions(2);

    await eosioToken.contract.transfer(
      {
        from: attacker.accountName,
        to: attackerContract.accountName,
        quantity: "10000.0000 EOS",
        memo: "sending tokens to own account but fake notification along the way",
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
