# Protocol stack

## Building the project
### Start the nix environment:
```sh
nix develop
```
### Compiling
To compile the whole stack for x86:
```sh
make all
```
To compile the whole stack for Cortex M3:
```sh
make all CPU=cm3 PROFILE=debug
```

To generate the documentation only:
```sh
make doc
```