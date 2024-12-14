# MCUBOOT example


## build
```shell
mkdir build
cd build

cmake ../ -G Ninja

ninja
```

## flash
```shell
openocd -f ../../../tools/openocd/interface/cmsis-dap.cfg -f ../../../tools/openocd/target/stm32g0x.cfg -c "program mcuboot_stm32.elf verify reset exit"
```
