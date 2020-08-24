[![lerna](https://img.shields.io/badge/maintained%20with-lerna-cc00ff.svg)](https://lerna.js.org/)
[![CircleCI](https://circleci.com/gh/rgiese/camera-slider/tree/master.svg?style=shield)](https://circleci.com/gh/rgiese/camera-slider/tree/master)

# camera-slider

Linear Motion Camera Slider

# License

Licensed under [CC-BY-NC-SA](LICENSE.md). Commercial licensing negotiable (hah).

# Dev setup

- Getting started
  - Getting code
    - `git submodule init`
    - `git submodule update`
  - For firmware development:
    - Configure credentials
      - Set the environment variable `PARTICLE_ACCESS_TOKEN` to a Particle API token (generate with `particle token create`)
  - `npm install`
  - `lerna bootstrap`
  - `lerna run build`
- Pre-commit
  - `npm run format:fix`
  - `npm run lint:fix`

## Dev tooling tricks

### Lerna

- `lerna clean` to wipe all `node_modules` from packages (though not the root)
- `lerna link convert` to move a new package's dev dependencies up to the root `package.json`
- `npx sort-package-json` to clean up `package.json` files
- Updating packages (run in repo root):
  - `npm update`
  - `lerna exec npm update --stream`
  - `lerna bootstrap`
  - `npm run build` to verify
