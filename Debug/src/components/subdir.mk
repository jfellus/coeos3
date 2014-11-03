################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/components/BezierHandle.cpp \
../src/components/ChangeGroupCreator.cpp \
../src/components/Component.cpp \
../src/components/Creator.cpp \
../src/components/LinkComponent.cpp \
../src/components/LinkCreator.cpp \
../src/components/ModuleCreator.cpp 

OBJS += \
./src/components/BezierHandle.o \
./src/components/ChangeGroupCreator.o \
./src/components/Component.o \
./src/components/Creator.o \
./src/components/LinkComponent.o \
./src/components/LinkCreator.o \
./src/components/ModuleCreator.o 

CPP_DEPS += \
./src/components/BezierHandle.d \
./src/components/ChangeGroupCreator.d \
./src/components/Component.d \
./src/components/Creator.d \
./src/components/LinkComponent.d \
./src/components/LinkCreator.d \
./src/components/ModuleCreator.d 


# Each subdirectory must supply rules for building sources it contributes
src/components/%.o: ../src/components/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/include/webkitgtk-3.0 -I/usr/include/cairo -I/usr/include/sigc++-2.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags gtk+-3.0 webkitgtk-3.0 ` -std=c++11 -rdynamic -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


