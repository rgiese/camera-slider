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
  Status: {
    Id: "fcccbeb7-eb63-4726-9315-e198b1e5ec1c",
    Characteristics: {
      State: "dc5d99b0-303c-45c2-b7a2-af6baadc0388",
      ReportedPosition: "0a5f29a0-5184-4961-85bc-8aa78ec98bd1",
      DesiredPosition: "28449837-bc47-489a-bbda-215a9d744728",
    },
  },
  Capabilities: {
    Id: "6f95f8ad-ce8f-4517-a4f6-445f48de3b32",
    Characteristics: {
      MaximumSpeed: "ac607c74-6a7b-47e3-99bb-da7b36268b2c",
      MaximumAcceleration: "e24e1773-60e9-4287-86a0-eddd7d7ce2fd",
      MaximumPosition: "26bf9446-4d04-4c49-92a6-dad62302d9e3",
    },
  },
};
