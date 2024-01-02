import socket
import struct

# Define the UDP server address and port
UDP_HOST = "0.0.0.0"  # Listen on all available network interfaces
UDP_PORT = 12345  # The same port as used in the C++ UDP sender

# Create a UDP socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind((UDP_HOST, UDP_PORT))

while True:
    try:
        # Receive UDP data (blocking)
        data, addr = udp_socket.recvfrom(12)  # Assuming each packet is 12 bytes

        # Decode the received data into a WheelData struct
        wheel_data = struct.unpack('HBB', data)
        # print(wheel_data[0])
        steering_angle = (wheel_data[0] / 65535) * 800.0 - 400.0
        pedal1_percentage = (wheel_data[1] / 255) * 100.0
        pedal2_percentage = (wheel_data[2] / 255) * 100.0

        # Print the decoded data
        print(f"Steering Angle: {steering_angle:.2f} degrees")
        print(f"Pedal 1 Percentage: {pedal1_percentage:.2f}%")
        print(f"Pedal 2 Percentage: {pedal2_percentage:.2f}%")
    
    except KeyboardInterrupt:
        # Exit gracefully on Ctrl+C
        break
    except Exception as e:
        print(f"Error: {e}")

# Close the UDP socket
udp_socket.close()
