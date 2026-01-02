
SECTIONS
{
    .shell_t_area :
    {
        . = ALIGN(4);
        _shell_t_list_start = .;
        
        KEEP(*(SORT(._shell_t*)))
        
        _shell_t_list_end = .;
        . = ALIGN(4);
    } > FLASH
}
