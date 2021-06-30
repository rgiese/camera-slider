type Uuid = string;

interface BluetoothServices {
  [serviceName: string]: {
    Id: Uuid;
    Characteristics: {
      [characteristicName: string]: Uuid;
    };
  };
}

export const BluetoothServices: BluetoothServices = {
  Capabilities: {
    Id: "6f95f8ad-ce8f-4517-a4f6-445f48de3b32",
    Characteristics: {
      MaximumPosition: "26bf9446-4d04-4c49-92a6-dad62302d9e3",
      MaximumSpeed: "ac607c74-6a7b-47e3-99bb-da7b36268b2c",
      MaximumAcceleration: "e24e1773-60e9-4287-86a0-eddd7d7ce2fd",
      StartTouchscreenCalibration: "ae65050b-e6cf-4cfa-adb7-92623e18d29d",
      TouchscreenCalibrationData: "24576982-5229-4317-91df-ed1377e6e602",
    },
  },
  Program: {
    Id: "e2a99562-a5ae-4d6e-96e8-5a5fe2213290",
    Characteristics: {
      DesiredMovementProgram: "0eeb387c-5bc1-4c00-b9ee-066643d11a82",
      StartMovementProgram: "3cdf2a86-f08f-48e2-b25e-c9edba067ed4",
      StopMovementProgram: "1f64ff50-d45b-4636-96c0-0c32da407cb3",
    },
  },
  Status: {
    Id: "fcccbeb7-eb63-4726-9315-e198b1e5ec1c",
    Characteristics: {
      State: "dc5d99b0-303c-45c2-b7a2-af6baadc0388",
      ReportedPosition: "0a5f29a0-5184-4961-85bc-8aa78ec98bd1",
      ReportedVelocity: "6d933277-dd04-4430-8ea6-3a2267c33364",
      ReportedMaximumSpeed: "73e3d2aa-a863-4f3d-9e85-c1cdd5a857e9",
      ReportedMaximumAcceleration: "f76b0acd-eb2b-4230-8006-92ccd637658f",
      ReportedMovementProgram: "24aadf24-5835-4ed5-9a6f-3c07a00a8b88",
    },
  },
  Tracking: {
    Id: "3daa0a65-6ee2-4c26-add4-31e089be2ead",
    Characteristics: {
      DesiredPosition: "28449837-bc47-489a-bbda-215a9d744728",
      DesiredMaximumSpeed: "03fe44f9-6ba8-4e84-b1a7-ec647c151666",
      DesiredMaximumAcceleration: "47350362-2692-481e-a5a5-99ad2331a5e4",
    },
  },
};
