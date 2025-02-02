$CC = 'C:\Users\Admin\Installers\avr8-gnu-toolchain\avr8-gnu-toolchain-3.7.0.1796-win32.any.x86_64\avr8-gnu-toolchain-win32_x86_64\bin\avr-gcc.exe'
$OBJCPY = 'C:\Users\Admin\Installers\avr8-gnu-toolchain\avr8-gnu-toolchain-3.7.0.1796-win32.any.x86_64\avr8-gnu-toolchain-win32_x86_64\avr\bin\objcopy.exe'
$CFLAGS = @('-Wall';'-pedantic';'-Os';'-std=gnu99';'-mmcu=atmega328p';'-D__AVR_ATmega328P__';'-DF_CPU=16000000UL')
$IFLAGS = @('-Iinclude';'-isystem inclide\avr')
$OBJFLAGS = @('-O';'ihex';'-R';'.eeprom')

$OBJECTS = @('main.o';'pinout.o';'usart0.o';'ymodem.o')
$HEX = 'peekbot-aps.hex'
$ELF = 'peekbot-aps.elf'

$INC = '.\include\'
$BIN = '.\bin\'
$OBJ = '.\obj\'
$SRC = '.\src\'

# OBJECTS
&$CC $IFLAGS -c $CFLAGS $SRC'main.c' -o $OBJ'main.o'
# &$CC $IFLAGS -c $CFLAGS $SRC'pinout.c' -o $OBJ'pinout.o'
# &$CC $IFLAGS -c $CFLAGS $SRC'usart0.c' -o $OBJ'usart0.o'
# &$CC $IFLAGS -c $CFLAGS $SRC'ymodem.c' -o $OBJ'ymodem.o'

# # ELF
# $CC $CFLAGS $OBJECTS -Wl,-u,vfprintf -lprintf_flt -lm -o $BIN$ELF

# # HEX
# $OBJCPY $OBJFLAGS $ELF $BIN$HEX