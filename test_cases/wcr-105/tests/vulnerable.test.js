const { loadConfig, Blockchain } = require("@klevoya/hydra");

const config = loadConfig("hydra.yml");

describe("wcr-105", () => {
  let blockchain = new Blockchain(config);
  let vuln = blockchain.createAccount(`vulnerable`);

  beforeAll(async () => {
    vuln.setContract(blockchain.contractTemplates[`wcr-105`]);
    vuln.updateAuth(`active`, `owner`, {
      accounts: [
        {
          permission: {
            actor: vuln.accountName,
            permission: `eosio.code`
          },
          weight: 1
        }
      ]
    });
  });

  it("can insert the value", async () => {
    expect.assertions(1);

    await vuln.contract.insert({
      value: "123456"
    });

    expect(vuln.getTableRowsScoped(`dummy`)[`vulnerable`]).toEqual([
      {
        value: "123456"
      }
    ]);
  });
});
