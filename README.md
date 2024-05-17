# linkerfs-warp-gen-oatpp

a warp configuration file generator backend for project linkerfs powered by Oat++ with swagger-ui and Qt.

## Repository Status

This repository is **Abandoned** at current, just as an example to use Oat++ with swagger-ui.

## Features

1. api implement: list dir, see swagger for more details
2. serve a directory `dist` build by webpack at working directory with `/webui` base prefix
3. swagger ui is running at `/swagger/ui`
4. server is running at `0.0.0.0:8000`

## Build

```shell
mkdir build && cd build
cmake ..
make
```

## Credits

[Qt](https://www.qt.io/) License: [LGPLv3](https://www.gnu.org/licenses/)

[Oat++](https://oatpp.io/) License: [Apache-2.0](https://www.apache.org/licenses/)

