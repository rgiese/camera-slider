import BaseView from "../components/BaseView";
import { Colors } from "../Theme";
import { List } from "react-native-paper";
import { NavigationStackScreenComponent } from "react-navigation-stack";
import React from "react";
import { ScrollView } from "react-native";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

// eslint-disable-next-line react/function-component-definition
const SettingsScreen: NavigationStackScreenComponent<{}> = (): React.ReactElement => {
  const rootStore = useRootStore();
  const capabilitiesStore = rootStore.bluetoothCapabilitiesStore;

  return (
    <BaseView>
      <ScrollView>
        <List.Section title="Maximum values">
          <List.Item
            left={(): React.ReactNode => <List.Icon color={Colors.Position} icon="map-marker" />}
            title={`${capabilitiesStore.maximumPosition} steps`}
          />
          <List.Item
            left={(): React.ReactNode => <List.Icon color={Colors.Speed} icon="speedometer" />}
            title={`${capabilitiesStore.maximumSpeed} steps/sec`}
          />
          <List.Item
            left={(): React.ReactNode => (
              <List.Icon color={Colors.Acceleration} icon="trending-up" />
            )}
            title={`${capabilitiesStore.maximumAcceleration} steps/sec^2`}
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
