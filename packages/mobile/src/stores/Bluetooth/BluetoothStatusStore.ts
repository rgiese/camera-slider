import {
  Base64DecodeInt32,
  Base64DecodeString,
  Base64DecodeUInt32,
  Base64EncodeUInt32,
} from "./Base64";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { BluetoothServices } from "@grumpycorp/camera-slider-shared";
import { Device } from "react-native-ble-plx";
import { observable } from "mobx";

export class BluetoothStatusStore extends BluetoothCharacteristicsStoreBase {
  @observable public state = "";

  @observable public reportedPosition = 0;

  @observable public reportedVelocity = 0;

  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Status.Id);

    bluetoothConnection.addCharacteristicsStore(this);
  }

  public async setDesiredPosition(desiredPosition: number): Promise<void> {
    await this.bluetoothConnection.writeCharacteristicValue(
      BluetoothServices.Tracking.Id,
      BluetoothServices.Tracking.Characteristics.DesiredPosition,
      Base64EncodeUInt32(desiredPosition)
    );
  }

  public async onDeviceConnected(device: Device): Promise<void> {
    await this.addCharacteristicListener<BluetoothStatusStore, string>(
      device,
      BluetoothServices.Status.Characteristics.State,
      "state",
      Base64DecodeString
    );

    await this.addCharacteristicListener<BluetoothStatusStore, number>(
      device,
      BluetoothServices.Status.Characteristics.ReportedPosition,
      "reportedPosition",
      Base64DecodeUInt32
    );

    await this.addCharacteristicListener<BluetoothStatusStore, number>(
      device,
      BluetoothServices.Status.Characteristics.ReportedVelocity,
      "reportedVelocity",
      Base64DecodeInt32
    );
  }
}
