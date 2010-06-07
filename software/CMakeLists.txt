cmake_minimum_required(VERSION 2.6)
SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CMCU "-mmcu=atmega328p")
SET(CDEFS "-DF_CPU=16000000")


SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${COPT}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

##<lcd>
#SET(SRC_LCD avrlib/lcd/font5x8.h)

#INCLUDE_DIRECTORIES(avrlib/lcd)

##</lcd>

SET(SRC phy.c phy.h mac.c mac.h usart.c usart.h global.h)

ADD_EXECUTABLE(1_wire_protocole.out main.c ${SRC}) 
#ADD_EXECUTABLE(compte.out ${SRC}) 


