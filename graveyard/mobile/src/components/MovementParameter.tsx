import { Colors, Icons } from "../Theme";
import { IconButton, List } from "react-native-paper";
import { Movement, MovementProgram } from "@grumpycorp/camera-slider-shared";
import { StyleSheet, Text } from "react-native";

import React from "react";
import Slider from "@react-native-community/slider";
import { observer } from "mobx-react";

const styles = StyleSheet.create({
  inlineSlider: {
    width: 250,
  },
  maximaText: {
    color: Colors.Text.Dim,
  },
});

function MovementParameter({
  movementProgram,
  movementIndex,
  setMovementProgram,
  movementField,
  maximumValue,
  minorIncrement,
  majorIncrement,
}: {
  movementProgram: MovementProgram;
  movementIndex: number;
  setMovementProgram: (updatedMovementProgram: MovementProgram) => void;
  movementField: "Position" | "Speed" | "Acceleration" | "Delay";
  maximumValue: number;

  minorIncrement: number;
  majorIncrement: number;
}): React.ReactElement {
  function dataField(): keyof Omit<Movement, "Type"> {
    switch (movementField) {
      case "Position":
        return "DesiredPosition";
      case "Speed":
        return "DesiredSpeed";
      case "Acceleration":
        return "DesiredAcceleration";
      case "Delay":
        return "DelayTime";
    }
  }

  const currentValue = movementProgram.Movements[movementIndex][dataField()] ?? 0;

  function updateMovement(updatedValue: number): void {
    const updatedMovements = movementProgram.Movements.slice();

    updatedMovements[movementIndex][dataField()] = Math.max(
      0,
      Math.min(updatedValue, maximumValue)
    );

    setMovementProgram({ ...movementProgram, Movements: updatedMovements });
  }

  return (
    <List.Item
      description={
        <Slider
          maximumTrackTintColor={Colors[movementField]}
          maximumValue={maximumValue}
          minimumTrackTintColor={Colors[movementField]}
          minimumValue={0}
          onSlidingComplete={(value: number): void => {
            updateMovement(value);
          }}
          step={minorIncrement}
          style={styles.inlineSlider}
          thumbTintColor={Colors[movementField]}
          value={currentValue}
        />
      }
      left={(): React.ReactNode => (
        <List.Icon color={Colors[movementField]} icon={Icons[movementField]} />
      )}
      right={(): React.ReactNode => (
        <>
          <IconButton
            icon="plus"
            onLongPress={(): void => {
              updateMovement(currentValue + majorIncrement);
            }}
            onPress={(): void => {
              updateMovement(currentValue + minorIncrement);
            }}
            size={18}
          />
          <IconButton
            icon="minus"
            onLongPress={(): void => {
              updateMovement(currentValue - majorIncrement);
            }}
            onPress={(): void => {
              updateMovement(currentValue - minorIncrement);
            }}
            size={18}
          />
        </>
      )}
      title={
        <Text>
          {`${currentValue}`}
          {`  `}
          <Text style={styles.maximaText}>{`(${maximumValue} max)`}</Text>
        </Text>
      }
    />
  );
}

export default observer(MovementParameter);
