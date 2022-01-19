import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO ETH Call option 
const NETWORK = "ethereum";
const contractAddr = "0x9b8f14554f40705de7908879e2228d2ac94fde1a";
const SHARE_TO_BURN = "155550000000000000"
const MIN_ETH = '10'; // 0 
const BASE_SCREENS_S = 9 //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Withdraw eth from vault', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function withdrawETH(uint256,uint256)']);
  const {data} = await contract.populateTransaction.withdrawETH(SHARE_TO_BURN, MIN_ETH);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_opt_withdraw_eth', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Withdraw eth from vault', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function withdrawETH(uint256,uint256)']);
//   const {data} = await contract.populateTransaction.withdrawETH(SHARE_TO_BURN, MIN_ETH);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;

//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_opt_withdraw_eth', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));