# Simulink / MATLAB Code Generation

Model currently in use is from the [SingleRotor-UAV](https://github.com/SolidGeek/SingleRotorUAV/tree/main/MATLAB/NonlinearSimulation) project and does not support direct C code generation from Simulink model in its entirety using Simulink Coder. 

## Simulink Coder

Simulink Coder is used to generate C code from a Simulink model block diagram. While code generation is inherently supportede for all built-in blocks, any user-defined
MATLAB [S-functions](https://www.mathworks.com/help/simulink/sfg/s-function-features.html) require a corresponding TLC file or must be remapped to C with a 
corresponding C Mex source file and TIP.tlc file in the same directory as the S-function (which contains %% CallAsCMexLevel1 or --"--Level2 on the first line).

## MATLAB Coder

[MATLAB Coder](https://www.mathworks.com/help/coder/ug/convert-matlab-code-to-fixed-point-c-code.html) generates ANSI C/C++ code from functions, first checking whether all 
functions are [compatible with code generation](https://www.mathworks.com/help/coder/ug/matlab-language-features-supported-for-code-generation.html). If not, MATLAB 
functions must be modified to be written with only compatible language features or manually translated into the desired language.

## Options for Integration of Simulink Model

1. Modify control code from (https://github.com/SolidGeek/SingleRotorUAV) to utilize for initial SoB integration.

2. Generate code from model (excluding the S-function block) using Simulink Coder, manually translate nonlinear dynamics functions to C code, and reintegrate.

3. Manually translate nonlinear dynamics functions into C code and integrate C functions into the Simulink model using the 
[legacy code tool](https://www.mathworks.com/help/simulink/sfg/integrating-existing-c-functions-into-simulink-models-with-the-legacy-code-tool.html) to enable tuning of model and full code generation.
