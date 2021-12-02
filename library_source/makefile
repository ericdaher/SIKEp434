####  Makefile for compilation on Unix-like operative systems  ####

OPT=-Os     # Optimization option by default

CC=arm-none-eabi-gcc
COMPILER=arm-none-eabi-gcc

ARCHITECTURE=_ARM_
USE_OPT_LEVEL=_GENERIC_
ARM_TARGET=YES

ifeq "$(OPT_LEVEL)" "GENERIC"
	USE_OPT_LEVEL=_GENERIC_
endif

ifeq "$(ARM_TARGET)" "YES"
	ARM_SETTING=-lrt
endif

ifeq "$(ARCHITECTURE)" "_AMD64_"
	ifeq "$(USE_OPT_LEVEL)" "_FAST_"
		MULX=-D _MULX_
		ifeq "$(USE_MULX)" "FALSE"
			MULX=
		else
			ADX=-D _ADX_
			ifeq "$(USE_ADX)" "FALSE"
				ADX=
			endif
		endif
	endif
endif

AR=ar rcs
RANLIB=ranlib

ADDITIONAL_SETTINGS=

CFLAGS=$(OPT) -std=gnu11 $(ADDITIONAL_SETTINGS) -D $(ARCHITECTURE) -D __NIX__ -D $(USE_OPT_LEVEL) $(MULX) $(ADX) -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug\board" -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug\source" -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug" -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug\CMSIS" -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug\drivers" -I"C:\Users\ericd\Documents\MCUXpressoIDE_11.4.0_6237\workspace\SIKE_library_debug\utilities" -fno-common -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../source/"=. -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -MMD -MP
LDFLAGS=-lm
ifeq "$(USE_OPT_LEVEL)" "_GENERIC_"
	EXTRA_OBJECTS_434=objs434/fp_generic.o
else ifeq "$(USE_OPT_LEVEL)" "_FAST_"
ifeq "$(ARCHITECTURE)" "_AMD64_"
	EXTRA_OBJECTS_434=objs434/fp_x64.o objs434/fp_x64_asm.o
endif
endif
OBJECTS_434=objs434/P434.o $(EXTRA_OBJECTS_434) objs/random.o objs/fips202.o

# Primeiro compilar lib434, depois compilar lib434_for_KATs

all: lib434 # lib434 lib434_for_KATs tests KATS

objs434/%.o: P434/%.c
	@mkdir -p $(@D)
	$(CC) -c -g3 $(CFLAGS) $< -o $@

ifeq "$(USE_OPT_LEVEL)" "_GENERIC_"     
    objs434/fp_generic.o: P434/generic/fp_generic.c
	    $(CC) -c -g3 $(CFLAGS) P434/generic/fp_generic.c -o objs434/fp_generic.o
else ifeq "$(USE_OPT_LEVEL)" "_FAST_"
ifeq "$(ARCHITECTURE)" "_AMD64_"        
	objs434/fp_x64.o: P434/AMD64/fp_x64.c
		$(CC) -c -g3 $(CFLAGS) P434/AMD64/fp_x64.c -o objs434/fp_x64.o

	objs434/fp_x64_asm.o: P434/AMD64/fp_x64_asm.S
		$(CC) -c -g3 $(CFLAGS) P434/AMD64/fp_x64_asm.S -o objs434/fp_x64_asm.o
endif
endif

objs/random.o: random/random.c
	@mkdir -p $(@D)
	$(CC) -c -g3 $(CFLAGS) random/random.c -o objs/random.o

objs/fips202.o: sha3/fips202.c
	$(CC) -c -g3 $(CFLAGS) sha3/fips202.c -o objs/fips202.o

lib434: $(OBJECTS_434)
	rm -rf lib434 sike
	mkdir lib434 sike
	arm-none-eabi-ar -rcs lib434/libsike.a $^
	arm-none-eabi-ranlib lib434/libsike.a

# tests: lib434
# 	$(CC) $(CFLAGS) -L./lib434 tests/test_SIKEp434.c tests/test_extras.c -lsike $(LDFLAGS) -o sike/test_KEM $(ARM_SETTING)

# AES
AES_OBJS=objs/aes.o objs/aes_c.o

objs/%.o: tests/aes/%.c
	@mkdir -p $(@D)
	$(CC) -c -g3 $(CFLAGS) $< -o $@

lib434_for_KATs: $(OBJECTS_434) $(AES_OBJS)
	arm-none-eabi-ar -rcs lib434/libsike_for_testing.a $^
	arm-none-eabi-ranlib lib434/libsike_for_testing.a

# KATS: lib434_for_KATs
# 	$(CC) $(CFLAGS) -L./lib434 tests/PQCtestKAT_kem434.c tests/rng/rng.c -lsike_for_testing $(LDFLAGS) -o sike/PQCtestKAT_kem $(ARM_SETTING)

# check: tests

.PHONY: clean

clean:
	rm -rf *.req objs434* objs lib434* sike 

