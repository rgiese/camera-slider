export const SliderStateNames = [
  "findingForwardRange",
  "homing",
  "initializingMotor",
  "trackingDesiredPosition",
  "unrecoverableError",
] as const;

export type SliderState = typeof SliderStateNames[number];
