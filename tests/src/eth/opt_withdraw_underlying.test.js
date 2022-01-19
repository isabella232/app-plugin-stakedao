import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO BTC Call option 
const NETWORK = "ethereum";
const contractAddr = "0x227e4635c5fe22d1e36dab1c921b62f8acc451b9";
const MIN_ECRV = '0'; // 0
const UNDERLYING = "1";
const BASE_SCREENS_S = 9; //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1); //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Withdraw underlying from option strategy', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function withdrawUnderlying(uint256,uint256)']);
  const {data} = await contract.populateTransaction.withdrawUnderlying(UNDERLYING, MIN_ECRV);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_opt_withdraw_underlying', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Withdraw underlying from option strategy', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function withdrawUnderlying(uint256,uint256)']);
//   const {data} = await contract.populateTransaction.withdrawUnderlying(UNDERLYING, MIN_ECRV);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;

//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_opt_withdraw_underlying', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));