<h1 align="center">MagoFly</h1>
<p align="center">
  <img
    src="img/drone.jpg"
    alt="drone"
    width=49%
  />
  <img
    src="img/rc_on.jpg"
    alt="rc"
    width=49%
  />
</p>
<h2 align="center">MagoFly: a basic drone project</h2>

## ✨ What's MagoFly?
MagoFly is a drone capable of doing basic manoeuvres which is **controlled** by a **remote controller wirelessly** using **nRF24L01+ radio module**.

## 🚀 Quickstart

### 💻 Installation
- What we need:

| Module           | Why We need it                                                                              | Qte                                                                     |
|------------------|------------------------------------------------------------------------------------|----------------------------------------------------------------------------------|
| Arduino nano     | the brain of the whole operations                                                           | 4                                                                       |
| nRF24L01+        | radio communication                                                                         | 3                                                                       |
| DJI450 FlameWheel| drone frame                                                                                 | 1                                                                       |
| MPU6050          | inertial measurment unit to get the accelerometer data                                      | 1                                                                       |
| LED              | (white)  light the drone when its dark                                                      | 8                                                                       |
| BLDC motors      | to provide enough lift to fly and manoevre the drone                                        | 4                                                                       |
| ESC              | (40A)  control the BLDC motors                                                              | 4                                                                       |
| NEO-6M GPS       | telemetry/coordonation data                                                                 | 1                                                                       |
| ESP32-CAM        | to see from above                                                                           | 1                                                                       |
| LiPo Battery     | power the drone (preferably a high C rating for longer flight time)                         | 1                                                                       |
| LCD display      |(st7920 IC) to show to reading on the RC                                                     | 1                                                                       |
| Joystick         |control the movements and the throttle of the drone                                          | 2                                                                       |
| Push buttons     |change the handmode for lefties and righties/enable lights / enable altitude control         | 3                                                                       |
| Toggle switch    | cut off engines                                                                             | 1                                                                       |
| Buzzer           | make asound everytime the altitude control or the buttons are pushed                        | 1                                                                       |
| Potentiometer    | determin max altitude                                                                       | 1                                                                       |
| Resistors        |(10k ohms) to link with the bush buttons between VCC & GND pins so the RC don't short circuit| 3                                                                       |
| Jumper wires     |  connect every thing                                                                        | ****LOT****                                                             |
| soldering board  | soldering everything on it                                                                  | 2                                                                       |
| zip ties         | hold everything to its proper place                                                         | 6                                                                       |

- How to connect all the components:
  - <p white-space=pre>-------------------for the RC follow this diagram----------------------------for the drone follow this diagram-------</p><br>
    <img
    src="circuits_schematics/Controller_circuit.png"
    alt="drone"
    width=49%
    />
    <img
    src="circuits_schematics/Drone_circuit.png"
    alt="drone"
    width=49%
    />
  - solder each of them on a seperate soldering board.
- change the SSID and the PASSWORD on the [`camera script`](.camera/CameraWebServer/CameraWebServer.ino) to your wifi' ssid and password:
  ```c
  //change these if needed
  const char *ssid = "elbo7's" ;
  const char *password = "NiggaNigga";
  ```
- Upload the code to each board.
- ****FLY!****

### 🛠️ Be cautious
- When labeling the BLDC motors in a clockwise direction: **1st** and **3rd**motors are spinning **Counter ClockWise**, **2nd** and **4th** spinning **ClockWise**.
- The drone **doesn't have** a **protocol** for when it **loses connection** with the RC. **In other words, it will hold the last inputs until it regain connection**.
- **Emergency Landing** and **CutOffEngine** are **irreversible**. After these actions are engaged, unplug the battery then re-plug it again if you want to fly the drone(**THIS WAS A SAFETY DESIGN**).
- The Drone is uncapable of doing diagonal movements. **So DO NOT move the joysticks is croos patterns!**

## 🚀 It's Flight Time
Follow this procedures:
- 1st: Power on the RC and **WAIT** for the animation to finish.
  <img
    src="img/rc_init.gif"
    alt="drone"
    width=100%
  />
- 2nd: Plug the battery connector and wait for the lights to flash.
  <img
    src="img/drone_flash_lights.gif"
    alt="drone"
    width=100%
  />
- 3rd: On the RC, click on the blue button (the toggle light) to see if there is a solid connection between thedrone and the RC.
- 4th: If the lights turned on, FLY! If not, get some distance between the RC and the Drone then try again. If that also fails, check the radio modules and see if the drone gets the sent data. If that fails, unplug the battery and repeat from step 2.

## 🎮 About the remote
- The left hand mode, the left joystick controls the **PITCH** and **ROLL** movements and the button enable/disable throttle change. You can know if the throttle is disables when you see horizontal line 
under the "THR" on the screen. While the right joystick controls the **THROTTLE** and **YAW** movements and the buttons toggle the meregency landing.

- The right hand mode, the right joystick controls the **PITCH** and **ROLL** movements and the button enable/disable throttle change. You can know if the throttle is disables when you see horizontal line 
under the "THR" on the screen. While the left joystick controls the **THROTTLE** and **YAW** movements and the buttons toggle the meregency landing.
