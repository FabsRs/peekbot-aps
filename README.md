<a id="readme-top"></a>

# PEEKBOT-APS

TODO: Add nice picture of APS

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#coding-standard">CODING STANDARD</a>
      <ul>
        <li><a href="#resources">Resources</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">GETTING STARTED</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#building">Building</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">USAGE</a></li>
    <li><a href="#contributing">CONTRIBUTORS</a></li>
    <li><a href="#license">LICENSE</a></li>
    <li><a href="#figures">FIGURES</a></li>
  </ol>
</details>

<!-- CODING STANDARD -->
## CODING STANDARD

### Resources
List of prerequisites: https://tinusaur.com/guides/avr-gcc-toolchain/

GCCAVR toolchain: https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers

Arduino IDE (Required for Coms and AVRdude): https://www.arduino.cc/en/software/

ATMega328-P Documentation: https://www.microchip.com/en-us/product/atmega328#Documentation

***Note** - AVRdude permits the installation of the program onto the microcontroller (ATMega328-P)*

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## GETTING STARTED

<div style="text-align: justify">
The PEEKBot is an ongoing lunar rover design project developed by the CSA in collaboration
with student teams at various Canadian universities. Its purpose is to verify the feasibility of a
small lunar rover using primarily PEEK panels for its structure. It has been ongoing since 2018 and
the project is set to end in 2025. Our capstone project was pitched as testing and optimization of
the existing Antenna Pointing System design, designed previously by the 2023-2024 Concordia APS
capstone team. We were also provided with key requirements and objectives for improvement over
the existing design. The project then evolved into a complete mechanical, electrical, and software
redesign following analysis of the existing system.
</div>

### Prerequisites

<div style="text-align: justify">
The current program is configured to work for the hardware configuration seen below (<a href="#fig1">See Figure 1</a>).
The pinout is controlled via the "pinout.h" header file in the project, through which the programmer
can modify the setup provided it remains compatible with the current hardware (Pay close attention 
to the timers and interrupts pinouts).

***Note** - Allthough the hardware utilizes arduino, the program has been made to work regardless of the use of arduino affiliated software or hardware.*
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Building

<div style="text-align: justify">
It is assumed the peekbot-aps program is being built in Microsoft Windows. The program includese a "build.ps1"
file that references the avr8-gnu-toolchain compiler and the avr8-gnu-toolchain objectcpy executables.
The result of the compilation is a set of object files (*.o) under the "obj" folder, and a peekbot-aps.elf and
peekbot-aps.exe files under the "bin" folder.
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

<div style="text-align: justify">
It is assumed the user is in possession of a programmer that can be connected via USB or via serial port. 
Installation is achieved by running the "install.ps1" file. It references the avrdude executable and the
avrdude.conf files provided by the Arduino IDE data package. The user is expected to modify the "$COM" variable to match
the COM port number to which their arduino is connected.
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE -->
## USAGE

<div style="text-align: justify">
To cumunicate to the microcontroller, the TX and RX pins must be connected. The MCU uses UART at a baudr rate of 57600.

The commands take the following shape: **motor ; percentage ; angular-position**

- motor: [EL | AZ] - Where EL selects the Elevation motor and AZ selects the Azimuth motor.
- percentage: Integer value between 0 and 100 representing the percentage of the maximum speed of the motor.
- angular-position: The bitwise angular position between 0 bit (0 degrees) and 541 bits (95.097 degrees) for the
Elevation and -16611 bit (-185 degrees) and 16611 bits (185 degrees)

Example: To move the Elevation from 0 to 90 degrees at 50% speed

> EL;50;512
</div>

<!-- CONTRIBUTORS -->
## CONTRIBUTORS

Fabrizio Eduardo Rojas Iacocca 
- Concordia University Email: f_rojas@live.concordia.ca
- Personal Email: fabriziorojas2000@gmail.com
- [LinkedIn](https://www.linkedin.com/in/fabrizio-rojas-iacocca-a987a3239/)
- [GitHub](https://github.com/FabsRS/)

Joshua Desrosiers
- [LinkedIn](https://www.linkedin.com/in/joshua-desrosiers-bb983386/)
- [GitHub](https://github.com/YoshMonster)

Sunidhi Sharma
- [LinkedIn](https://www.linkedin.com/in/-sunidhi-sharma/)
- [GitHub](https://github.com/sunidhi-16)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LISENCE -->
## LISENCE

Distributed under the MIT License. See `LICENSE.txt` for more information.

>// MIT License\
>// \
>// Copyright (c) 2024 Fabrizio Eduardo Rojas Iacocca\
>// \
>// Permission is hereby granted, free of charge, to any person obtaining a copy\
>// of this software and associated documentation files (the "Software"), to deal\
>// in the Software without restriction, including without limitation the rights\
>// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\
>// copies of the Software, and to permit persons to whom the SWoftware is\
>// furnished to do so, subject to the following conditions:\
>// \
>// The above copyright notice and this permission notice shall be included in all\
>// copies or substantial portions of the Software.\
>// \
>// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\
>// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\
>// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\
>// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\
>// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\
>// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\
>// SOFTWARE.\

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LISENCE -->
## FIGURES

<a id="fig1"></a>

*Figure.1 - PEEKBot APS - Hardware Configuration*
![PEEKBot APS - Hardware Configuration](/peekbot-aps-Hardware-Configuration.png "PEEKBot APS - Hardware Configuration")

<a id="fig2"></a>

*Figure.2 - Main Function flowchart v0.2.2*
![Main Function flowchart v0.2.2](/peekbot-aps-v0.2.0-process-diagram.png "Main Function flowchart v0.2.2")

<p align="right">(<a href="#readme-top">back to top</a>)</p>