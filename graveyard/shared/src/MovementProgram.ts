export type MovementType = "Move" | "Delay";

export interface Movement {
  Type: MovementType;

  // Type = Move
  DesiredPosition?: number;
  DesiredSpeed?: number;
  DesiredAcceleration?: number;

  // Type = Delay
  DelayTime?: number;
}

export interface MovementProgram {
  Rate: number;
  Repeats: boolean;
  Movements: Movement[];
}

export function dumpMovementProgram(movementProgram: MovementProgram): void {
  console.log(
    `Rate: ${movementProgram.Rate}, repeats: ${movementProgram.Repeats.toString()}, #movements: ${
      movementProgram.Movements.length
    }`
  );

  movementProgram.Movements.forEach(movement => {
    switch (movement.Type) {
      case "Move":
        console.log(
          `  - ${movement.Type}: ${movement.DesiredPosition} / ${movement.DesiredSpeed} / ${movement.DesiredAcceleration}`
        );
        break;

      case "Delay":
        console.log(`  - ${movement.Type}: ${movement.DelayTime} msec`);
        break;
    }
  });
}
