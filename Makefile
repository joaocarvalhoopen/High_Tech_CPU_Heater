CC           = gcc
CFLAGS       = -O2 -std=c11 -mavx2 -pthread -Wall -Werror -fpic
CFLAGSL_LIB  = -pthread
RM           = rm -f


default: all

all: high_tech_cpu_heater high_tech_cpu_heater_lib.so

high_tech_cpu_heater: high_tech_cpu_heater.c
	$(CC) $(CFLAGS) -o high_tech_cpu_heater high_tech_cpu_heater.c

high_tech_cpu_heater_lib.so: high_tech_cpu_heater.o
	$(CC) $(CFLAGS_LIB) -shared -o high_tech_cpu_heater.so high_tech_cpu_heater.o

high_tech_cpu_heater.o: high_tech_cpu_heater.c
	$(CC) $(CFLAGS) -c  high_tech_cpu_heater.c

clean:
	$(RM) high_tech_cpu_heater
	$(RM) high_tech_cpu_heater.so
	$(RM) high_tech_cpu_heater.o


