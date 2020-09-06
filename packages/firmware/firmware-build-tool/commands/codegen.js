const { Command, flags } = require("@oclif/command");
const fs = require("fs");
const path = require("path");

const { BluetoothStatusService } = require("@grumpycorp/camera-slider-shared");

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

    function generateConstants(namespace, constants) {
      headerFileContent += `namespace ${namespace}Constants {\n`;

      for (const prop in constants) {
        if (!constants.hasOwnProperty(prop)) {
          continue;
        }

        headerFileContent += `    const BleUuid ${prop}("${constants[prop]}");\n`;
      }

      headerFileContent += `} // ${namespace}Constants\n`;
    }

    generateConstants("BluetoothStatusService", BluetoothStatusService);

    fs.writeFileSync(path.join(projectGeneratedRoot, "bluetoothConstants.h"), headerFileContent);

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
