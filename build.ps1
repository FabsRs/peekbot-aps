# Compilers
$CC = 'C:\Users\Admin\Installers\avr8-gnu-toolchain\avr8-gnu-toolchain-3.7.0.1796-win32.any.x86_64\avr8-gnu-toolchain-win32_x86_64\bin\avr-gcc.exe'
$OBJCPY = 'C:\Users\Admin\Installers\avr8-gnu-toolchain\avr8-gnu-toolchain-3.7.0.1796-win32.any.x86_64\avr8-gnu-toolchain-win32_x86_64\avr\bin\objcopy.exe'

# Definitions
$DEF = @('-D__AVR_ATmega328P__';'-DF_CPU=16000000UL')

# Flags
$CFLAGS = @('-Wall';'-Os';'-std=gnu99';'-mmcu=atmega328p';'-pedantic')
$LFLAGS = @('-w';'-Os';'-g';'-flto';'-fuse-linker-plugin';'-Wl,--gc-sections';'-mmcu=atmega328p')
$IFLAGS = @('-Iinclude';'-isystem inclide\avr')
$OFLAGS = @('-O';'ihex';'-R';'.eeprom')

# Paths
$INC = '.\include\'
$BIN = '.\bin\'
$OBJ = '.\obj\'
$SRC = '.\src\'

# Files
$OBJECTS = @('main.o';'pinout.o';'usart0.o';'ymodem.o')
$HEX = 'peekbot-aps.hex'
$ELF = 'peekbot-aps.elf'
$BOOTLOADER = 'bootloader\Arduino_UNO_bootloader.hex'

# Cleanup

Foreach ($file in $OBJECTS) {
    If (Test-Path $OBJ$file) {
        Remove-Item $OBJ$file -verbose
    }
}

If (Test-Path $BIN$ELF) {
    Remove-Item $BIN$ELF -verbose
}

If (Test-Path $BIN$HEX) {
    Remove-Item $BIN$HEX -verbose
}

# Prepare

$P_OBJECTS = $OBJECTS | ForEach-Object { "$OBJ$_" }

# Build

&$CC $IFLAGS -c $CFLAGS $DEF $SRC'main.c' -o $OBJ'main.o'
&$CC $IFLAGS -c $CFLAGS $DEF $SRC'pinout.c' -o $OBJ'pinout.o'
&$CC $IFLAGS -c $CFLAGS $DEF $SRC'usart0.c' -o $OBJ'usart0.o'
&$CC $IFLAGS -c $CFLAGS $DEF $SRC'ymodem.c' -o $OBJ'ymodem.o'

&$CC $LFLAGS $P_OBJECTS -o $BIN$ELF -lm

&$OBJCPY $OFLAGS $BIN$ELF $BIN$HEX

# ARDUINO BOOTLOADER: Uncomment the section below in case of using a parallel programmer and want to regain MCU funcitonaliy with Arduino Hardware

# # Remove end of file ('00000001FF')
# (Get-Content $BIN$HEX) -notmatch '00000001FF' | Set-Content $BIN$HEX

# # Append Arduino UNO Bootloader
# Get-Content $BOOTLOADER | Add-Content $BIN$HEX