import { BluetoothConnection } from "./Bluetooth/BluetoothConnection";
import { BluetoothStatusStore } from "./Bluetooth/BluetoothStatusStore";

export class RootStore {
  public readonly bluetoothConnection: BluetoothConnection;

  public readonly bluetoothStatusStore: BluetoothStatusStore;

  public constructor() {
    this.bluetoothConnection = new BluetoothConnection();
    this.bluetoothStatusStore = new BluetoothStatusStore(this.bluetoothConnection);
  }
}
