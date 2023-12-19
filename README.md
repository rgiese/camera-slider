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
  - For Android development: https://reactnative.dev/docs/environment-setup
  - `npm install`
  - `npm run build -ws`
- Pre-commit
  - `npm run format -ws`
  - `npm run lint -ws`
