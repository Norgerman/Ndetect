# Ndetect

An video/picture people detection and preprocess tool base on opencv

# Build
## Download and build boost.filesystem

```
*nix & (win32 | 32bit opencv)
    ./b2 link=static --with-filesystem

win64 & use 64bit opencv
    ./b2 address-model=64 link=static --with-filesystem
```
copy boost into 3rd/boost/include and copy stage/lib into 3rd/boost


## Build use cmake