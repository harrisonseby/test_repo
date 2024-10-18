#include <cstdint>

struct IO_Link_Port_Parameters
{
    uint8_t io_link_function_pin_4;                          // Values: 0 = Deactivated, 1 = IO-Link Manual Configuration, ..., 97 = Digital Input NC
    uint8_t validation_backup;                               // Values: 0 = No Device check, 1 = Type compatible Device V1.0, ..., 4 = Type compatible Device V1.1, Restore
    uint8_t behavior_pin_2;                                  // Values: 0 = Automatic Mode (DIO), 1 = Digital Input, ..., 7 = Deactivated
    uint8_t port_cycle_time;                                 // Values: 0 = As fast as possible, 1-255 = Multiplier/TimeBase in ms
    uint16_t vendor_id;                                      // Vendor ID for the IO-Link device with validation setting
    uint32_t device_id;                                      // Device ID for the attached IO-Link device with validation setting
    uint8_t io_link_prozess_data_swap;                       // Values: 0 = No Swap, 1 = 16 Bit Swap, ..., 3 = Full Swap
    uint8_t io_link_event_integration;                       // Values: 0 = Standard Integration, 1 = Murrelektronik Integration, ..., 2 = Murrelektronik IO-Link Extended Integration
    uint8_t digital_input_signal_filter_pin_4;               // Values: 0 = No Filter, 1 = 1 ms, ..., 5 = 15 ms
    uint8_t digital_input_signal_filter_pin_2;               // Values: 0 = No Filter, 1 = 1 ms, ..., 5 = 15 ms
    uint8_t digital_output_failsafe_mode_pin_4;              // Values: 0 = Force 0, 1 = Force 1, 2 = Last State
    uint8_t digital_output_failsafe_mode_pin_2;              // Values: 0 = Force 0, 1 = Force 1, 2 = Last State
    uint8_t digital_port_function_configuration_pin_1;       // Values: 0 = Deactivated, 1 = L+ (US) Sensor Supply
    uint8_t output_current_limitation_pin_1;                 // Values: 0 = 0.5A, 1 = 1.0A, ..., 3 = 2.0A
    uint8_t output_current_limitation_pin_2;                 // Values: 0 = 0.5A, 1 = 1.0A, ..., 3 = 2.0A
    uint8_t output_current_limitation_pin_4;                 // Values: 0 = 0.5A, 1 = 1.0A, ..., 3 = 2.0A
    uint8_t reserved[4];                                     // Reserved for future use
};


struct Instance_111
{
    uint8_t pin_port_layout;
    IO_Link_Port_Parameters port_X0;
    IO_Link_Port_Parameters port_X1;  
};