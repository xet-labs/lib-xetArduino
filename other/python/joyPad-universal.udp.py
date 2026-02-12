#!/usr/bin/env python3
import argparse
import pygame
import socket
import struct
from datetime import datetime

# ------------------ Command-line arguments ------------------
parser = argparse.ArgumentParser(description="Xbox controller UDP sender for ESP32")
parser.add_argument("--ip", type=str, default="robSoc.local", help="Device IP (default: robSoc.local)")
parser.add_argument("--port", type=int, default=8888, help="Device UDP port (default: 8888)")
args = parser.parse_args()

UDP_IP = args.ip
UDP_PORT = args.port

print(f"Target -> IP: {UDP_IP} | Port: {UDP_PORT}")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# ------------------ Initialize Pygame Joystick ------------------
pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick detected!")
    exit(1)

joy = pygame.joystick.Joystick(0)
joy.init()
print(f"Controller detected: {joy.get_name()}")

# ------------------ Controller state ------------------
state = {
    "buttons": 0,
    "LX": 0,
    "LY": 0,
    "RX": 0,
    "RY": 0,
    "LT": 0,
    "RT": 0
}

# Button -> bit mapping
button_map = {
    0: 0,   # A
    1: 1,   # B
    2: 2,   # X
    3: 3,   # Y
    4: 4,   # LB
    5: 5,   # RB
    6: 6,   # Back
    7: 7,   # Start
    8: 8,   # LStick
    9: 9,   # RStick
}

# ------------------ D-Pad (hat) mapping ------------------
def update_dpad(hat):
    global state
    x, y = hat
    # Left / Right
    if x == -1:
        state["buttons"] |= (1 << 12)
        state["buttons"] &= ~(1 << 13)
    elif x == 1:
        state["buttons"] |= (1 << 13)
        state["buttons"] &= ~(1 << 12)
    else:
        state["buttons"] &= ~((1 << 12) | (1 << 13))
    # Up / Down
    if y == 1:
        state["buttons"] |= (1 << 10)
        state["buttons"] &= ~(1 << 11)
    elif y == -1:
        state["buttons"] |= (1 << 11)
        state["buttons"] &= ~(1 << 10)
    else:
        state["buttons"] &= ~((1 << 10) | (1 << 11))

# ------------------ State printing ------------------
def print_state(state, packet=None):
    ts = datetime.now().strftime("%H:%M:%S.%f")[:-2]
    button_names = {
        0: "A", 1: "B", 2: "X", 3: "Y",
        4: "LB", 5: "RB", 6: "Back", 7: "Start",
        8: "LStick", 9: "RStick", 10: "DPadUp", 11: "DPadDown",
        12: "DPadLeft", 13: "DPadRight"
    }
    pressed = [name for bit, name in button_names.items() if state["buttons"] & (1 << bit)]
    
    print(f"{ts}   LX:{state['LX']:06d}   LY:{state['LY']:06d}   RX:{state['RX']:06d}   "
          f"RY:{state['RY']:06d}   LT:{state['LT']:03d}   RT:{state['RT']:03d}   "
          f"Buttons: 0x{state['buttons']:04X} ({', '.join(pressed) if pressed else 'None'})")
    if packet:
        print(f"   Tx: {packet.hex()}")

# ------------------ Main loop ------------------
clock = pygame.time.Clock()
while True:
    pygame.event.pump()

    # Buttons
    state["buttons"] = 0
    for btn_idx, bit in button_map.items():
        if joy.get_button(btn_idx):
            state["buttons"] |= (1 << bit)

    # Axes
    state["LX"] = int(joy.get_axis(0) * 32767)
    state["LY"] = int(joy.get_axis(1) * 32767)
    state["RX"] = int(joy.get_axis(3) * 32767)
    state["RY"] = int(joy.get_axis(4) * 32767)
    # Triggers 0-255
    state["LT"] = int((joy.get_axis(2)+1)/2*255)
    state["RT"] = int((joy.get_axis(5)+1)/2*255)

    # D-Pad / Hat
    if joy.get_numhats() > 0:
        update_dpad(joy.get_hat(0))

    # Pack and send UDP packet
    packet = struct.pack('<HhhhhBB',
                         state["buttons"],
                         state["LX"],
                         state["LY"],
                         state["RX"],
                         state["RY"],
                         state["LT"],
                         state["RT"])

    sock.sendto(packet, (UDP_IP, UDP_PORT))

    # Print state
    print_state(state, packet)

    clock.tick(60)  # Limit to 60 FPS
