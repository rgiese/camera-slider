import { BleError, BleManager, Characteristic } from "react-native-ble-plx";
import { action, observable } from "mobx";

import { StoreBase } from "./StoreBase";
import base64 from "react-native-base64";

export class BluetoothStore extends StoreBase {
  @observable public deviceStatus = "";

  private readonly bleManager: BleManager;

  public constructor() {
    super("Bluetooth");

    this.setState("initializing");

    // TODO: https://reactnative.dev/docs/permissionsandroid
    // to request permissions on app startup
    this.bleManager = new BleManager();

    const subscription = this.bleManager.onStateChange(state => {
      if (state === "PoweredOn") {
        this.setState("ready");
        subscription.remove();
      }
    }, true);
  }

  @action public scanAndConnect() {
    this.setState("fetching");

    this.bleManager.startDeviceScan(
      ["fcccbeb7-eb63-4726-9315-e198b1e5ec1c"],
      null,
      async (error, device) => {
        try {
          if (error) {
            throw error ?? new Error("Unknown Bluetooth error");
          }

          if (!device) {
            throw new Error("No device provided");
          }

          this.bleManager.stopDeviceScan();

          const connectedDevice = await (
            await device.connect()
          ).discoverAllServicesAndCharacteristics();

          connectedDevice.monitorCharacteristicForService(
            "fcccbeb7-eb63-4726-9315-e198b1e5ec1c",
            "dc5d99b0-303c-45c2-b7a2-af6baadc0388",
            this.onStatusCharacteristicUpdated
          );

          this.setState("ready");
        } catch (error) {
          this.setError(JSON.stringify(error)); // stringify exception
        }
      }
    );
  }

  @action private readonly onStatusCharacteristicUpdated = (
    error: BleError | null,
    characteristic: Characteristic | null
  ) => {
    if (error) {
      this.setError(JSON.stringify(error));
      return;
    }

    if (!characteristic) {
      this.setError("Status characteristic: no characteristic provided");
      return;
    }

    const encodedValue = characteristic.value;

    if (encodedValue) {
      this.deviceStatus = base64.decode(encodedValue);
    }
  };
}
