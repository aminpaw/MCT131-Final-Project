/*
  Design and created by Blascarr
  DebugInterface
  Name    : Blascarr
  Description: DebugInterface.h
  version : 1.0
        DebugInterface is an interface to debug Hardware Sensor to test and 
        read the response by Serial or another interface available.

        Blascarr invests time and resources providing this open source code like
  some other libraries, please respect the job and support open-source software.

        Written by Adrian for Blascarr
*/

#ifndef _DEBUGINTERFACE_H
#define _DEBUGINTERFACE_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

// Pattern for DEBUG protocol detection
#ifndef DEBUG_STARTCMD
    #define DEBUG_STARTCMD "{"
#endif
#ifndef DEBUG_ENDCMD
    #define DEBUG_ENDCMD "}"
#endif
#ifndef DEBUG_SEPCM
    #define DEBUG_SEPCMD ","
#endif

#if defined(MEGA_DEBUG) && (defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__))
    #if MEGA_DEBUG == 1
        #define SERIALDEBUG Serial1
    #elif MEGA_DEBUG == 2
        #define SERIALDEBUG Serial2
    #elif MEGA_DEBUG == 3
        #define SERIALDEBUG Serial3
    #else
        #define SERIALDEBUG Serial
    #endif
#endif

#if defined(SERIAL_DEBUG)
    #include "Arduino.h"
    #define SERIALDEBUG Serial
#else
    #error "SERIAL_DEBUG not defined.
#endif

class SerialDebugger {
    public:
        SerialDebugger (){

        };
        void prints(const char* message){printFromProgmem( message );};
        void printsln(const char* message){
            printFromProgmem( message );
            SERIALDEBUG.println();
        };
        void printv(const int* value){
            SERIALDEBUG.print(readIntFromProgmem( value) );
        };
        void printv(const float* value){
            SERIALDEBUG.print(readFloatFromProgmem( value) );
        };

        void printvln(const int* value){
            printv( value );
            SERIALDEBUG.println();
        };

        void printvln(const float* value){
            printv( value );
            SERIALDEBUG.println();
        };

        template <typename T>
        void dump(const char* message, T value) {
            printFromProgmem(message);
            SERIALDEBUG.print( " " );
            SERIALDEBUG.print(value);
        }

        template <typename T>
        void dumpln(const char* message, T value) {
            printFromProgmem(message);
            SERIALDEBUG.print( " " );
            SERIALDEBUG.print(value);
            SERIALDEBUG.println();
        }

    private:
        void printFromProgmem(const char* progmem_ptr) {
            char c;
            while ((c = pgm_read_byte(progmem_ptr++)) != 0) {
                SERIALDEBUG.print(c);
            }
        }
        int readIntFromProgmem(const int* progmem_ptr) {
            return pgm_read_word(progmem_ptr);
        }
        float readFloatFromProgmem(const float* progmem_ptr) {
            return pgm_read_float(progmem_ptr);
        }
};

#endif