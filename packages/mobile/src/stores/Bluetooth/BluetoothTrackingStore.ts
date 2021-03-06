import { Base64EncodeInt32, Base64EncodeUInt32 } from "./Base64";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { BluetoothServices } from "@grumpycorp/camera-slider-shared";
import { Device } from "react-native-ble-plx";

export class BluetoothTrackingStore extends BluetoothCharacteristicsStoreBase {
  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Tracking.Id);
  }

  public async setDesiredPosition(desiredPosition: number): Promise<void> {
    await this.writeCharacteristicValue(
      BluetoothServices.Tracking.Characteristics.DesiredPosition,
      Base64EncodeInt32(desiredPosition)
    );
  }

  public async setDesiredMaximumSpeed(desiredMaximumSpeed: number): Promise<void> {
    await this.writeCharacteristicValue(
      BluetoothServices.Tracking.Characteristics.DesiredMaximumSpeed,
      Base64EncodeUInt32(desiredMaximumSpeed)
    );
  }

  public async setDesiredMaximumAcceleration(desiredMaximumAcceleration: number): Promise<void> {
    await this.writeCharacteristicValue(
      BluetoothServices.Tracking.Characteristics.DesiredMaximumAcceleration,
      Base64EncodeUInt32(desiredMaximumAcceleration)
    );
  }

  /* eslint-disable-next-line @typescript-eslint/no-empty-function */
  public async onDeviceConnected(_device: Device): Promise<void> {}
}
