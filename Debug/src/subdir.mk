################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/bluetooth.cpp \
../src/configThreeLight.cpp \
../src/iBeaconInfo.cpp 

OBJS += \
./src/bluetooth.o \
./src/configThreeLight.o \
./src/iBeaconInfo.o 

CPP_DEPS += \
./src/bluetooth.d \
./src/configThreeLight.d \
./src/iBeaconInfo.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv2 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


