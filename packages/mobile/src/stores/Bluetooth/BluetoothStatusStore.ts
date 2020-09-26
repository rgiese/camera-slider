import { Base64DecodeInt32, Base64DecodeString, Base64DecodeUInt32 } from "./Base64";
import { BluetoothServices, SliderState } from "@grumpycorp/camera-slider-shared";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { Device } from "react-native-ble-plx";
import { observable } from "mobx";

export class BluetoothStatusStore extends BluetoothCharacteristicsStoreBase {
  @observable public state: SliderState = "initializingMotor";

  @observable public reportedPosition = 0;

  @observable public reportedVelocity = 0;

  @observable public reportedMaximumSpeed = 0;

  @observable public reportedMaximumAcceleration = 0;

  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Status.Id);

    bluetoothConnection.addCharacteristicsStore(this);
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
      Base64DecodeInt32
    );

    await this.addCharacteristicListener<BluetoothStatusStore, number>(
      device,
      BluetoothServices.Status.Characteristics.ReportedVelocity,
      "reportedVelocity",
      Base64DecodeInt32
    );

    await this.addCharacteristicListener<BluetoothStatusStore, number>(
      device,
      BluetoothServices.Status.Characteristics.ReportedMaximumSpeed,
      "reportedMaximumSpeed",
      Base64DecodeUInt32
    );

    await this.addCharacteristicListener<BluetoothStatusStore, number>(
      device,
      BluetoothServices.Status.Characteristics.ReportedMaximumAcceleration,
      "reportedMaximumAcceleration",
      Base64DecodeUInt32
    );
  }
}
