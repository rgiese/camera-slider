import { BluetoothCapabilitiesStore } from "./Bluetooth/BluetoothCapabilitiesStore";
import { BluetoothConnection } from "./Bluetooth/BluetoothConnection";
import { BluetoothProgramStore } from "./Bluetooth/BluetoothProgramStore";
import { BluetoothStatusStore } from "./Bluetooth/BluetoothStatusStore";
import { BluetoothTrackingStore } from "./Bluetooth/BluetoothTrackingStore";

export class RootStore {
  public readonly bluetoothConnection: BluetoothConnection;

  public readonly bluetoothCapabilitiesStore: BluetoothCapabilitiesStore;

  public readonly bluetoothProgramStore: BluetoothProgramStore;

  public readonly bluetoothStatusStore: BluetoothStatusStore;

  public readonly bluetoothTrackingStore: BluetoothTrackingStore;

  public constructor() {
    this.bluetoothConnection = new BluetoothConnection();
    this.bluetoothCapabilitiesStore = new BluetoothCapabilitiesStore(this.bluetoothConnection);
    this.bluetoothProgramStore = new BluetoothProgramStore(this.bluetoothConnection);
    this.bluetoothStatusStore = new BluetoothStatusStore(this.bluetoothConnection);
    this.bluetoothTrackingStore = new BluetoothTrackingStore(this.bluetoothConnection);
  }
}
