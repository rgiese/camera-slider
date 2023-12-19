import {
  NavigationScreenConfigProps,
  createAppContainer,
  createSwitchNavigator,
} from "react-navigation";
import {
  NavigationStackOptions,
  NavigationStackProp,
  createStackNavigator,
} from "react-navigation-stack";

import ConnectScreen from "./ConnectScreen";
import HomeScreen from "./HomeScreen";
import ScreenProps from "./ScreenProps";
import ScreenRoutes from "./ScreenRoutes";
import SettingsScreen from "./SettingsScreen";

// App screens
const AppNavigator = createStackNavigator(
  {
    [ScreenRoutes.Home]: { screen: HomeScreen },
    [ScreenRoutes.Settings]: { screen: SettingsScreen },
  },
  {
    initialRouteName: ScreenRoutes.Home,
    defaultNavigationOptions: (
      navigationOptionsContainer: NavigationScreenConfigProps<NavigationStackProp>
    ): NavigationStackOptions => {
      const screenProps = navigationOptionsContainer.screenProps as ScreenProps;
      const colors = screenProps.theme.colors;

      return {
        headerStyle: {
          backgroundColor: colors.background,
        },
        headerTintColor: colors.primary,
      };
    },
  }
);

// Connect flow screens
const ConnectNavigator = createStackNavigator(
  {
    [ScreenRoutes.Connect]: ConnectScreen,
  },
  {
    defaultNavigationOptions: {
      headerShown: false,
    },
  }
);

// Stitch together app and auth flow
const RootNavigator = createSwitchNavigator(
  {
    App: AppNavigator,
    Connect: ConnectNavigator,
  },
  {
    initialRouteName: ScreenRoutes.Connect,
  }
);

// Create top-level container
const NavigationAppContainer = createAppContainer(RootNavigator);

export default NavigationAppContainer;
