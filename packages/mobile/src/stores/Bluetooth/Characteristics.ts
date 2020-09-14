import { Base64DecodeInt32, Base64DecodeString, Base64DecodeUInt32 } from "./Base64";
import { BleError, Characteristic, Device } from "react-native-ble-plx";
import { action, observable } from "mobx";

import { BluetoothServices } from "@grumpycorp/camera-slider-shared";

type KeysOfType<T, TProp> = { [P in keyof T]: T[P] extends TProp ? P : never }[keyof T];

type Base64DecoderFunction<T> = (source: string) => T;

export abstract class BluetoothCharacteristicsStore {
  @observable public deviceState = "";

  @observable public reportedPosition = 0;

  @observable public reportedVelocity = 0;

  protected async addCharacteristicListener<T>(
    device: Device,
    serviceUuid: string,
    characteristicUuid: string,
    targetProperty: KeysOfType<BluetoothCharacteristicsStore, T>,
    decoderFunction: Base64DecoderFunction<T>
  ): Promise<void> {
    // Find characteristic
    const deviceCharacteristic = await device.readCharacteristicForService(
      serviceUuid,
      characteristicUuid
    );

    // Build update function
    const onCharacteristicUpdated = action(
      (error: BleError | null, characteristic: Characteristic | null): void => {
        if (error) {
          this.setError(JSON.stringify(error));
          return;
        }

        if (!characteristic) {
          this.setError(`No characteristic provided for ${characteristicUuid}`);
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

  protected async onDeviceConnected(device: Device): Promise<void> {
    await this.addCharacteristicListener(
      device,
      BluetoothServices.Status.Id,
      BluetoothServices.Status.Characteristics.State,
      "deviceState",
      Base64DecodeString
    );

    await this.addCharacteristicListener(
      device,
      BluetoothServices.Status.Id,
      BluetoothServices.Status.Characteristics.ReportedPosition,
      "reportedPosition",
      Base64DecodeUInt32
    );

    await this.addCharacteristicListener(
      device,
      BluetoothServices.Status.Id,
      BluetoothServices.Status.Characteristics.ReportedVelocity,
      "reportedVelocity",
      Base64DecodeInt32
    );
  }

  protected abstract setError(error: string): void;
}
