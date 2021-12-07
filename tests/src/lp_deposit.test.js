import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO Masterchef
const contractAddr = "0xfea5e213bbd81a8a94d0e1edb09dbd7ceab61e1c";
const AMOUNT_TO_DEPOSIT = '100259000000000000000'; // 100.25
const PID = '10';
const BASE_SCREENS_S = (1 + 2 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Deposit Lps into masterchef', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function deposit(uint256,uint256)']);
  const {data} = await contract.populateTransaction.deposit(PID, AMOUNT_TO_DEPOSIT);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_lp_deposit', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Deposit Lps into masterchef', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function deposit(uint256,uint256)']);
  const {data} = await contract.populateTransaction.deposit(PID, AMOUNT_TO_DEPOSIT);

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_lp_deposit', [BASE_SCREENS_X, 0]);
  await tx;
}));