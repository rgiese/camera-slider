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
