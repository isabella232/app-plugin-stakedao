import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO DarkParadise
const contractAddr = "0x20d1b558ef44a6e23d9bf4bf8db1653626e642c3";
//const AMOUNT_TO_WITHDRAW = '100259000000000000000'; // 100.25
const BASE_SCREENS_S = (1 + 2 + 1 + 1) //STAKEDAO + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1) //STAKEDAO + GAS_FEES + VAULT + APPROVE

test('[Nano S] Stake into the premium strategy', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function exit()']);
  const {data} = await contract.populateTransaction.exit();
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_premium_withdraw', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Stake into the Palace', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function exit())']);
  const {data} = await contract.populateTransaction.exit();

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_premium_withdraw', [BASE_SCREENS_X, 0]);
  await tx;
}));