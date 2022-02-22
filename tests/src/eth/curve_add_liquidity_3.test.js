import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from '../test.fixture';
import { ethers } from "ethers";

// Crv 3crv pool 
const NETWORK = "ethereum";
const contractAddr = "0xbebc44782c7db0a1a60cb6fe97d0b483032ff1c7";
const DAI_TO_DEPOSIT = '100259000000000000000'; // 100.259 DAI
const USDC_TO_DEPOSIT = '100000000000000000000'; // 100 USDC
const USDT_TO_DEPOSIT = '300000000000000000000'; // 300 USDT
const MIN_AMOUNT = "0";
const BASE_SCREENS_S = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 3 + 1 + 1); // STAKEDAO + STRATEGY + WANT + AMOUNT +  STRATEGY ADDRESS (3) + GAS_FEES + ACCEPT

test('[Nano S] Provide liquidity into the 3crv curve pool', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function add_liquidity(uint256[3],uint256)']);
  const {data} = await contract.populateTransaction.add_liquidity([DAI_TO_DEPOSIT, USDC_TO_DEPOSIT, USDT_TO_DEPOSIT], MIN_AMOUNT);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_curve_add_liquidity_3', [BASE_SCREENS_S, 0]);
  await tx;
}, NETWORK));

// test('[Nano X] Provide liquidity into the 3crv curve pool', zemu("nanox", async (sim, eth) => {
//     const contract = new ethers.Contract(contractAddr, ['function provide_liquidity(uint256[3],uint256)']);
//     const {data} = await contract.populateTransaction.provide_liquidity([DAI_TO_DEPOSIT, USDC_TO_DEPOSIT, USDC_TO_DEPOSIT], MIN_AMOUNT);
//     let unsignedTx = genericTx;
//     unsignedTx.to = contractAddr;
//     unsignedTx.data = data;
  
//     const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
//     const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);
  
//     await waitForAppScreen(sim);
//     await sim.navigateAndCompareSnapshots('.', 'nanox_curve_add_liquidity_3', [BASE_SCREENS_X, 0]);
//     await tx;
//   }, NETWORK));
