Runtime Polymorphism on Class
===================

> Calculator Polymorphism on Class with PlatformIO.

We’d like to write code that, given an Input,

 - determines what calculator will handle it,
 - launches the calculation to produce an Output,
 - and invokes the logging of that calculator for the Input and the Output.

===================
1. Open this folder in VSCode and agree if it suggests to install extension.
2. Use `Terminal -> Run Build Task... -> PlatformIO: Build` menu to compile.
3. Run `.pio/Build/native/program`.

## Details

Use `platformio.ini` for example, see comments inside.

`include` folder contains config, required for `etl`. All is as transparent
as possible. Set all additional variables via `build_flags` option in
`platform.ini`. Currently only `PROFILE_GCC_GENERIC` set to make things work.
