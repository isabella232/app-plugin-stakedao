import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO Premium strategy
const NETWORK = "ethereum";
const contractAddr = "0x20d1b558ef44a6e23d9bf4bf8db1653626e642c3";
const AMOUNT_TO_STAKE = '1000000000000000000'; // 1 SDT
const NFT_ID = 442;
const BASE_SCREENS_S = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT

test('[Nano S] Stake SDT into premium', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function stake(uint256,uint256)']);
  const {data} = await contract.populateTransaction.stake(AMOUNT_TO_STAKE, NFT_ID);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_premium_stake', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Stake SDT into premium', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function stake(uint256,uint256)']);
//   const {data} = await contract.populateTransaction.stake(AMOUNT_TO_STAKE, NFT_ID);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;

//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_premium_stake', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));