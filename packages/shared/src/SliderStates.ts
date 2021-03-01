export const SliderStateNames = [
  "findingForwardRange",
  "homing",
  "initializingMotor",
  "runningMovementProgram",
  "trackingDesiredPosition",
  "unrecoverableError",
  "calibrateTouchStart",
  "calibrateTouchTopLeft",
  "calibrateTouchTopRight",
  "calibrateTouchBottomLeft",
  "calibrateTouchBottomRight",
] as const;

export type SliderState = typeof SliderStateNames[number];
