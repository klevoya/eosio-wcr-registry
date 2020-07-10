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
describe("wcr-108", () => {
  let blockchain = new Blockchain(config);
  let eosioToken = blockchain.createAccount(`eosio.token`);
  let vuln = blockchain.createAccount(`vulnerable`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-108`]);
    eosioToken.setContract(blockchain.contractTemplates[`eosio.token`]);
    attacker.setContract(blockchain.contractTemplates[`wcr-108-attacker`]);
    [vuln, eosioToken, attacker].forEach((acc) =>
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
          balance: "10000.0000 EOS",
        },
      ],
      [attacker.accountName]: [
        {
          balance: "10.0000 EOS",
        },
      ],
    });
  });

  it("can perform the attack", async () => {
    expect.assertions(1);

    await expect(
      attacker.contract.attack({}, [
        { actor: attacker.accountName, permission: `active` },
      ])
    ).rejects.toMatchObject({
      message: expect.stringMatching(/would have lost/gi),
    });
  });
});
