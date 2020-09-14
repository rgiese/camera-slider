import { BluetoothStore } from "./Bluetooth";

export class RootStore {
  public readonly bluetoothStore: BluetoothStore;

  public constructor() {
    this.bluetoothStore = new BluetoothStore();
  }
}
