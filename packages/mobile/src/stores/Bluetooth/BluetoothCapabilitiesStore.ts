import { Base64DecodeInt32, Base64DecodeUInt32 } from "./Base64";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { BluetoothServices } from "@grumpycorp/camera-slider-shared";
import { Device } from "react-native-ble-plx";
import { observable } from "mobx";

export class BluetoothCapabilitiesStore extends BluetoothCharacteristicsStoreBase {
  @observable public maximumPosition = 0;

  @observable public maximumSpeed = 0;

  @observable public maximumAcceleration = 0;

  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Capabilities.Id);

    bluetoothConnection.addCharacteristicsStore(this);
  }

  public async onDeviceConnected(device: Device): Promise<void> {
    await this.addCharacteristicListener<BluetoothCapabilitiesStore, number>(
      device,
      BluetoothServices.Capabilities.Characteristics.MaximumPosition,
      "maximumPosition",
      Base64DecodeInt32
    );

    await this.addCharacteristicListener<BluetoothCapabilitiesStore, number>(
      device,
      BluetoothServices.Capabilities.Characteristics.MaximumSpeed,
      "maximumSpeed",
      Base64DecodeUInt32
    );

    await this.addCharacteristicListener<BluetoothCapabilitiesStore, number>(
      device,
      BluetoothServices.Capabilities.Characteristics.MaximumAcceleration,
      "maximumAcceleration",
      Base64DecodeUInt32
    );
  }
}
