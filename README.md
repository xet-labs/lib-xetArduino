# libXetArduino

🧰 A modular and reusable Arduino library collection by [Xet Labs](https://github.com/xet-labs).  
Useful across multiple projects like robotic arms, sensor systems, and more.

---

## 📦 Features

- Servo control (ESP32Servo)
- Stepper motor handling (AccelStepper)
- Distance sensors (VL53L0X)
- Wi-Fi and Networking (ESP32)
- Utility wrappers and helpers

---

## 🛠 Installation

### ✅ Option 1: Add as a Git Submodule

In your main project directory:

```bash
# Add libXetArduino as a submodule under lib/
git submodule add https://github.com/xet-labs/libXetArduino.git lib/libXetArduino

# Initialize and fetch submodule content
git submodule update --init --recursive
