import { ActivityIndicator, Button, Surface, Text, Title } from "react-native-paper";

import BaseView from "../components/BaseView";
import GrumpyRobin from "../assets/grumpy-robin.svg";
import { NavigationStackScreenComponent } from "react-navigation-stack";
import React from "react";
import ScreenRoutes from "./ScreenRoutes";
import { View } from "react-native";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

const ConnectScreen: NavigationStackScreenComponent<{}> = ({ navigation }): React.ReactElement => {
  const bluetoothStore = useRootStore().bluetoothStore;

  if (bluetoothStore.state === "initializing" || bluetoothStore.state === "connecting") {
    return (
      <BaseView>
        <View style={{ flex: 1, justifyContent: "center", alignItems: "center" }}>
          <ActivityIndicator animating />
        </View>
      </BaseView>
    );
  } else if (bluetoothStore.state === "disconnected") {
    return (
      <BaseView>
        <View style={{ flex: 1, justifyContent: "center", alignItems: "center" }}>
          <Surface
            style={{
              justifyContent: "center",
              alignItems: "center",
              width: "100%",
              height: 80,
              marginBottom: 20,
            }}
          >
            <GrumpyRobin fill="#000" height={60} width={60} />
          </Surface>
          <Title style={{ marginBottom: 20 }}>Camera Slider</Title>
          <Button
            mode="contained"
            onPress={(): void => {
              bluetoothStore.connect();
            }}
          >
            Connect
          </Button>
        </View>
      </BaseView>
    );
  } else if (bluetoothStore.state === "connected") {
    setImmediate(() => {
      // Can only run this once render is done
      navigation.navigate(ScreenRoutes.Home);
    });

    return <BaseView />;
  } else if (bluetoothStore.state === "error") {
    return (
      <BaseView>
        <View style={{ flex: 1, justifyContent: "center", alignItems: "center" }}>
          <Text>{bluetoothStore.error}</Text>
        </View>
      </BaseView>
    );
  } else {
    throw new Error("Unexpected authentication state");
  }
};

export default observer(ConnectScreen);
