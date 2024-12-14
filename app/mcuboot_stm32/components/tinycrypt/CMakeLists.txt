
add_library(tinycrypt STATIC)

target_include_directories(tinycrypt
    PUBLIC
        ../../../../components/tinycrypt/lib/include
    PRIVATE
)

target_sources(tinycrypt
    PRIVATE
        ../../../../components/tinycrypt/lib/source/aes_decrypt.c
        ../../../../components/tinycrypt/lib/source/aes_encrypt.c
        ../../../../components/tinycrypt/lib/source/cbc_mode.c   
        ../../../../components/tinycrypt/lib/source/ccm_mode.c   
        ../../../../components/tinycrypt/lib/source/cmac_mode.c  
        ../../../../components/tinycrypt/lib/source/ctr_mode.c   
        ../../../../components/tinycrypt/lib/source/ctr_prng.c   
        ../../../../components/tinycrypt/lib/source/ecc.c
        ../../../../components/tinycrypt/lib/source/ecc_dh.c
        ../../../../components/tinycrypt/lib/source/ecc_dsa.c
        ../../../../components/tinycrypt/lib/source/ecc_platform_specific.c
        ../../../../components/tinycrypt/lib/source/hmac.c
        ../../../../components/tinycrypt/lib/source/hmac_prng.c
        ../../../../components/tinycrypt/lib/source/sha256.c
        ../../../../components/tinycrypt/lib/source/utils.c
)
