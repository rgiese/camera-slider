import { NavigationRoute, NavigationScreenProp } from "react-navigation";
import { NavigationStackOptions, NavigationStackScreenComponent } from "react-navigation-stack";
import { ScrollView, StyleSheet } from "react-native";

import BaseView from "../components/BaseView";
import { Colors } from "../Theme";
import { IconButton } from "react-native-paper";
import { List } from "react-native-paper";
import React from "react";
import ScreenProps from "./ScreenProps";
import ScreenRoutes from "./ScreenRoutes";
import Slider from "@react-native-community/slider";
import { SliderState } from "@grumpycorp/camera-slider-shared";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

/* ESLint believes the headerRight created below is a component. Alas. */
/* eslint-disable react/no-multi-comp */

/* ESLint also wants to see HomeScreen as a proper function component. */
/* This NavigationStack stuff is too weird to mess with it. Alas. */
/* eslint-disable react/function-component-definition */

const styles = StyleSheet.create({
  inlineSlider: {
    width: 250,
  },
});

const HomeScreen: NavigationStackScreenComponent<{}> = ({ navigation }): React.ReactElement => {
  const rootStore = useRootStore();
  const bluetoothConnection = rootStore.bluetoothConnection;
  const bluetoothCapabilitiesStore = rootStore.bluetoothCapabilitiesStore;
  const bluetoothStatusStore = rootStore.bluetoothStatusStore;

  if (!bluetoothConnection.isConnected) {
    setImmediate(() => {
      // Can only run this once render is done
      navigation.navigate(ScreenRoutes.Connect);
    });

    return <BaseView />;
  }

  function humanizeState(state: SliderState): string {
    // fooBarBaz -> foo Bar Baz -> foo bar baz -> Foo bar baz
    const stateComponents = state.split(/(?=[A-Z])/);
    const lowerCaseState = stateComponents.map(component => component.toLowerCase()).join(" ");

    return lowerCaseState.slice(0, 1).toUpperCase() + lowerCaseState.slice(1);
  }

  return (
    <BaseView>
      <ScrollView>
        <List.Section title="Status">
          <List.Item
            left={(): React.ReactNode => <List.Icon icon="information" />}
            title={`${humanizeState(bluetoothStatusStore.state)}`}
          />
          <List.Item
            description={
              <Slider
                maximumTrackTintColor={Colors.Position}
                maximumValue={bluetoothCapabilitiesStore.maximumPosition}
                minimumTrackTintColor={Colors.Position}
                minimumValue={0}
                onValueChange={async (value: number): Promise<void> => {
                  await rootStore.bluetoothTrackingStore.setDesiredPosition(value);
                }}
                step={100}
                style={styles.inlineSlider}
                thumbTintColor={Colors.Position}
              />
            }
            left={(): React.ReactNode => <List.Icon color={Colors.Position} icon="map-marker" />}
            title={`${bluetoothStatusStore.reportedPosition} steps`}
          />
          <List.Item
            left={(): React.ReactNode => <List.Icon color={Colors.Speed} icon="speedometer" />}
            title={`${bluetoothStatusStore.reportedVelocity} steps/sec`}
          />
        </List.Section>
      </ScrollView>
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
        icon="cog"
        onPress={(): void => {
          navigation.navigate(ScreenRoutes.Settings);
        }}
      />
    );
  },
});

export default observer(HomeScreen);
