import { BleError, Characteristic, Device } from "react-native-ble-plx";

import { BluetoothConnection } from "./BluetoothConnection";
import { action } from "mobx";

type KeysOfType<T, TProp> = { [P in keyof T]: T[P] extends TProp ? P : never }[keyof T];

type Base64DecoderFunction<T> = (source: string) => T;

export abstract class BluetoothCharacteristicsStoreBase {
  protected readonly bluetoothConnection: BluetoothConnection;

  private readonly serviceUuid: string;

  public constructor(bluetoothConnection: BluetoothConnection, serviceUuid: string) {
    this.bluetoothConnection = bluetoothConnection;
    this.serviceUuid = serviceUuid;
  }

  protected async addCharacteristicListener<TDerived extends BluetoothCharacteristicsStoreBase, T>(
    this: TDerived,
    device: Device,
    characteristicUuid: string,
    targetProperty: KeysOfType<TDerived, T>,
    decoderFunction: Base64DecoderFunction<T>
  ): Promise<void> {
    // Find characteristic
    const deviceCharacteristic = await device.readCharacteristicForService(
      this.serviceUuid,
      characteristicUuid
    );

    // Build update function
    const onCharacteristicUpdated = action(
      (error: BleError | null, characteristic: Characteristic | null): void => {
        if (error) {
          this.bluetoothConnection.setError(JSON.stringify(error));
          return;
        }

        if (!characteristic) {
          this.bluetoothConnection.setError(`No characteristic provided for ${characteristicUuid}`);
          return;
        }

        const encodedValue = characteristic.value;

        if (encodedValue) {
          (this[targetProperty] as any) = decoderFunction(encodedValue);
        }
      }
    );

    // Invoke update function to retrieve initial value
    onCharacteristicUpdated(null, deviceCharacteristic);

    // Monitor value
    deviceCharacteristic.monitor(onCharacteristicUpdated);
  }

  protected async writeCharacteristicValue(characteristicId: string, value: string): Promise<void> {
    await this.bluetoothConnection.writeCharacteristicValue(
      this.serviceUuid,
      characteristicId,
      value
    );
  }

  public abstract async onDeviceConnected(device: Device): Promise<void>;
}
