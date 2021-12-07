import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO DarkParadise
const contractAddr = "0x20d1b558ef44a6e23d9bf4bf8db1653626e642c3";
const AMOUNT_TO_STAKE = '100259000000000000000'; // 100.25
const NFT_TO_STAKE = '200';
const BASE_SCREENS_S = (1 + 2 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Stake into the premium strategy', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function stake(uint256,uint256)']);
  const {data} = await contract.populateTransaction.stake(AMOUNT_TO_STAKE, NFT_TO_STAKE);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_premium_stake', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Stake into the Palace', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function stake(uint256,uint256)']);
  const {data} = await contract.populateTransaction.deposit(AMOUNT_TO_STAKE, NFT_TO_STAKE);

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_premium_stake', [BASE_SCREENS_X, 0]);
  await tx;
}));