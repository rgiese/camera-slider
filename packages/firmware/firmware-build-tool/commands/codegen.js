const { Command, flags } = require("@oclif/command");
const fs = require("fs");
const path = require("path");

const { BluetoothServices } = require("@grumpycorp/camera-slider-shared");

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

    // Make sure the output directory for generated files exists
    if (!fs.existsSync(projectGeneratedRoot)) {
      fs.mkdirSync(projectGeneratedRoot);
    }

    //
    // Codegen Bluetooth constants
    //

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

    //
    // Codegen clang ignore file
    //

    fs.writeFileSync(
      path.join(projectGeneratedRoot, ".clang-format"),
      "DisableFormat: true\nSortIncludes: false\n"
    );
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
