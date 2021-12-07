import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO ETH Call option 
const contractAddr = "0x9b8f14554f40705de7908879e2228d2ac94fde1a";
const AMOUNT_TO_DEPOSIT = '100259000000000000000'; // 100.25
const BASE_SCREENS_S = (1 + 2 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Deposit eth into vault', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function depositETH(uint256)']);
  const {data} = await contract.populateTransaction.depositETH(AMOUNT_TO_DEPOSIT);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_opt_deposit_eth', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Deposit eth into vault', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function depositETH(uint256)']);
  const {data} = await contract.populateTransaction.depositETH(AMOUNT_TO_DEPOSIT);

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_opt_deposit_eth', [BASE_SCREENS_X, 0]);
  await tx;
}));