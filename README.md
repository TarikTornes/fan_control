# Fan\_Control (Beta)
A simple and lightweight daemon for controlling the cooling system of your Raspberry Pi.

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


