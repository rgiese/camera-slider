import * as fs from "fs";
import * as glob from "glob";
import { execSync } from "child_process";
import * as path from "path";

import { getBuiltImagePath } from "./common";

function exitWithUsage() {
  console.error(
    `Usage: ${process.argv[1]} <projectDirectory> [-i IncludeGlob] [-x ExcludeGlob] [-r IncludeRoot] ...`
  );
  process.exit(1);
}

// Check parameters
if (process.argv.length < 2) {
  exitWithUsage();
}

const projectName = process.argv[2];

const includeGlobs: string[] = [];
const excludeGlobs: string[] = [];
const includeRoots: string[] = [];

for (let idx = 3; idx < process.argv.length; idx += 2) {
  // Check that we have both an option (e.g. "-i") and an option value following it
  if (idx + 1 >= process.argv.length) {
    console.error(`Option ${process.argv[idx]} is missing an option value.`);
    exitWithUsage();
  }

  const optionName = process.argv[idx];
  const optionValue = process.argv[idx + 1];

  // Parse option
  if (optionName === "-i") {
    includeGlobs.push(optionValue);
  } else if (optionName === "-x") {
    excludeGlobs.push(optionValue);
  } else if (optionName === "-r") {
    includeRoots.push(optionValue);
  } else {
    console.error(`Option ${optionName} not recognized.`);
    exitWithUsage();
  }
}

// Check environment
const particleAccessToken = process.env.PARTICLE_ACCESS_TOKEN;

if (!particleAccessToken) {
  console.error("Environment variable PARTICLE_ACCESS_TOKEN must be defined.");
  process.exit(1);
}

// Log in Particle CLI
if (process.env.CIRCLECI) {
  execSync(`particle login --token ${particleAccessToken}`, { stdio: "inherit" });
}

// Set up paths
const packageRoot = process.cwd();
const projectRoot = path.join(packageRoot, projectName);

const builtImage = getBuiltImagePath(projectRoot, projectName);

{
  // Ensure built image directory exists
  const builtImageDirectory = path.dirname(builtImage);
  if (!fs.existsSync(builtImageDirectory)) {
    fs.mkdirSync(builtImageDirectory);
  }
}

// Create clean temporary directory
const buildRoot = path.join(packageRoot, "build");

if (!fs.existsSync(buildRoot)) {
  fs.mkdirSync(buildRoot);
}

const projectBuildRoot = path.join(buildRoot, projectName);

if (fs.existsSync(projectBuildRoot)) {
  fs.rmSync(projectBuildRoot, { recursive: true, force: true });
}

// Collect source files
console.log(`Building ${projectRoot} in ${projectBuildRoot}...`);

function copySourceFileToTemporaryDirectory(fileName: string, destinationRelativePath?: string) {
  // Locate source file
  const sourceRelativePath = path.relative(projectRoot, fileName).replace("\\", "/"); // for Windows

  // Copy source file to temporary directory
  const destinationPath = path.join(
    projectBuildRoot,
    destinationRelativePath ?? sourceRelativePath
  );

  {
    // Ensure destination directory exists
    const destinationDirectory = path.dirname(destinationPath);

    if (!fs.existsSync(destinationDirectory)) {
      fs.mkdirSync(destinationDirectory, { recursive: true });
    }
  }

  fs.copyFileSync(fileName, destinationPath);
}

glob
  .sync(`${projectRoot}/**/*.{c,cpp,h,hpp,properties}`, { ignore: excludeGlobs })
  .forEach((fileName) => {
    copySourceFileToTemporaryDirectory(fileName);
  });

// Find external dependencies
const relativeIncludeRoots = new Array();

includeRoots.map((includeRootGlob) => {
  glob.sync(`${packageRoot}/${includeRootGlob}`).forEach((pathName) => {
    const relativePath = path.relative(packageRoot, pathName).replace(/\\/g, "/");
    console.log(`Include root: ${relativePath}`);

    relativeIncludeRoots.push(relativePath);
  });
});

includeGlobs.map((includeGlob) => {
  console.log(`Processing include glob ${includeGlob}...`);

  glob
    .sync(`${packageRoot}/${includeGlob}`, {
      ignore: excludeGlobs,
    })
    .forEach((fileName) => {
      const relativePath = path.relative(packageRoot, fileName).replace(/\\/g, "/");

      // Add file minus include root, if found
      for (const relativeIncludeRoot of relativeIncludeRoots) {
        if (relativePath.startsWith(relativeIncludeRoot)) {
          const rootRelativePath = relativePath.substring(relativeIncludeRoot.length + 1); // +1 for path separator

          copySourceFileToTemporaryDirectory(fileName, rootRelativePath);
          return;
        }
      }

      // Add file as-is
      copySourceFileToTemporaryDirectory(fileName);
    });
});

// Compile
execSync(`particle compile argon --saveTo ${builtImage}`, {
  cwd: projectBuildRoot,
  stdio: "inherit",
});
