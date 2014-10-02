################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/graphics/Graphics.cpp \
../src/graphics/GraphicsImage.cpp \
../src/graphics/WidgetGraphics.cpp \
../src/graphics/svg.cpp 

OBJS += \
./src/graphics/Graphics.o \
./src/graphics/GraphicsImage.o \
./src/graphics/WidgetGraphics.o \
./src/graphics/svg.o 

CPP_DEPS += \
./src/graphics/Graphics.d \
./src/graphics/GraphicsImage.d \
./src/graphics/WidgetGraphics.d \
./src/graphics/svg.d 


# Each subdirectory must supply rules for building sources it contributes
src/graphics/%.o: ../src/graphics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include -I/usr/include/c++/4.8.2 -I/usr/include/librsvg-2.0 -I/usr/include/glib-2.0 -I/usr/include/gtkmm-3.0 -I/usr/include/cairomm-1.0 -I/usr/include/glibmm-2.4 -I/usr/include/gdkmm-3.0 -I/usr/include/sigc++-2.0 -I/usr/include/gtk-3.0 -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags glibmm-2.4 librsvg-2.0 gtkmm-3.0` -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


