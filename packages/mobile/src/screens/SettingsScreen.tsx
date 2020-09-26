import { Button, List } from "react-native-paper";

import BaseView from "../components/BaseView";
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
            left={(): React.ReactNode => <List.Icon icon="map-marker" />}
            title={`${capabilitiesStore.maximumPosition} steps`}
          />
          <List.Item
            left={(): React.ReactNode => <List.Icon icon="speedometer" />}
            title={`${capabilitiesStore.maximumSpeed} steps/sec`}
          />
          <List.Item
            left={(): React.ReactNode => <List.Icon icon="trending-up" />}
            title={`${capabilitiesStore.maximumAcceleration} steps/sec^2`}
          />
          <Button
            mode="outlined"
            onPress={async (): Promise<void> => {
              //await authStore.authProvider.requestLogout();
            }}
          >
            Find maximum range
          </Button>
        </List.Section>
      </ScrollView>
    </BaseView>
  );
};

SettingsScreen.navigationOptions = {
  title: "Settings",
};

export default observer(SettingsScreen);
