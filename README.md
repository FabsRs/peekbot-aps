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
        <li><a href="#philosophy">Philosophy</a></li>
        <li><a href="#versioning">Versioning</a></li>
        <li><a href="#syntax">Syntax</a></li>
        <li><a href="#resources">Resourcesx</a></li>
      </ul>
      <a href="#introduction">INTRODUCTION</a>
      <ul>
        <li><a href="#project-overview">Project Overview</a></li>
        <li><a href="#requirements">Requirements</a></li>
        <li><a href="#history">History</a></li>
        <li><a href="#debugging">Debugging</a></li>
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
    <li>
        <a href="#usage">USAGE</a></li>
        <ul>
            <li><a href="#data-packet">Data Packet</a></li>
            <li><a href="#installation">Commands</a></li>
            <li><a href="#installation">Telemetry</a></li>
        </ul>
    <li><a href="#contributing">CONTRIBUTORS</a></li>
    <li><a href="#license">LICENSE</a></li>
  </ol>
</details>

<!-- CODING STANDARD -->
## CODING STANDARD

### Philosophy

The Power o 10: Rules for Developing Safety-Critical Code [1]

    Avoid complex flow constructs, such as goto and recursion.
    All loops must have fixed bounds. This prevents runaway code.
    Avoid heap memory allocation.
    Restrict functions to a single printed page.
    Use a minimum of two runtime assertions per function.
    Restrict the scope of data to the smallest possible.
    Check the return value of all non-void functions, or cast to void to indicate the return value is useless.
    Use the preprocessor sparingly.
    Limit pointer use to a single dereference, and do not use function pointers.
    Compile with all possible warnings active; all warnings should then be addressed before release of the software.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Versoining
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Syntax
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Resources
<p align="right">(<a href="#readme-top">back to top</a>)</p>
https://tinusaur.com/guides/avr-gcc-toolchain/

<!-- INTRODUCTION -->
## INTRODUCTION

TODO: Add descrition of section

### Project Overview
TODO: Add project description

TODO: Focus on software intent. Why is the software being developed? What is the rationale behind  the selection of the current MCU, etc (scoped Context)
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Requirements
TODO: Focus on software requirements. enumerate requirements here

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### History
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Debugging
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## GETTING STARTED

TODO: Add descrition of section

### Prerequisites

TODO: Mention software requirements to be able to build the project. Link to websites if necessary. Add resources (Literature) if necessary. Mention Hardware requirements. WMention current fuse arrangement and why it's the best arrangement possible? How is the chip itself setup (Fuses, brownout timer, clock settings, etc).
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Building

TODO: Mention avr-gcc, decsribe what it is, whi it's being used
<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

TODO: Mention different installation methods as per ATMEGA328P (SPI, Parallel, Serial), why was serial selected? (Because it is cheaper, and readly available, and we have enough pins available that we do not need to do parallel programming).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE -->
## USAGE

TODO: Add descrition of section

### Data Packet
<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Commands
<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Telemetry
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTORS -->
## CONTRIBUTORS

Fabrizio Eduardo Rojas Iacocca 
- Concordia University Email: f_rojas@live.concordia.ca
- Personal Email: fabriziorojas2000@gmail.com
- [LinkedIn](https://www.linkedin.com/in/fabrizio-rojas-iacocca-a987a3239/)
- [GitHub](https://github.com/FabsRS/)
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LISENCE -->
## LISENCE

Distributed under the MIT License. See `LICENSE.txt` for more information.
<p align="right">(<a href="#readme-top">back to top</a>)</p>
