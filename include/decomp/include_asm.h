#ifndef _INCLUDEASM_H
#define _INCLUDEASM_H

#if !defined(SPLAT) && !defined(M2CTX) && !defined(PERMUTER) && !defined(SKIP_ASM)
    #ifdef VER_USA
        #define BASE_ASM_PATH "USA"
    #elif VER_EUR
        #define BASE_ASM_PATH "EUR"
    #elif VER_JAP0
        #define BASE_ASM_PATH "JAP0"
    #elif VER_JAP1
        #define BASE_ASM_PATH "JAP1"
    #elif VER_JAP2
        #define BASE_ASM_PATH "JAP2"
    #else
        #define BASE_ASM_PATH "USA"
    #endif

    #ifndef INCLUDE_ASM
        #define INCLUDE_ASM(FOLDER, NAME)                                                     \
            void __maspsx_include_asm_hack_##NAME()                                           \
            {                                                                                 \
                __asm__(".text # maspsx-keep \n"                                              \
                        "\t.align\t2 # maspsx-keep\n"                                         \
                        "\t.set noreorder # maspsx-keep\n"                                    \
                        "\t.set noat # maspsx-keep\n"                                         \
                        ".include \"asm/"BASE_ASM_PATH"/"FOLDER"/"#NAME".s\" # maspsx-keep\n" \
                        "\t.set reorder # maspsx-keep\n"                                      \
                        "\t.set at # maspsx-keep\n");                                         \
            }
    #endif

    #ifndef INCLUDE_RODATA
        #define INCLUDE_RODATA(FOLDER, NAME)                                        \
            __asm__(".section .rodata\n"                                            \
                    "\t.include \"asm/" BASE_ASM_PATH "/" FOLDER "/" #NAME ".s\"\n" \
                    ".section .text")
    #endif

    __asm__(".include \"macro.inc\"\n"
            "\t.set\treorder\n"
            "\t.set\tat\n");

#else
    #ifndef INCLUDE_ASM
        #define INCLUDE_ASM(FOLDER, NAME)
    #endif

    #ifndef INCLUDE_RODATA
        #define INCLUDE_RODATA(FOLDER, NAME)
    #endif
#endif

#endif
