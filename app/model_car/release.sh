
echo "Erase all chip"
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x mass_erase 0; exit"

echo "Write User Configuration"
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x option_write 0 0x20 0xEF6FE0AA; stm32l4x option_load; exit"

echo "Download Model Car firmware ..."
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "program app/model_car/model_car_20251227.elf verify reset exit"

echo "Lock MCU flash ..."
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x option_write 0 0x20 0xBB 0xFF; stm32l4x option_load; exit"

