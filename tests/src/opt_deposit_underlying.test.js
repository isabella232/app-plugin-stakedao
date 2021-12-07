import "core-js/stable";
import "regenerator-runtime/runtime";
import { waitForAppScreen, zemu, genericTx } from './test.fixture';
import { ethers } from "ethers";

// StakeDAO ETH Call option 
const contractAddr = "0x227e4635c5fe22d1e36dab1c921b62f8acc451b9";
const AMOUNT_TO_DEPOSIT = '100259000000000000000'; // 100.25
const MIN_RECEIVED = '0';
const BASE_SCREENS_S = (1 + 2 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE
const BASE_SCREENS_X = (1 + 1 + 1 + 1 + 1) //STAKEDAO + AMOUNT + GAS_FEES + VAULT + APPROVE

test('[Nano S] Deposit underlying into options', zemu("nanos", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function depositUnderlying(uint256,uint256)']);
  const {data} = await contract.populateTransaction.depositUnderlying(AMOUNT_TO_DEPOSIT, MIN_RECEIVED);
  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanos_opt_deposit_under', [BASE_SCREENS_S, 0]);
  await tx;
}));

test('[Nano X] Deposit underlying into options', zemu("nanox", async (sim, eth) => {
  const contract = new ethers.Contract(contractAddr, ['function depositUnderlying(uint256,uint256)']);
  const {data} = await contract.populateTransaction.depositUnderrlying(AMOUNT_TO_DEPOSIT, MIN_RECEIVED);

  let unsignedTx = genericTx;
  unsignedTx.to = contractAddr;
  unsignedTx.data = data;

  const serializedTx = ethers.utils.serializeTransaction(unsignedTx).slice(2);
  const tx = eth.signTransaction("44'/60'/0'/0", serializedTx);

  await waitForAppScreen(sim);
  await sim.navigateAndCompareSnapshots('.', 'nanox_opt_deposit_under', [BASE_SCREENS_X, 0]);
  await tx;
}));