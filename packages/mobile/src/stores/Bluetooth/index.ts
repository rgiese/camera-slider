import { BleManager, Device } from "react-native-ble-plx";
import { action, computed, observable } from "mobx";

import { Base64EncodeUInt32 } from "./Base64";
import { BluetoothCharacteristicsStore } from "./Characteristics";
import { BluetoothServices } from "@grumpycorp/camera-slider-shared";

export type BluetoothStoreState =
  | "initializing"
  | "disconnected"
  | "connecting"
  | "connected"
  | "error";

export class BluetoothStore extends BluetoothCharacteristicsStore {
  @observable public state: BluetoothStoreState = "initializing";

  public error?: string;

  private readonly bleManager: BleManager;

  private device?: Device;

  public constructor() {
    super();

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

  @action private setState(state: BluetoothStoreState): void {
    console.log(`BluetoothStore: ${this.state} -> ${state}`);

    this.state = state;
  }

  public async setDesiredPosition(desiredPosition: number): Promise<void> {
    if (!this.device) {
      throw new Error("Bluetooth not connected.");
    }

    await this.device.writeCharacteristicWithoutResponseForService(
      BluetoothServices.Tracking.Id,
      BluetoothServices.Tracking.Characteristics.DesiredPosition,
      Base64EncodeUInt32(desiredPosition)
    );
  }

  protected async onDeviceConnected(device: Device): Promise<void> {
    // Connect
    await device.connect();

    await device.discoverAllServicesAndCharacteristics();

    // Set up subscriptions and perform initial reads
    await super.onDeviceConnected(device);
  }

  protected setError(error: string): void {
    console.log(`BluetoothStore error: ${error}`);

    this.error = error;
    this.setState("error");
  }
}
