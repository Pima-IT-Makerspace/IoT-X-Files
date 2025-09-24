Some errors that may be encountered during reconstruction of this are that the motor boards sometimes need power to the 5v slot as well from the ESP-32 (VIN) because the 5v slot on the motorboard controls the logic functions. There was, however, an anomaly, as one out of the many boards was able to just use the 12v and function normally.

However, the downside is that if we put the full 12v to the motor board, it will fry the esp-32. This is because the 12v's don't seem to be stepped down to the 5v port, and therefore sends too much.
