# Model Car

## Build

```shell
cmake ../app/model_car -G Ninja

ninja -j16

openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "program build/model_car.elf verify reset exit"

pyocd reset

# Erase all chip
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x mass_erase 0; exit"

# Read & Write NRST_MODE from 3 to 1
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; set val [mdw 0x40022020]; echo \$val; exit"
--> 0x40022020: ff4fe0aa

openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x option_write 0 0x20 0xEF6FE0AA; stm32l4x option_load; exit"

# Read & Write RDP from 0xAA to 0xBB
openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; set val [mdw 0x40022020]; echo \$val; exit"
0x40022020: ef6fe0aa

openocd -f tools/openocd/interface/cmsis-dap.cfg -f tools/openocd/target/stm32g0x.cfg -c "init; reset halt; stm32l4x option_write 0 0x20 0xBB 0xFF; stm32l4x option_load; exit"


```

## CMD

```
led -i <0...5> -l <0...1>

btn -p <1...9>

thro -v <0...100>
```

## Code flow

- Button
```mermaid
graph LR

    IRQ[GPIO IRQ] 

    subgraph E0[Edge detect]
        E1(Check SG level) -->
        E2(Measure interval) -->
        E3(Generate signal)
    end

    subgraph S0[Button Process]
        S1(Check Signal input) -->
        S2(Decode signal
        press/release) -->
        S3(Check timeout) -->
        S4(Generate Button event)
    end

    subgraph B0[Button Event]
        B1(Check Button Event)

        subgraph L0[LED Event]
            L1(C and T turns on)
            L2(C and T remains on, H turns on)
            L3(C and T remains on, H turns off)
            L4(C and T turns off)

            L5(turns on/off high beam)
        end

        subgraph Sys0[System Event]
            Sys1(Reset the neutral point)
            Sys2(Reverse the direction of F)
        end

        B1 --> L0 & Sys0
    end


    IRQ --> E0 --> S0 --> B0

```

- Throttle
```mermaid
graph LR
    IRQ[GPIO IRQ] 

    subgraph E0[Edge detect]
        E1(Check SG level) -->
        E2(Measure interval) -->
        E3(Generate signal)
    end

    subgraph S0[Throttle Process]
        S1(Check Signal input) -->
        S2(Record throttle value) -->
        S3(Analyze throttle behavior) -->
        S4(Send throttle Event)
    end

    subgraph T0[Throttle Event]
        direction LR
        T2(None)
        T3(Tap brake 0.5s)
        T4(Long brake 1s)
        T5(Back fire)
    end

    IRQ --> E0 --> S0 --> T0

```
