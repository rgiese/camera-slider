import { Base64Decode, Base64DecodeInt32, Base64DecodeString, Base64DecodeUInt32 } from "./Base64";
import {
  BluetoothServices,
  Flatbuffers,
  MovementProgram,
  SliderState,
  flatbuffers,
} from "@grumpycorp/camera-slider-shared";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { Device } from "react-native-ble-plx";
import { observable } from "mobx";

function Base64DecodeMovementProgram(source: string): MovementProgram {
  const flatbufferData = Base64Decode(source);
  const flatbufferBuffer = new flatbuffers.ByteBuffer(flatbufferData);

  const flatbufferMovementProgram = Flatbuffers.Firmware.MovementProgram.getRootAsMovementProgram(
    flatbufferBuffer
  );

  const movementProgram: MovementProgram = {
    Rate: flatbufferMovementProgram.rate(),
    Repeats: !!(
      flatbufferMovementProgram.flags() & Flatbuffers.Firmware.MovementProgramFlags.Repeat
    ),
    Movements: [],
  };

  for (
    let idxMovement = 0;
    idxMovement < flatbufferMovementProgram.movementsLength();
    ++idxMovement
  ) {
    const flatbufferMovement = flatbufferMovementProgram.movements(idxMovement);

    if (flatbufferMovement) {
      switch (flatbufferMovement.type()) {
        case Flatbuffers.Firmware.MovementType.Move:
          movementProgram.Movements.push({
            Type: "Move",
            DesiredPosition: flatbufferMovement.desiredPosition(),
            DesiredSpeed: flatbufferMovement.desiredSpeed(),
            DesiredAcceleration: flatbufferMovement.desiredAcceleration(),
          });
          break;

        case Flatbuffers.Firmware.MovementType.Delay:
          movementProgram.Movements.push({
            Type: "Delay",
            DelayTime: flatbufferMovement.delayTime(),
          });
          break;

        default:
          // Ignore
          break;
      }
    }
  }

  return movementProgram;
}

export class BluetoothStatusStore extends BluetoothCharacteristicsStoreBase {
  @observable public state: SliderState = "initializingMotor";

  @observable public reportedMovementProgram: MovementProgram = {
    Rate: 100,
    Repeats: false,
    Movements: [],
  };

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

    await this.addCharacteristicListener<BluetoothStatusStore, MovementProgram>(
      device,
      BluetoothServices.Status.Characteristics.ReportedMovementProgram,
      "reportedMovementProgram",
      Base64DecodeMovementProgram
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
