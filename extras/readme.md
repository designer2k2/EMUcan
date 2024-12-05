# Extras

This files here are used during the automated testing.

The test is run by this GitHub Action: `.github/workflows/unit-test.yml`

`runtests.cpp` is the actual test, it runs the EMUcan library through its lifecycle.

`WProgram.h` and `WProgram.cpp` simulate the needed millis() function.

This files are included when building the library outside of the Arduino environment.
