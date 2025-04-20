
SECTIONS
{
  .service : {
    . = ALIGN(4);
    _start_service = .;
    KEEP(*(.service))
    _stop_service = .;
  } >FLASH
}
