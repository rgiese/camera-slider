import { DarkTheme, Theme } from "react-native-paper";

const baseTheme = DarkTheme;

const appTheme: Theme = {
  ...baseTheme,
  colors: {
    ...baseTheme.colors,
    primary: "#f26739",
    accent: "#999",
  },
};

export const Colors = {
  Position: "#f26739",
  Speed: "#f52c68",
  Acceleration: "#e0ca3e",
  Rate: "#0cf2bd",
  Repeat: "#00a67f",
  Delay: "#0cf2bd",
  Text: {
    Dim: "#999",
  },
};

export const Icons = {
  Position: "map-marker",
  Speed: "speedometer",
  Acceleration: "trending-up",
  Rate: "run-fast",
  Repeat: "repeat",
  Delay: "update",
  Edit: "pencil",
  Delete: "delete",
};

export default appTheme;
