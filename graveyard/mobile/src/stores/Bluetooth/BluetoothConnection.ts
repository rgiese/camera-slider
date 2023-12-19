import { BleManager, Device } from "react-native-ble-plx";
import { action, computed, observable } from "mobx";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothServices } from "@grumpycorp/camera-slider-shared";

export type BluetoothConnectionState =
  | "initializing"
  | "disconnected"
  | "connecting"
  | "connected"
  | "error";

export class BluetoothConnection {
  @observable public state: BluetoothConnectionState = "initializing";

  public error?: string;

  private readonly bleManager: BleManager;

  private device?: Device;

  private readonly characteristicsStores: BluetoothCharacteristicsStoreBase[] = [];

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

  @action public setError(error: string): void {
    console.log(`BluetoothStore error: ${error}`);

    this.error = error;
    this.setState("error");
  }

  @computed public get isConnected(): boolean {
    return this.state === "connected";
  }

  @action public connect(): void {
    this.setState("connecting");

    this.bleManager.startDeviceScan(
      // Find device by service
      [BluetoothServices.Status.Id],
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
              this.device = device;
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

  @action private setState(state: BluetoothConnectionState): void {
    console.log(`BluetoothConnection: ${this.state} -> ${state}`);

    this.state = state;
  }

  public addCharacteristicsStore(characteristicsStore: BluetoothCharacteristicsStoreBase): void {
    this.characteristicsStores.push(characteristicsStore);
  }

  public async writeCharacteristicValue(
    serviceId: string,
    characteristicId: string,
    value: string
  ): Promise<void> {
    if (!this.device) {
      throw new Error("Bluetooth not connected.");
    }

    await this.device.writeCharacteristicWithoutResponseForService(
      serviceId,
      characteristicId,
      value
    );
  }

  private async onDeviceConnected(device: Device): Promise<void> {
    // Connect
    await device.connect();

    await device.discoverAllServicesAndCharacteristics();

    // Set up subscriptions and perform initial reads
    for (const characteristicsStore of this.characteristicsStores) {
      await characteristicsStore.onDeviceConnected(device);
    }
  }
}
