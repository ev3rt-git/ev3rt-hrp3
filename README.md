# ev3rt-hrp3
RTOS for Mindstorms EV3 w/ TOPPERS/HRP3 Kernel

# Usage

```bash
git clone https://github.com/ev3rt-git/ev3rt-hrp3
cd ev3rt-hrp3
git submodule init
git submodule update

# Build loader uImage
cd ev3rt-hrp3/sdk/firmware
make img=loader

# Build helloev3 app
cd ev3rt-hrp3/sdk/workspace
make app=helloev3
```

# Build status

Status               | Platform
--------------       | ------ 
[![Build Status](https://travis-ci.org/ev3rt-git/ev3rt-hrp3.svg?branch=master)](https://travis-ci.org/ev3rt-git/ev3rt-hrp3) | Ubuntu 14.04
