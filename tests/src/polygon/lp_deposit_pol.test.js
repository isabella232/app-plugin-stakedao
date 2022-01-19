import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO Masterchef on polygon
const NETWORK = "polygon";
const contractAddr = "0x68456b298c230415e2de7ad4897a79ee3f1a965a";
const AMOUNT_TO_DEPOSIT = '100259000000000000000'; // 100.25
const PID = 2;
const BASE_SCREENS_S = (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1); //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1 + 1 + 1 + 1); //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Deposit LPs into masterchef', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function deposit(uint256,uint256)']);
  const {data} = await contract.populateTransaction.deposit(PID, AMOUNT_TO_DEPOSIT);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_lp_deposit_pol', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Deposit LPs into masterchef', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function deposit(uint256,uint256)']);
//   const {data} = await contract.populateTransaction.deposit(PID, AMOUNT_TO_DEPOSIT);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;

//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_lp_deposit_pol', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));