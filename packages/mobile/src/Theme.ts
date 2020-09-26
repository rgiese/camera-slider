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
};

export default appTheme;
