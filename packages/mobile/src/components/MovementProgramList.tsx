import { Alert, StyleSheet } from "react-native";
import { Colors, Icons } from "../Theme";
import { IconButton, List, Switch } from "react-native-paper";

import MovementParameter from "./MovementParameter";
import { MovementProgram } from "@grumpycorp/camera-slider-shared";
import React from "react";
import Slider from "@react-native-community/slider";
import { observer } from "mobx-react";

const styles = StyleSheet.create({
  inlineSlider: {
    width: 300,
  },
});

function MovementProgramList({
  movementProgram,
  setMovementProgram,
}: {
  movementProgram: MovementProgram;
  setMovementProgram: React.Dispatch<React.SetStateAction<MovementProgram>>;
}): React.ReactElement {
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
            maximumValue={500}
            minimumTrackTintColor={Colors.Rate}
            minimumValue={1}
            onValueChange={(value: number): void => {
              setMovementProgram({ ...movementProgram, Rate: value });
            }}
            step={10}
            style={styles.inlineSlider}
            thumbTintColor={Colors.Rate}
            value={movementProgram.Rate}
          />
        }
        left={(): React.ReactNode => <List.Icon color={Colors.Rate} icon={Icons.Rate} />}
        title={`Rate: ${movementProgram?.Rate ?? `<none>`}`}
      />
      {movementProgram.Movements.map(
        (movement, index): React.ReactNode => (
          <List.Item
            // eslint-disable-next-line react/no-array-index-key
            key={JSON.stringify(movement)}
            left={(): React.ReactNode => (
              <>
                <IconButton
                  disabled={index === 0}
                  icon="arrow-up"
                  onPress={(): void => swapMovements(index, index - 1)}
                  size={16}
                />
                <IconButton
                  disabled={index === movementProgram.Movements.length - 1}
                  icon="arrow-down"
                  onPress={(): void => swapMovements(index, index + 1)}
                  size={16}
                />
              </>
            )}
            right={(): React.ReactNode => (
              <IconButton icon={Icons.Delete} onPress={(): void => maybeDeleteMove(index)} />
            )}
            title={
              <>
                {movement.Type === "Move" && (
                  <>
                    <MovementParameter parameter="Position" value={movement.DesiredPosition ?? 0} />
                    <MovementParameter parameter="Speed" value={movement.DesiredSpeed ?? 0} />
                    <MovementParameter
                      parameter="Acceleration"
                      value={movement.DesiredAcceleration ?? 0}
                    />
                  </>
                )}
                {movement.Type === "Delay" && (
                  <MovementParameter parameter="Delay" value={movement.DelayTime ?? 0} />
                )}
              </>
            }
          />
        )
      )}
    </>
  );
}

export default observer(MovementProgramList);
