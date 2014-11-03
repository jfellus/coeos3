################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/dynamics/Body.cpp \
../src/dynamics/DynEngine.cpp \
../src/dynamics/ModuleBody.cpp 

OBJS += \
./src/dynamics/Body.o \
./src/dynamics/DynEngine.o \
./src/dynamics/ModuleBody.o 

CPP_DEPS += \
./src/dynamics/Body.d \
./src/dynamics/DynEngine.d \
./src/dynamics/ModuleBody.d 


# Each subdirectory must supply rules for building sources it contributes
src/dynamics/%.o: ../src/dynamics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


