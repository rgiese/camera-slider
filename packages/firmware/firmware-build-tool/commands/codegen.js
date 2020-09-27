const { Command, flags } = require("@oclif/command");
const { execSync } = require("child_process");
const fs = require("fs");
const path = require("path");

const { BluetoothServices, SliderStateNames } = require("@grumpycorp/camera-slider-shared");

class CodegenCommand extends Command {
  async run() {
    // Check parameters
    const { flags } = this.parse(CodegenCommand);

    if (!flags.project) {
      this.error("No project directory specified (-p). Exiting.");
    }

    // Set up paths
    const packageRoot = process.cwd();
    const projectRoot = path.join(packageRoot, flags.project);
    const projectGeneratedRoot = path.join(projectRoot, "generated");

    const sharedPackageRoot = path.join(packageRoot, "../shared/");
    const schemasRoot = path.join(sharedPackageRoot, "src/schema");

    // Make sure the output directory for generated files exists
    if (!fs.existsSync(projectGeneratedRoot)) {
      fs.mkdirSync(projectGeneratedRoot);
    }

    // Codegen Flatbuffers
    this.codegenFlatbuffers(schemasRoot, projectGeneratedRoot);

    // Codegen Bluetooth constants
    this.codegenBluetoothConstants(projectGeneratedRoot);

    // Codegen Slider states
    this.codegenSliderStates(projectGeneratedRoot);

    // Codegen clang ignore file
    fs.writeFileSync(
      path.join(projectGeneratedRoot, ".clang-format"),
      "DisableFormat: true\nSortIncludes: false\n"
    );
  }

  codegenBluetoothConstants(projectGeneratedRoot) {
    let headerFileContent = "#pragma once\n// This is a generated file.\n";
    headerFileContent += `namespace BluetoothIds {\n`;

    for (const serviceName in BluetoothServices) {
      if (!BluetoothServices.hasOwnProperty(serviceName)) {
        continue;
      }

      const service = BluetoothServices[serviceName];

      headerFileContent += `  namespace ${serviceName} {\n`;
      headerFileContent += `    const BleUuid Id("${service.Id}");\n`;
      headerFileContent += `    namespace Characteristics {\n`;

      for (const characteristicName in service.Characteristics) {
        if (!service.Characteristics.hasOwnProperty(characteristicName)) {
          continue;
        }

        headerFileContent += `      const BleUuid ${characteristicName}("${service.Characteristics[characteristicName]}");\n`;
      }

      headerFileContent += `    } // namespace Characteristics\n`;
      headerFileContent += `  } // namespace ${serviceName}\n`;
    }

    headerFileContent += `} // BluetoothIds\n`;

    fs.writeFileSync(path.join(projectGeneratedRoot, "bluetoothIds.h"), headerFileContent);
  }

  codegenSliderStates(projectGeneratedRoot) {
    let headerFileContent = "#pragma once\n// This is a generated file.\n";

    const capitalize = str => {
      return str.slice(0, 1).toUpperCase() + str.slice(1);
    };

    // Enum
    headerFileContent += `enum class SliderState {\n`;

    for (const sliderState of SliderStateNames) {
      headerFileContent += `  ${capitalize(sliderState)},\n`;
    }

    headerFileContent += `};\n`;

    // Enum->string mapping
    headerFileContent += `inline char const* getSliderStateName(SliderState const state) {\n`;
    headerFileContent += `  switch (state) {\n`;

    for (const sliderState of SliderStateNames) {
      const capitalState = capitalize(sliderState);
      headerFileContent += `    case SliderState::${capitalState}: return "${sliderState}";\n`;
    }

    headerFileContent += `    default: return "<unknown>";\n`;
    headerFileContent += `  }\n`;
    headerFileContent += `}\n`;

    fs.writeFileSync(path.join(projectGeneratedRoot, "sliderStates.h"), headerFileContent);
  }

  codegenFlatbuffers(schemasRoot, projectGeneratedRoot) {
    const grumpycorpRoot = process.env.GRUMPYCORP_ROOT;

    if (!grumpycorpRoot) {
      this.error("Environment variable GRUMPYCORP_ROOT must be defined. Exiting.");
      return;
    }

    const flatbuffersRoot = path.join(grumpycorpRoot, "flatbuffers");

    const fixIncludePathsInFile = fileName => {
      // Modify include locations from "flatbuffers/{foo.h}" to "{foo.h}"
      // because we don't get to set additional include roots with the Particle compiler
      const fileContent = fs.readFileSync(fileName).toString();

      const fixedUpFileContent = fileContent.replace('#include "flatbuffers/', '#include "');

      fs.writeFileSync(fileName, fixedUpFileContent);
    };

    // Run codegen
    ["firmware.fbs"].map(fbs => {
      const flatbuffersSchema = path.join(schemasRoot, fbs);
      this.log(`Processing ${flatbuffersSchema}`);

      execSync(
        `${flatbuffersRoot}/flatc --cpp --scoped-enums -o ${projectGeneratedRoot} ${flatbuffersSchema}`,
        {
          stdio: "inherit",
        }
      );

      fixIncludePathsInFile(path.join(projectGeneratedRoot, fbs.replace(".fbs", "_generated.h")));
    });

    // Copy C++ headers
    const flatbuffersIncludesSource = path.join(flatbuffersRoot, "include/flatbuffers");

    ["flatbuffers.h", "base.h", "stl_emulation.h"].map(header => {
      const source = path.join(flatbuffersIncludesSource, header);
      const destination = path.join(projectGeneratedRoot, header);

      this.log(`Rewriting ${source} -> ${destination}`);
      fs.copyFileSync(source, destination);
      fixIncludePathsInFile(destination);
    });
  }
}

CodegenCommand.description = `Generate files for firmware project
...
Provide name of project directory with -p
`;

CodegenCommand.flags = {
  project: flags.string({ char: "p", description: "Project to codegen for" }),
};

module.exports = CodegenCommand;
