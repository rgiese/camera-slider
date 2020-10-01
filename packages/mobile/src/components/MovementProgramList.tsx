import { Alert, StyleSheet, Text } from "react-native";
import { Colors, Icons } from "../Theme";
import { IconButton, List, Switch } from "react-native-paper";

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
        (movement, index): React.ReactNode => {
          switch (movement.Type) {
            case "Move":
              return (
                <List.Item
                  description={
                    <>
                      <IconButton color={Colors.Position} icon={Icons.Position} />
                      <Text>{movement.DesiredPosition}</Text>

                      <IconButton color={Colors.Speed} icon={Icons.Speed} />
                      <Text>{movement.DesiredSpeed}</Text>

                      <IconButton color={Colors.Acceleration} icon={Icons.Acceleration} />
                      <Text>{movement.DesiredPosition}</Text>
                    </>
                  }
                  // eslint-disable-next-line react/no-array-index-key
                  key={index}
                  left={(): React.ReactNode => <List.Icon icon={Icons.Move} />}
                  right={(): React.ReactNode => (
                    <>
                      <IconButton icon={Icons.Edit} />
                      <IconButton
                        icon={Icons.Delete}
                        onPress={(): void => maybeDeleteMove(index)}
                      />
                    </>
                  )}
                  title="Move"
                />
              );
            case "Delay":
              return (
                <List.Item
                  description={`${movement.DelayTime} msec`}
                  // eslint-disable-next-line react/no-array-index-key
                  key={index}
                  left={(): React.ReactNode => <List.Icon icon={Icons.Delay} />}
                  right={(): React.ReactNode => (
                    <>
                      <IconButton icon={Icons.Edit} />
                      <IconButton
                        icon={Icons.Delete}
                        onPress={(): void => maybeDeleteMove(index)}
                      />
                    </>
                  )}
                  title="Delay"
                />
              );
          }
        }
      )}
    </>
  );
}

export default observer(MovementProgramList);
