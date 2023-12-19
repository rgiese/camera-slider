export const SliderStateNames = [
  "findingForwardRange",
  "homing",
  "initializingMotor",
  "runningMovementProgram",
  "trackingDesiredPosition",
  "unrecoverableError",
] as const;

export type SliderState = typeof SliderStateNames[number];
