Wifi car controlled by a gamepad, supports most controllers, tested on xbox controller.

**Errors / Problems:**

Some errors that may be encountered during reconstruction of this are that the motor boards sometimes need power to the 5V slot as well from the ESP-32 (VIN) because the 5V slot on the motorboard controls the logic functions. There was, however, an anomaly, as one out of the many boards was able to just use the 12V and function normally.

However, the downside is that if we put the full 12V to the motor board, it will fry the ESP-32. This is because the 12v's don't seem to be stepped down to the 5v port, and therefore sends too much.

**Instructions:**

1.) Set up the wifi details in the code to the network you are planning to connect to.

2.) Find the IP address of the ESP-32 you are using for Speedy. This can be done using the router's wireless list of devices, nmap, or Wireshark. The easiest way to do this is to flash the ESP-32 with the code, and the serial monitor in the Arduino will show the IP address for you. Keep in mind that the baud rate has to be set in the serial monitor to the same as the one specified in the serial begin in the code.

3.) In your browser, Type in the IP address. (IMPORTANT) Type in HTTP:// specifically, because if you don't, the browser likes to default to https, and nothing will show up.

4.) Once the page is displayed, the buttons work, but its speed scale may not be a high enough value to actually make your motors spin.

5.) So for a more effective experience, plug in some type of controller, we have tons of gamepads, but any should work. - Speed is controlled by the left and right triggers, left being to decrease speed, right being to increase it. - Direction is controlled by the face button letters, like X,A,B,Y.

6.) Drive mr speedy and enjoy.

****Extra:****

Many variables can be changed to your liking, like the speed curve can be increased or decreased with the map function.
