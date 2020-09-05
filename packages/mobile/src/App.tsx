import AppTheme from "./Theme";
import { configure as MobxConfigure } from "mobx";
import { Provider as PaperProvider } from "react-native-paper";
import React from "react";
import { RootStore } from "./stores/RootStore";
import { RootStoreContext } from "./stores/RootStoreContext";
import ScreenProps from "./screens/ScreenProps";
import Screens from "./screens";

// App-wide MobX configuration
MobxConfigure({ enforceActions: "observed" });

const rootStore = new RootStore();

function App(): React.ReactElement {
  const screenProps: ScreenProps = {
    theme: AppTheme,
  };

  return (
    <PaperProvider theme={screenProps.theme}>
      <RootStoreContext.Provider value={{ rootStore }}>
        <Screens screenProps={screenProps} />
      </RootStoreContext.Provider>
    </PaperProvider>
  );
}

export default App;
