import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO Gauge multi rewards for the angle strategy 
const NETWORK = "ethereum";
const USER = "0x2ece0f323384e9076383cecaa479eb7ca5376126";
const contractAddr = "0x3c310fc54c0534dc3c45312934508722284352d1";
const BASE_SCREENS_S = (1 + 1 + 1 + 1 + 3 + 1 + 1) // STAKEDAO + STRATEGY + RECEIVE + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 3 + 1 + 1) // STAKEDAO + STRATEGY + RECEIVE + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT

test('[Nano S] Premium Strategy getReward', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function getRewardFor(address)']);
  const {data} = await contract.populateTransaction.getRewardFor(USER);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_angle_getreward', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Premium Strategy getReward', zemu("nanox", async (sim, eth) => {
//     const contract = new ethers.Contract(contractAddr, ['function getRewardFor(address)']);
//     const {data} = await contract.populateTransaction.getRewardFor(USER);
//     let unsignedTx = genericTx;
//     unsignedTx.to = contractAddr;
//     unsignedTx.data = data;
  
//     const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//     const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);
  
//     await waitForAppScreen(sim);
//     await sim.navigateAndCompareSnapshots('.', 'nanox_angle_getreward', [BASE_SCREENS_X, 0]);
//     await tx;
//   }, NETWORK));

