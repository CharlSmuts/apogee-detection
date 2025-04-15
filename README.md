Arduino program that uses a bmp180 sensor to monitor the altitude and apogee of a water rocket.

When the rocket decends it will trigger a servo movement to release the parachute to ensure that the rocket doesnt get damaged.

V1 in the main branch is working but the code was badly written. while using it I did experience one false positive where the patachute was released while the rocket was still on the launnch platform.

V2 aims to fix both of these issues. It is currently untesed and is slightly slower than V1 but most likely fixed the false positive issue.


