import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO USD Vault
const contractAddr = "0xb17640796e4c27a39af51887aff3f8dc0daf9567";
const AMOUNT_TO_WITHDRAW = '100259000000000000000'; // 100.259
const BASE_SCREENS_S = (1 + 2 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

// Nanos S test
test('[Nano S] Withdraw Tokens from vault', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function withdraw(uint256)']);
  const {data} = await contract.populateTransaction.withdraw(AMOUNT_TO_WITHDRAW);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_vault_withdraw', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Withdraw Tokens from vault', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function withdraw(uint256)']);
  const {data} = await contract.populateTransaction.withdraw(AMOUNT_TO_WITHDRAW);

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_vault_withdraw', [BASE_SCREENS_X, 0]);
  await tx;
}));