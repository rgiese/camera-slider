import { Button, List } from "react-native-paper";
import { Colors, Icons } from "../Theme";
import { NavigationRoute, NavigationScreenProp } from "react-navigation";
import { NavigationStackOptions, NavigationStackScreenComponent } from "react-navigation-stack";
import { ScrollView, StyleSheet, Text } from "react-native";

import BaseView from "../components/BaseView";
import { IconButton } from "react-native-paper";
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
    width: 300,
  },
  maximaText: {
    color: Colors.Text.Dim,
  },
});

const HomeScreen: NavigationStackScreenComponent<{}> = ({ navigation }): React.ReactElement => {
  const rootStore = useRootStore();
  const bluetoothConnection = rootStore.bluetoothConnection;
  const bluetoothCapabilitiesStore = rootStore.bluetoothCapabilitiesStore;
  const bluetoothStatusStore = rootStore.bluetoothStatusStore;
  const bluetoothTrackingStore = rootStore.bluetoothTrackingStore;

  if (!bluetoothConnection.isConnected) {
    setImmediate(() => {
      // Can only run this once render is done
      navigation.navigate(ScreenRoutes.Connect);
    });

    return <BaseView />;
  }

  const canRequestChanges = bluetoothStatusStore.state === "trackingDesiredPosition";

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
              canRequestChanges && (
                <Slider
                  maximumTrackTintColor={Colors.Position}
                  maximumValue={bluetoothCapabilitiesStore.maximumPosition}
                  minimumTrackTintColor={Colors.Position}
                  minimumValue={0}
                  onValueChange={async (value: number): Promise<void> => {
                    await bluetoothTrackingStore.setDesiredPosition(value);
                  }}
                  step={bluetoothCapabilitiesStore.maximumPosition / styles.inlineSlider.width}
                  style={styles.inlineSlider}
                  thumbTintColor={Colors.Position}
                  value={bluetoothStatusStore.reportedPosition}
                />
              )
            }
            left={(): React.ReactNode => (
              <List.Icon color={Colors.Position} icon={Icons.Position} />
            )}
            title={
              <Text>
                {`${bluetoothStatusStore.reportedPosition} steps `}
                <Text
                  style={styles.maximaText}
                >{`(${bluetoothCapabilitiesStore.maximumPosition} max)`}</Text>
              </Text>
            }
          />
          {canRequestChanges && (
            <>
              <List.Item
                description={
                  <Slider
                    maximumTrackTintColor={Colors.Speed}
                    maximumValue={bluetoothCapabilitiesStore.maximumSpeed}
                    minimumTrackTintColor={Colors.Speed}
                    minimumValue={0}
                    onValueChange={async (value: number): Promise<void> => {
                      await bluetoothTrackingStore.setDesiredMaximumSpeed(value);
                    }}
                    step={bluetoothCapabilitiesStore.maximumSpeed / styles.inlineSlider.width}
                    style={styles.inlineSlider}
                    thumbTintColor={Colors.Speed}
                    value={bluetoothStatusStore.reportedMaximumSpeed}
                  />
                }
                left={(): React.ReactNode => <List.Icon color={Colors.Speed} icon={Icons.Speed} />}
                title={
                  <Text>
                    {`${bluetoothStatusStore.reportedVelocity} steps/sec `}
                    <Text
                      style={styles.maximaText}
                    >{`(limit ${bluetoothStatusStore.reportedMaximumSpeed}, ${bluetoothCapabilitiesStore.maximumSpeed} max)`}</Text>
                  </Text>
                }
              />
              <List.Item
                description={
                  <Slider
                    maximumTrackTintColor={Colors.Acceleration}
                    maximumValue={bluetoothCapabilitiesStore.maximumAcceleration}
                    minimumTrackTintColor={Colors.Acceleration}
                    minimumValue={0}
                    onValueChange={async (value: number): Promise<void> => {
                      await bluetoothTrackingStore.setDesiredMaximumAcceleration(value);
                    }}
                    step={
                      bluetoothCapabilitiesStore.maximumAcceleration / styles.inlineSlider.width
                    }
                    style={styles.inlineSlider}
                    thumbTintColor={Colors.Acceleration}
                    value={bluetoothStatusStore.reportedMaximumAcceleration}
                  />
                }
                left={(): React.ReactNode => (
                  <List.Icon color={Colors.Acceleration} icon={Icons.Acceleration} />
                )}
                title={
                  <Text>
                    steps/sec^2{" "}
                    <Text
                      style={styles.maximaText}
                    >{`(limit ${bluetoothStatusStore.reportedMaximumAcceleration}, ${bluetoothCapabilitiesStore.maximumAcceleration} max)`}</Text>
                  </Text>
                }
              />
            </>
          )}
        </List.Section>
        <List.Section title="Testing">
          {canRequestChanges && (
            <List.Item
              title={
                <Button
                  mode="contained"
                  onPress={async (): Promise<void> => {
                    await rootStore.bluetoothProgramStore.test();
                  }}
                >
                  Test
                </Button>
              }
            />
          )}
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
