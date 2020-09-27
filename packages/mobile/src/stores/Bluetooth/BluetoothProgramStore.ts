import {
  BluetoothServices,
  Flatbuffers,
  MovementProgram,
  flatbuffers,
} from "@grumpycorp/camera-slider-shared";

import { Base64Encode } from "./Base64";
import { BluetoothCharacteristicsStoreBase } from "./BluetoothCharacteristicsStoreBase";
import { BluetoothConnection } from "./BluetoothConnection";
import { Device } from "react-native-ble-plx";

export class BluetoothProgramStore extends BluetoothCharacteristicsStoreBase {
  public constructor(bluetoothConnection: BluetoothConnection) {
    super(bluetoothConnection, BluetoothServices.Program.Id);
  }

  public async test(): Promise<void> {
    const movementProgram: MovementProgram = {
      Rate: 1,
      Repeats: true,
      Movements: [
        {
          Type: "Move",
          DesiredPosition: 0,
          DesiredSpeed: 1000,
          DesiredAcceleration: 1000,
        },
        {
          Type: "Move",
          DesiredPosition: 2000,
          DesiredSpeed: 1000,
          DesiredAcceleration: 1000,
        },
        {
          Type: "Delay",
          DelayTime: 1000,
        },
      ],
    };

    // Create buffer
    const flatbufferBuilder: flatbuffers.Builder = new flatbuffers.Builder(64); // ...initial guess at size, FIX ME

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

    Flatbuffers.Firmware.MovementProgram.addRateMinus1X100(
      flatbufferBuilder,
      Math.round((movementProgram.Rate - 1) * 100)
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

  /* eslint-disable-next-line @typescript-eslint/no-empty-function */
  public async onDeviceConnected(_device: Device): Promise<void> {}
}
