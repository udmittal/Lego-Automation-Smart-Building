from pymodbus.client import ModbusTcpClient
import time

# ---------------------------------------------------------
# YOUR ESP32 ADDRESS (I updated this for you)
# ---------------------------------------------------------
ESP_IP = "<ENTER_ESP_IP_ADDRESS>" 

# Connect to the ESP32
print(f"Connecting to Satellite Station at {ESP_IP}...")
client = ModbusTcpClient(ESP_IP)
connection = client.connect()

if connection:
    print("✅ Connected successfully!")
else:
    print("❌ Failed to connect. Check IP or WiFi.")
    exit()

def toggle_factory(fan_on, light_on):
    # Write to Coil 100 (Fan) and 101 (Light)
    client.write_coil(101, fan_on)
    client.write_coil(100, light_on)
    
    fan_status = "ON" if fan_on else "OFF"
    light_status = "ON" if light_on else "OFF"
    print(f"Sent Command -> Fan: {fan_status} | Light: {light_status}")

# The Main Loop
try:
    while True:
        print("\n--- SATELLITE STATION CONTROL ---")
        print("0: Turn ON Everything")
        print("1: Turn OFF Everything")
        print("f: Fan Only")
        print("l: Light Only")
        print("q: Quit")
        
        cmd = input("Enter command: ").strip().lower()
        
        if cmd == '1':
            toggle_factory(True, True)
        elif cmd == '0':
            toggle_factory(False, False)
        elif cmd == 'f':
            toggle_factory(True, False)
        elif cmd == 'l':
            toggle_factory(False, True)
        elif cmd == 'q':
            break
        else:
            print("Unknown command")

except KeyboardInterrupt:
    print("\nExiting...")

client.close()
print("Connection closed.")
