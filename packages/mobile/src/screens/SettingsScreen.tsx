import { Alert, ScrollView } from "react-native";
import { Button, List } from "react-native-paper";
import { Colors, Icons } from "../Theme";

import BaseView from "../components/BaseView";
import { NavigationStackScreenComponent } from "react-navigation-stack";
import React from "react";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

// eslint-disable-next-line react/function-component-definition
const SettingsScreen: NavigationStackScreenComponent<{}> = (): React.ReactElement => {
  const rootStore = useRootStore();
  const bluetoothCapabilitiesStore = rootStore.bluetoothCapabilitiesStore;
  const bluetoothStatusStore = rootStore.bluetoothStatusStore;

  return (
    <BaseView>
      <ScrollView>
        <List.Section title="Maximum values">
          <List.Item
            left={(): React.ReactNode => (
              <List.Icon color={Colors.Position} icon={Icons.Position} />
            )}
            title={`${bluetoothCapabilitiesStore.maximumPosition} steps`}
          />
          <List.Item
            left={(): React.ReactNode => <List.Icon color={Colors.Speed} icon={Icons.Speed} />}
            title={`${bluetoothCapabilitiesStore.maximumSpeed} steps/sec`}
          />
          <List.Item
            left={(): React.ReactNode => (
              <List.Icon color={Colors.Acceleration} icon={Icons.Acceleration} />
            )}
            title={`${bluetoothCapabilitiesStore.maximumAcceleration} steps/sec^2`}
          />
        </List.Section>
        <List.Section title="Calibration">
          <List.Item
            left={(): React.ReactNode => <List.Icon color={Colors.Delay} icon="axis-arrow-info" />}
            title={bluetoothCapabilitiesStore.touchscreenCalibrationData}
          />
          <List.Item
            title={
              <Button
                compact
                color="white"
                disabled={bluetoothStatusStore.state !== "trackingDesiredPosition"}
                icon="play"
                onPress={async (): Promise<void> =>
                  bluetoothCapabilitiesStore.startTouchscreenCalibration()
                }
              >
                Calibrate touchscreen
              </Button>
            }
          />
        </List.Section>
      </ScrollView>
    </BaseView>
  );
};

SettingsScreen.navigationOptions = {
  title: "Settings",
};

export default observer(SettingsScreen);
