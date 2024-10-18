#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
#include <map>
#include <bitset>

// Device connection details
const std::string DEVICE_IP = "192.168.1.60";
const int PORT = 44818;  // Standard EtherNet/IP port

// Assembly instances
const uint8_t ASSEMBLY_INSTANCE_INPUT = 101;
const uint8_t ASSEMBLY_INSTANCE_OUTPUT = 111;

// Port-to-byte mapping for input and output data
const std::map<std::string, std::pair<int, int>> master_ports_input = {
    {"Master", {0, 1}},
    {"X0", {10, 11}},
    {"X1", {50, 51}},
    {"X2", {90, 91}},
    {"X3", {130, 131}},
    {"X4", {170, 171}},
    {"X5", {210, 211}},
    {"X6", {250, 251}},
    {"X7", {290, 291}}
};

const std::map<std::string, std::pair<int, int>> master_ports_output = {
    {"Master", {0, 1}},
    {"X0", {2, 3}},
    {"X1", {34, 35}},
    {"X2", {66, 67}},
    {"X3", {98, 99}},
    {"X4", {130, 131}},
    {"X5", {162, 163}},
    {"X6", {194, 195}},
    {"X7", {226, 227}}
};

#pragma pack(push, 1)
struct Command {
    uint8_t service_code;
    uint8_t class_code;
    uint8_t instance;
    uint8_t attribute;
};
#pragma pack(pop)

class Impact67_Driver {
private:
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;
    
public:
    Impact67_Driver() : socket(io_context) {}
    
    bool connect() {
        try {
            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::connect(socket, resolver.resolve(DEVICE_IP, std::to_string(PORT)));
            return true;
        } catch (std::exception& e) {
            std::cerr << "Connection failed: " << e.what() << std::endl;
            return false;
        }
    }
    
    void disconnect() {
        if (socket.is_open()) {
            socket.close();
        }
    }

    std::vector<uint8_t> sendCommand(const Command& command, const std::vector<uint8_t>& request_data = {}) {
        try {
            // Send command
            boost::asio::write(socket, boost::asio::buffer(&command, sizeof(command)));
            
            if (!request_data.empty()) {
                boost::asio::write(socket, boost::asio::buffer(request_data));
            }
            
            // Receive response
            std::vector<uint8_t> response(1024);
            size_t len = socket.read_some(boost::asio::buffer(response));
            response.resize(len);
            
            return response;
        } catch (std::exception& e) {
            std::cerr << "Command execution failed: " << e.what() << std::endl;
            return {};
        }
    }
    
    void read_input_data(const std::string& port) {
        Command cmd{0x0E, 0x04, ASSEMBLY_INSTANCE_INPUT, 3};
        std::cout << "Sending read command to get current output data...\n" << std::endl;
        auto response = sendCommand(cmd);
        
        if (!response.empty()) {
            auto port_bytes = master_ports_input.at(port);
            std::cout << "\nReading input data for IO-Link port " << port << ":\n";
            for (int i = 0; i < 2; ++i) {
                int byte_index = port_bytes.first + i;
                if (byte_index < response.size()) {
                    std::cout << "Byte " << byte_index << " (Port " << port << "): "
                              << std::bitset<8>(response[byte_index]) << " (binary), "
                              << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                              << static_cast<int>(response[byte_index]) << " (hex)\n";
                }
            }
        } else {
            std::cout << "Failed to read input data for port " << port << std::endl;
        }
    }
    
    void read_output_data(const std::string& port) {
        Command cmd{0x0E, 0x04, ASSEMBLY_INSTANCE_OUTPUT, 3};
        auto response = sendCommand(cmd);
        
        if (!response.empty()) {
            auto port_bytes = master_ports_output.at(port);
            std::cout << "\nReading output data for IO-Link port " << port << ":\n";
            for (int i = 0; i < 2; ++i) {
                int byte_index = port_bytes.first + i;
                if (byte_index < response.size()) {
                    std::cout << "Byte " << byte_index << " (Port " << port << "): "
                              << std::bitset<8>(response[byte_index]) << " (binary), "
                              << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                              << static_cast<int>(response[byte_index]) << " (hex)\n";
                }
            }
        } else {
            std::cout << "Failed to read output data for port " << port << std::endl;
        }
    }
    
    void write_output_data(const std::string& port, const std::vector<uint8_t>& hex_data) {
        // First, read current output data
        Command read_cmd{0x0E, 0x04, ASSEMBLY_INSTANCE_OUTPUT, 3};
        
        auto current_data = sendCommand(read_cmd);
        
        if (!current_data.empty() && hex_data.size() >= 2) {
            auto port_bytes = master_ports_output.at(port);
            current_data[port_bytes.first] = hex_data[0];
            current_data[port_bytes.first + 1] = hex_data[1];
            
            Command write_cmd{0x10, 0x04, ASSEMBLY_INSTANCE_OUTPUT, 3};
            auto write_response = sendCommand(write_cmd, current_data);
            
            if (!write_response.empty()) {
                std::cout << "\nSuccessfully wrote to port " << port << std::endl;
            } else {
                std::cout << "Failed to write to port " << port << std::endl;
            }
        } else {
            std::cout << "Failed to read output data for port " << port << " before writing" << std::endl;
        }
    }
};

