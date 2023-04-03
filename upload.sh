arduino-cli compile --fqbn arduino:megaavr:nona4809 garage.ino &&
arduino-cli upload --port /dev/ttyACM0 --fqbn arduino:megaavr:nona4809 garage.ino
