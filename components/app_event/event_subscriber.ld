
SECTIONS
{
  .event_subscriber : {
    . = ALIGN(4);
    _event_subscriber_start = .;
    KEEP(*(.event_subscriber))
    _event_subscriber_end = .;
  } >FLASH
}
