// I need to revisit this pattern

const config = {
  local: {
    frontend: "http://localhost:3000",
    api: "http://localhost:4001", //"http://api.tiro-workers.dubcell.com",
    auth: "http://localhost:4000",
  },
  staging: {
    frontend: "http://tiro-workers.dubcell.com",
    api: "http://api.tiro-workers.dubcell.com",
    auth: "http://auth.tiro-workers.dubcell.com",
  },
  production: {
    frontend: "http://tiro-workers.dubcell.com",
    api: "http://api.tiro-workers.dubcell.com",
    auth: "http://auth.tiro-workers.dubcell.com",
  },
};

// console.log("process.env.RAZZLE_APP_ENV = %j", process.env.RAZZLE_APP_ENV);

export default config[process.env.RAZZLE_APP_ENV || "local"];