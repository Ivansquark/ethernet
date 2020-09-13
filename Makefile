.PHONY: all clean load 

TARGET = src/main.cpp
INC = inc/
LIB = lib/
SRC = src/

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
#for freeRTOS
CFLAGS = -Wall -g -O0 -specs=nosys.specs -specs=nano.specs \
	 -c -fno-common -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
	 -Wno-pointer-arith 
CPPFLAGS = -c -Wall -g -O0 --specs=nosys.specs -specs=nano.specs \
	   -nostartfiles -fno-common -D"assert_param(x)=" \
	   -mcpu=cortex-m4 -mthumb -march=armv7e-m -fno-exceptions \
	   -Wno-pointer-arith -fno-rtti -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
	   -u_printf_float -ffast-math -fno-math-errno \
	   -ffunction-sections
LDFLAGS = -specs=nano.specs -specs=nosys.specs \
	  -marmelf --gc-sections -lgcc\
	  -L/usr/lib/gcc/arm-none-eabi/7.3.1/ \
	  -L/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/ \
	  -I/lib/arm-none-eabi/newlib/thumb/v7e-m/
LFLAGS = -mcpu=cortex-m4 -mthumb -nostartfiles -lm -lc -lgcc \
		 -specs=nano.specs -specs=nosys.specs -fno-exceptions -fno-rtti \
		 -u_printf_float -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
		 -L/usr/lib/arm-none-eabi/newlib/thumb/v7e-m/fpv4-sp/hard/
		#для FPU
load:	main.bin
	st-info --probe
	st-flash write main.bin 0x08000000
	#st-info --probe
	
all:	main.bin main.lst main.elf
main.bin: main.elf
	arm-none-eabi-objcopy main.elf main.bin -O binary
main.lst: main.elf
	arm-none-eabi-objdump -D main.elf > main.lst

main.elf: startup.o malloc.o main.o
	$(CC) -o main.elf -T$(LIB)stm32f4.ld startup.o malloc.o main.o \
	$(LFLAGS) -Xlinker -Map=main.map 
	# -z  muldefs
	arm-none-eabi-size main.elf
	
startup.o: $(LIB)startup.cpp
	$(CC) $(LIB)startup.cpp -o startup.o $(CPPFLAGS) 
	#arm-none-eabi-objdump startup.o -h
malloc.o: $(SRC)malloc.cpp $(INC) 
	$(CC) $(SRC)malloc.cpp -o malloc.o -I$(INC) $(CPPFLAGS)

main.o: $(TARGET) $(INC) 
	$(CC) $(TARGET) -o main.o -I$(INC) -I$(LIB) $(CPPFLAGS)
	#arm-none-eabi-objdump main.o -h 
				
clean:
	rm -rf *.o *.elf *.lst *.bin *.map
