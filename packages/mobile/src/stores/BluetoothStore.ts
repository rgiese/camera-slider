import { Base64DecodeString, Base64DecodeUInt32 } from "../Base64";
import { BleError, BleManager, Characteristic, Device } from "react-native-ble-plx";
import { action, computed, observable } from "mobx";

import { BluetoothStatusService } from "@grumpycorp/camera-slider-shared";

export type BluetoothStoreState =
  | "initializing"
  | "disconnected"
  | "connecting"
  | "connected"
  | "error";

export class BluetoothStore {
  @observable public state: BluetoothStoreState = "initializing";

  @observable public deviceState = "";

  @observable public reportedPosition = 0;

  public error?: string;

  private readonly bleManager: BleManager;

  public constructor() {
    // TODO: https://reactnative.dev/docs/permissionsandroid
    // to request permissions on app startup
    this.bleManager = new BleManager();

    const subscription = this.bleManager.onStateChange(state => {
      if (state === "PoweredOn") {
        this.setState("disconnected");
        subscription.remove();
      }
    }, true);
  }

  @computed public get isConnected(): boolean {
    return this.state === "connected";
  }

  @action public connect(): void {
    this.setState("connecting");

    this.bleManager.startDeviceScan(
      // Find device by service
      [BluetoothStatusService.serviceUuid],
      null,
      (error, device) => {
        try {
          if (error) {
            throw error;
          }

          if (!device) {
            throw new Error("No device provided");
          }

          this.bleManager.stopDeviceScan();

          /* eslint-disable-next-line promise/no-promise-in-callback */
          this.onDeviceConnected(device)
            .then((): void => {
              return this.setState("connected");
            })
            .catch((reason: any) => {
              throw new Error(reason);
            });
        } catch (error) {
          this.setError(JSON.stringify(error)); // stringify exception
        }
      }
    );
  }

  /* eslint-disable @typescript-eslint/no-invalid-this */
  @action private readonly onStatusCharacteristicUpdated = (
    error: BleError | null,
    characteristic: Characteristic | null
  ): void => {
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
      this.deviceState = Base64DecodeString(encodedValue);
    }
  };

  /* eslint-disable @typescript-eslint/no-invalid-this */
  @action private readonly onReportedPositionCharacteristicUpdated = (
    error: BleError | null,
    characteristic: Characteristic | null
  ): void => {
    if (error) {
      this.setError(JSON.stringify(error));
      return;
    }

    if (!characteristic) {
      this.setError("Reported position characteristic: no characteristic provided");
      return;
    }

    const encodedValue = characteristic.value;

    if (encodedValue) {
      this.reportedPosition = Base64DecodeUInt32(encodedValue);
    }
  };

  @action private setState(state: BluetoothStoreState): void {
    console.log(`BluetoothStore: ${this.state} -> ${state}`);

    this.state = state;
  }

  private async onDeviceConnected(device: Device): Promise<void> {
    // Connect
    await device.connect();

    await device.discoverAllServicesAndCharacteristics();

    // Set up subscriptions and perform initial reads
    const statusCharacteristic = await device.readCharacteristicForService(
      BluetoothStatusService.serviceUuid,
      BluetoothStatusService.stateCharacteristicUuid
    );

    this.onStatusCharacteristicUpdated(null, statusCharacteristic);
    statusCharacteristic.monitor(this.onStatusCharacteristicUpdated);

    const reportedPositionCharacteristic = await device.readCharacteristicForService(
      BluetoothStatusService.serviceUuid,
      BluetoothStatusService.reportedPositionCharacteristicUuid
    );

    this.onReportedPositionCharacteristicUpdated(null, reportedPositionCharacteristic);
    reportedPositionCharacteristic.monitor(this.onReportedPositionCharacteristicUpdated);
  }

  private setError(error: string): void {
    console.log(`BluetoothStore error: ${error}`);

    this.error = error;
    this.setState("error");
  }
}
