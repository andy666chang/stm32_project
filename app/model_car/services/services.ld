
SECTIONS
{
  .service_func : {
    . = ALIGN(4);
    _start_service_func = .;
    KEEP(*(.service_func))
    _stop_service_func = .;
  } >FLASH
}
