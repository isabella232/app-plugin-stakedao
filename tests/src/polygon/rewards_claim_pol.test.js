import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO Rewards Claimer on polygon
const NETWORK = "polygon";
const contractAddr = "0xDD24327fF0B55A6213f91dBf5A8A894585836835";
const MERKLE_INDEX = "11";
const INDEX = "65";
const AMOUNT_TO_CLAIM = '21507013325470224384'; // 21.5
const MERKLE_PROOF = [];
const BASE_SCREENS_S = (1 + 1 + 1 + 2 + 3 + 1 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT (2) +  STRATEGY ADDRESS (3) + NETWORK + GAS_FEES + ACCEPT
const BASE_SCREENS_X = (1 + 1 + 1 + 2 + 3 + 1 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT (2) +  STRATEGY ADDRESS (3) + NETWORK + GAS_FEES + ACCEPT

test('[Nano S] Claim xSDT reward', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function claim(uint256,uint256,uint256,bytes32[])']);
  const {data} = await contract.populateTransaction.claim(MERKLE_INDEX, INDEX, AMOUNT_TO_CLAIM, MERKLE_PROOF);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;
  unsignedTx.chainId = 137;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_rewards_claim_pol', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Claim xSDT reward', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function claim(uint256,uint256,uint256,bytes32[])']);
//   const {data} = await contract.populateTransaction.claim(MERKLE_INDEX, INDEX, AMOUNT_TO_CLAIM, MERKLE_PROOF);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;
//   unsignedTx.chainId = 137;
  
//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);
  
//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_rewards_claim_pol', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));