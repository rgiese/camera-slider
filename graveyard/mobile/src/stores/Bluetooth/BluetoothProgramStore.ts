import { Base64Encode, Base64EncodeUInt32 } from "./Base64";
import {
  BluetoothServices,
  Flatbuffers,
  MovementProgram,
  flatbuffers,
} from "@grumpycorp/camera-slider-shared";

import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { Device } from "react-native-ble-plx";

export class BluetoothProgramStore extends BluetoothCharacteristicsStoreBase {
  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Program.Id);
  }

  public async setDesiredMovementProgram(movementProgram: MovementProgram): Promise<void> {
    // Create buffer
    const flatbufferBuilder: flatbuffers.Builder = new flatbuffers.Builder(128); // ...initial guess at size, FIX ME

    // Create movements array
    Flatbuffers.Firmware.MovementProgram.startMovementsVector(
      flatbufferBuilder,
      movementProgram.Movements.length
    );

    // Iterate in reverse since this is how Flatbuffers builds stuff
    movementProgram.Movements.slice()
      .reverse()
      .forEach((movement): void => {
        Flatbuffers.Firmware.Movement.createMovement(
          flatbufferBuilder,
          movement.Type === "Move"
            ? Flatbuffers.Firmware.MovementType.Move
            : Flatbuffers.Firmware.MovementType.Delay,
          0,
          movement.Type === "Delay" ? Math.round(movement.DelayTime ?? 0) : 0,
          movement.Type === "Move" ? Math.round(movement.DesiredPosition ?? 0) : 0,
          movement.Type === "Move" ? Math.round(movement.DesiredSpeed ?? 0) : 0,
          movement.Type === "Move" ? Math.round(movement.DesiredAcceleration ?? 0) : 0
        );
      });

    const movementsVector = flatbufferBuilder.endVector();

    // Start top-level table
    Flatbuffers.Firmware.MovementProgram.startMovementProgram(flatbufferBuilder);

    // Set top-level fields
    if (movementProgram.Repeats) {
      Flatbuffers.Firmware.MovementProgram.addFlags(
        flatbufferBuilder,
        Flatbuffers.Firmware.MovementProgramFlags.Repeat
      );
    }

    Flatbuffers.Firmware.MovementProgram.addRate(
      flatbufferBuilder,
      Math.round(movementProgram.Rate)
    );

    Flatbuffers.Firmware.MovementProgram.addMovements(flatbufferBuilder, movementsVector);

    // Finish top-level table
    const movementProgramOffset = Flatbuffers.Firmware.MovementProgram.endMovementProgram(
      flatbufferBuilder
    );

    Flatbuffers.Firmware.MovementProgram.finishMovementProgramBuffer(
      flatbufferBuilder,
      movementProgramOffset
    );

    // Extract and encode
    const movementProgramBytes = flatbufferBuilder.asUint8Array();

    await this.writeCharacteristicValue(
      BluetoothServices.Program.Characteristics.DesiredMovementProgram,
      Base64Encode(movementProgramBytes)
    );
  }

  public async startMovementProgram(atStep: number): Promise<void> {
    await this.writeCharacteristicValue(
      BluetoothServices.Program.Characteristics.StartMovementProgram,
      Base64EncodeUInt32(atStep)
    );
  }

  public async stopMovementProgram(): Promise<void> {
    await this.writeCharacteristicValue(
      BluetoothServices.Program.Characteristics.StopMovementProgram,
      ""
    );
  }

  /* eslint-disable-next-line @typescript-eslint/no-empty-function */
  public async onDeviceConnected(_device: Device): Promise<void> {}
}
