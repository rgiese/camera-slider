import { Colors, Icons } from "../Theme";

import Icon from "react-native-vector-icons/MaterialCommunityIcons";
import React from "react";
import { Text } from "react-native";
import { observer } from "mobx-react";

function MovementParameter({
  parameter,
  value,
}: {
  parameter: "Position" | "Speed" | "Acceleration" | "Delay";
  value: number;
}): React.ReactElement {
  return (
    <>
      <Icon color={Colors[parameter]} name={Icons[parameter]} size={24} />
      <Text>
        {" " /* crappy padding */}
        {value}
        {"   "}
      </Text>
    </>
  );
}

export default observer(MovementParameter);
