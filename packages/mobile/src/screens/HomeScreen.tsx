import { IconButton, Text } from "react-native-paper";
import { NavigationRoute, NavigationScreenProp } from "react-navigation";
import { NavigationStackOptions, NavigationStackScreenComponent } from "react-navigation-stack";

import BaseView from "../components/BaseView";
import React from "react";
import ScreenProps from "./ScreenProps";
import ScreenRoutes from "./ScreenRoutes";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

/* ESLint believes the headerRight created below is a component. Alas. */
/* eslint-disable react/no-multi-comp */

/* ESLint also wants to see HomeScreen as a proper function component. */
/* This NavigationStack stuff is too weird to mess with it. Alas. */
/* eslint-disable react/function-component-definition */

const HomeScreen: NavigationStackScreenComponent<{}> = ({ navigation }): React.ReactElement => {
  const bluetoothStore = useRootStore().bluetoothStore;

  if (!bluetoothStore.isConnected) {
    setImmediate(() => {
      // Can only run this once render is done
      navigation.navigate(ScreenRoutes.Connect);
    });

    return <BaseView />;
  }

  return (
    <BaseView>
      <Text>{bluetoothStore.deviceStatus}</Text>
    </BaseView>
  );
};

HomeScreen.navigationOptions = ({
  navigation,
  screenProps,
}: {
  navigation: NavigationScreenProp<NavigationRoute<{}>, {}>;
  screenProps: any;
}): NavigationStackOptions => ({
  title: "Home",
  headerRight: function(): React.ReactNode {
    return (
      <IconButton
        color={(screenProps as ScreenProps).theme.colors.text}
        icon="account"
        onPress={(): void => {
          navigation.navigate(ScreenRoutes.Home); // TEMPORARY
        }}
      />
    );
  },
});

export default observer(HomeScreen);
