import { BluetoothStore } from "./BluetoothStore";

export class RootStore {
  public readonly bluetoothStore: BluetoothStore;

  public constructor() {
    this.bluetoothStore = new BluetoothStore();
  }
}
