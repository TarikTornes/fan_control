<img src="./resources/logoFanControl.png" width="40%" height="40%" align="right">

# Fan\_Control (0.3.1)
A simple and lightweight daemon for controlling the cooling system of your Raspberry Pi.

This project was developed as a hands-on exercise to gain familiarity with the C programming language and to explore the setup of a DIY home server.

## Pre-Requisites
### Hardware & Operating System
- **Supported Devices:** This daemon is tested and confirmed to run on the Raspberry Pi 4B with Raspberry Pi OS (Debian-based).

- **Compatibility:** While the instructions provided are specific to the Raspberry Pi 4B, they are likely applicable to other Raspberry Pi models running a Debian-based distribution.

### Cooling System
- **Recommended Cooling System:** The project is designed for use with the [Ice Tower CPU Cooling Fan](https://52pi.com/products/52pi-ultra-thin-ice-tower-cooler-cooling-fan-for-raspberry-pi-4-model-b-cpu-fan).

- **Alternative Cooling Systems:** Other cooling systems can be used as long as they connect the fan to GPIO pins via 5V, GND, and TXD. To ensure speed control, the fan should support PWM (Pulse Width Modulation).


## Installation

### Building Fan\_Control from Source (Raspbian)

1. **Install Dependencies**

   Before installing and using Fan\_Control, ensure that the WiringPi library is installed. This library is necessary for controlling the fan via the GPIO pins on your Raspberry Pi. Follow the instructions in the [WiringPi Installation Guide](https://github.com/WiringPi/WiringPi).

2. **Build Fan_Control from Source**

   Open a terminal and execute the following commands:
   ```bash
   # Install Git
   sudo apt install git
   
   # Clone the repository
   git clone https://github.com/TarikTornes/fan_control.git
   cd fan_control

   # Build the package
   sudo make
   ```

3. **Daemonize Fan_Control**

    To run Fan\_Control as a daemon using `systemd`, follow these steps:

    Open a terminal and execute the following commands:
    ```bash
    # Reload systemd manager configuration
    sudo systemctl daemon-reload

    # Enable Fan\_Control to start on boot
    sudo systemctl enable fan_control.service

    # Start the Fan\_Control service
    sudo systemctl start fan_control.service

    # Check the status of the service
    sudo systemctl status fan_control.service
    ```

## Configuration
After building Fan\_Control from source, a configuration file should be available at /etc/fan\_control.conf.

#### Configuration options
- `temp1` and `temp2` are temperature thresholds in Celsius that determine the fan speed of your cooling system.

#### Example configuration
```
[General]
temp1 : 30
temp2 : 50
```

#### How it works
With the above configuration:

- The fan remains OFF when the temperature is at or below 30°C.
- The fan runs at MID speed when the temperature is between 30°C and 50°C.
- The fan operates at FULL speed when the temperature exceeds 50°C.


