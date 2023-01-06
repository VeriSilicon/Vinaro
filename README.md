
![Vinaro](doc/images/Vinaro.png)

# Vinaro Software Development Kit

The Vinaro Software Development Kit (Vinaro SDK) is developed for developing and debugging software based on risc-v architecture platform.

Vinaro SDK is built on Vinaro's own HAL (Hardware Abstraction Layer) framework. Users can use the HAL API provided by Vinaro to access onboard peripherals, such as GPIO, UART, I2C, SPI and SENSORS.

Vinaro SDK provides the OSAL (Operating System Abstraction Layer) API. Users can use these interfaces to complete unified operating system operations.

## Directory Structure

The following is the directory structure of Vinaro SDK

```
$VINARO_SDK_ROOT
|———— doc
|   |—— images
|———— hal
|   |—— include
|———— osal
|   |—— os_adapter
|———— README.md
|———— RELEASE.md
```

* **doc**

  This directory contains the details of the Vinaro SDK API and some resource files.

* **hal**

  This directory contains the hal header and source files, we can use the hal api to simply access the onboard peripherals.


* **osal**

  This directory contains the osal header and source files, we can use the osal api to complete unified operating system operations.

## API Reference

The API Reference Documentation explains in more detail every HAL and OSAL API functions.
It is located in the "doc" subdirectory.

## Licence

Copyright (C) 2021-2022 VeriSilicon Holdings Co., Ltd.
All rights reserved.
