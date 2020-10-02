import { Alert, StyleSheet } from "react-native";
import { Button, IconButton, List, Switch } from "react-native-paper";
import { Colors, Icons } from "../Theme";

import MovementParameter from "./MovementParameter";
import { MovementProgram } from "@grumpycorp/camera-slider-shared";
import React from "react";
import Slider from "@react-native-community/slider";
import { observer } from "mobx-react";
import { useRootStore } from "../stores/RootStoreContext";

const styles = StyleSheet.create({
  inlineSlider: {
    width: 250,
  },
});

function MovementProgramList({
  movementProgram,
  setMovementProgram,
}: {
  movementProgram: MovementProgram;
  setMovementProgram: React.Dispatch<React.SetStateAction<MovementProgram>>;
}): React.ReactElement {
  const rootStore = useRootStore();
  const bluetoothCapabilitiesStore = rootStore.bluetoothCapabilitiesStore;

  function maybeDeleteMove(moveIndex: number): void {
    Alert.alert(`Delete move?`, undefined, [
      { text: "Cancel", style: "cancel" },
      {
        text: "OK",
        onPress: (): void => {
          setMovementProgram({
            ...movementProgram,
            Movements: movementProgram.Movements.filter((_value, index) => index != moveIndex),
          });
        },
      },
    ]);
  }

  function swapMovements(indexA: number, indexB: number): void {
    const updatedMovements = movementProgram.Movements.slice();

    const originalA = updatedMovements[indexA];
    updatedMovements[indexA] = updatedMovements[indexB];
    updatedMovements[indexB] = originalA;

    setMovementProgram({ ...movementProgram, Movements: updatedMovements });
  }

  const minimumRate = 10;
  const maximumRate = 500;

  const minorRateIncrement = 10;
  const majorRateIncrement = 100;

  function updateRate(rate: number): void {
    setMovementProgram({
      ...movementProgram,
      Rate: Math.max(minimumRate, Math.min(rate, maximumRate)),
    });
  }

  return (
    <>
      <List.Item
        left={(): React.ReactNode => <List.Icon color={Colors.Repeat} icon={Icons.Repeat} />}
        title={
          <>
            <Switch
              color={Colors.Repeat}
              onValueChange={(value: boolean): void =>
                setMovementProgram({ ...movementProgram, Repeats: value })
              }
              value={movementProgram.Repeats}
            />
            {movementProgram.Repeats ? `Repeat` : `Run once`}
          </>
        }
      />
      <List.Item
        description={
          <Slider
            maximumTrackTintColor={Colors.Rate}
            maximumValue={maximumRate}
            minimumTrackTintColor={Colors.Rate}
            minimumValue={minimumRate}
            onValueChange={(value: number): void => updateRate(value)}
            step={10}
            style={styles.inlineSlider}
            thumbTintColor={Colors.Rate}
            value={movementProgram.Rate}
          />
        }
        left={(): React.ReactNode => <List.Icon color={Colors.Rate} icon={Icons.Rate} />}
        right={(): React.ReactNode => (
          <>
            <IconButton
              icon="plus"
              onLongPress={(): void => updateRate(movementProgram.Rate + majorRateIncrement)}
              onPress={(): void => updateRate(movementProgram.Rate + minorRateIncrement)}
              size={18}
            />
            <IconButton
              icon="minus"
              onLongPress={(): void => updateRate(movementProgram.Rate - majorRateIncrement)}
              onPress={(): void => updateRate(movementProgram.Rate - minorRateIncrement)}
              size={18}
            />
          </>
        )}
        title={`Rate: ${movementProgram?.Rate ?? `<none>`}`}
      />
      {movementProgram.Movements.map(
        (movement, index): React.ReactNode => (
          <React.Fragment key={index.toString() + JSON.stringify(movement)}>
            <List.Item
              right={(): React.ReactElement => (
                <>
                  <IconButton
                    disabled={index === 0}
                    icon="arrow-up"
                    onPress={(): void => swapMovements(index, index - 1)}
                  />
                  <IconButton
                    disabled={index === movementProgram.Movements.length - 1}
                    icon="arrow-down"
                    onPress={(): void => swapMovements(index, index + 1)}
                  />
                  <IconButton
                    color={Colors.Text.Dim}
                    icon={Icons.Delete}
                    onPress={(): void => maybeDeleteMove(index)}
                  />
                </>
              )}
              title={
                <Button color={Colors[movement.Type]} icon="play">
                  {`Step ${index + 1}: ${movement.Type}`}
                </Button>
              }
            />
            {movement.Type === "Move" && (
              <>
                <MovementParameter
                  majorIncrement={100}
                  maximumValue={bluetoothCapabilitiesStore.maximumPosition}
                  minorIncrement={10}
                  movementField="Position"
                  movementIndex={index}
                  movementProgram={movementProgram}
                  setMovementProgram={setMovementProgram}
                />
                <MovementParameter
                  majorIncrement={100}
                  maximumValue={bluetoothCapabilitiesStore.maximumSpeed}
                  minorIncrement={10}
                  movementField="Speed"
                  movementIndex={index}
                  movementProgram={movementProgram}
                  setMovementProgram={setMovementProgram}
                />
                <MovementParameter
                  majorIncrement={100}
                  maximumValue={bluetoothCapabilitiesStore.maximumAcceleration}
                  minorIncrement={10}
                  movementField="Acceleration"
                  movementIndex={index}
                  movementProgram={movementProgram}
                  setMovementProgram={setMovementProgram}
                />
              </>
            )}
            {movement.Type === "Delay" && (
              <MovementParameter
                majorIncrement={1000}
                maximumValue={5000}
                minorIncrement={100}
                movementField="Delay"
                movementIndex={index}
                movementProgram={movementProgram}
                setMovementProgram={setMovementProgram}
              />
            )}
          </React.Fragment>
        )
      )}
    </>
  );
}

export default observer(MovementProgramList);
