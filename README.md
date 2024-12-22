 ## Ubuntu 24 Install and Config gcc g++ CMake...

sudo apt install -y build-essential

## Gcc es el compilador.
sudo apt install -y gcc-14 g++-14

sudo apt install -y cmake

## VSCode install extensións:
- C/C++
- CMake
- CMake  Tools

## Install ninja
sudo apt install -y ninja-build

## Config VSCode settings.json
"cmake.configureOnOpen": true,
"cmake.generator": "Ninja"

## VSCode C/C++
- Include path `${workspaceFolder}`

## Library nlohmann-json3
sudo apt install -y nlohmann-json3-dev