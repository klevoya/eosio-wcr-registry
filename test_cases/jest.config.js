module.exports = {
  // transform: { "^.+\\.(ts|tsx)$": "ts-jest" },
  testPathIgnorePatterns: ["/node_modules/", "<rootDir>/template/"],
  testEnvironment: "node",
  testTimeout: 60 * 1e3,
};
