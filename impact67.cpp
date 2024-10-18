#include "impact67.hpp"

int main() {
    Impact67_Driver driver;
    
    if (!driver.connect()) {
        std::cerr << "Failed to connect to the device." << std::endl;
        return 1;
    }
    std::cout << "Connected to the device." << std::endl;
    
    const std::string master_port = "X0";
    std::vector<uint8_t> write_data = {0x00, 0x00};
    
    std::cout << "\n=== Reading Input Data for Port X0 ===" << std::endl;
    driver.read_input_data(master_port);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // std::cout << "\n=== Reading Output Data for Port X0 ===" << std::endl;
    // driver.read_output_data(master_port);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // std::cout << "\n=== Writing Output Data to Port X0 ===" << std::endl;
    // driver.write_output_data(master_port, write_data);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // std::cout << "\n=== Reading Output Data Again for Port X0 ===" << std::endl;
    // driver.read_output_data(master_port);
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // std::cout << "\n=== Reading Input Data Again for Port X0 ===" << std::endl;
    // driver.read_input_data(master_port);
    
    driver.disconnect();
    return 0;
}
