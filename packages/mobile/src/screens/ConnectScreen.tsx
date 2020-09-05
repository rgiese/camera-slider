import { Button, Surface, Title } from "react-native-paper";

import BaseView from "../components/BaseView";
import GrumpyRobin from "../assets/grumpy-robin.svg";
import { NavigationStackScreenComponent } from "react-navigation-stack";
import React from "react";
import { View } from "react-native";
import { observer } from "mobx-react";

const ConnectScreen: NavigationStackScreenComponent<{}> = (): React.ReactElement => {
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
          onPress={async (): Promise<void> => {
            //await authStore.authProvider.requestLogin();
          }}
        >
          Sign in
        </Button>
      </View>
    </BaseView>
  );
};

export default observer(ConnectScreen);
