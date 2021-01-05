# How to

To use this project:

```
mkdir build
cd build
cmake ..
./bin/unittest
```

The test code is in test1.cpp

## Gitpod
Also a gitpod docker is available.

## Value

Matrix as it is in file (as uint16) note that value in matrix can be converted to double:
```
double=valueInMatrixAsUint16/32768
```
Bias/Tare as it is in file (int16)
ADC value as int16