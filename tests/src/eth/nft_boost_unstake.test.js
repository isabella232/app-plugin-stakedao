import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// StakeDAO NFTBoostVault contract
const NETWORK = "ethereum";
const contractAddr = "0xa324a2e3a6f64bd588565e0e1e2dd50e7a68bdd9";
const BASE_SCREENS_S = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT + STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT + STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT

test('[Nano S] Unstake NFT from NftBoostVault', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function unstake()']);
  const {data} = await contract.populateTransaction.unstake();
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_nft_unstake', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Unstake NFT from NftBoostVault', zemu("nanox", async (sim, eth) => {
//   const contract = new ethers.Contract(contractAddr, ['function stake()']);
//   const {data} = await contract.populateTransaction.stake(NFT_ID);
//   let unsignedTx = genericTx;
//   unsignedTx.to = contractAddr;
//   unsignedTx.data = data;
  
//   const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//   const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);
  
//   await waitForAppScreen(sim);
//   await sim.navigateAndCompareSnapshots('.', 'nanox_nft_stake', [BASE_SCREENS_X, 0]);
//   await tx;
// }, NETWORK));