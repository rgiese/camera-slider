import { BluetoothConnection } from "./Bluetooth/BluetoothConnection";
import { BluetoothStatusStore } from "./Bluetooth/BluetoothStatusStore";
import { BluetoothTrackingStore } from "./Bluetooth/BluetoothTrackingStore";

export class RootStore {
  public readonly bluetoothConnection: BluetoothConnection;

  public readonly bluetoothStatusStore: BluetoothStatusStore;

  public readonly bluetoothTrackingStore: BluetoothTrackingStore;

  public constructor() {
    this.bluetoothConnection = new BluetoothConnection();
    this.bluetoothStatusStore = new BluetoothStatusStore(this.bluetoothConnection);
    this.bluetoothTrackingStore = new BluetoothTrackingStore(this.bluetoothConnection);
  }
}
