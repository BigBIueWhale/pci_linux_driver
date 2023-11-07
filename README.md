# PCIe DMA Example Driver

This repository contains a Linux kernel module for PCIe devices that demonstrates the setup of a PCIe driver with DMA (Direct Memory Access) capabilities. It is designed to be a starting point for developing PCIe drivers that utilize DMA for high-speed data transfer between the host and a PCIe device, typically an FPGA or similar hardware.

## Requirements

This driver has been tested and is confirmed to run on Ubuntu 22.04 64-bit.

## Structure

Here is a brief overview of the project files:

- `hello.c`: The main source file for the PCIe DMA driver.
- `Makefile`: Contains commands to compile the kernel module.
- `compile.sh`: A script to run the compilation process using the Makefile.
- `clean.sh`: A script to clean up the build environment.
- `load_module.sh`: A script to insert the kernel module into the running kernel.
- `unload_module.sh`: A script to remove the kernel module from the running kernel.
- `view_kernel_log.sh`: A script to view the kernel log messages related to the module.
- `print_all_files.sh`: A script to print out the contents of all the project files.

## Installation

Before proceeding, ensure that you have the necessary packages for building kernel modules:

```sh
sudo apt-get install build-essential linux-headers-$(uname -r)
```

To compile and run the driver on your system, follow these steps:

1. **Compile the Kernel Module**:

```bash
./compile.sh
```

This will use the `Makefile` to compile the kernel module.

2. **Load the Kernel Module**:

```bash
./load_module.sh
```

This will insert the compiled kernel module into the kernel.

3. **Unload the Kernel Module**:

When you are done with the kernel module, you can remove it from the kernel:

```bash
./unload_module.sh
```

4. **View Kernel Log**:

To see the kernel log messages related to this module:

```bash
./view_kernel_log.sh
```

5. **Clean the Build Environment**:

To clean up the build environment after you're done:

```bash
./clean.sh
```

## Usage

Replace `MY_VENDOR_ID` and `MY_DEVICE_ID` in `hello.c` with your specific PCIe device's vendor and device IDs. Once the module is loaded, it will automatically set up a DMA buffer and print the physical address to the kernel log.

## Scripts

- **compile.sh**: Runs the make command to compile the kernel module.
- **clean.sh**: Cleans up the build environment by removing generated files.
- **load_module.sh**: Inserts the module into the kernel.
- **unload_module.sh**: Removes the module from the kernel.
- **print_all_files.sh**: Displays the content of all the project files for quick inspection.
- **view_kernel_log.sh**: Displays the last few kernel log entries, which is useful for debugging.

## Contribution

Contributions to this project are welcome. To contribute:

1. Fork it (https://github.com/yourname/yourproject/fork)
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

## Author

Ronen Zyroff

---

Please make sure to update tests as appropriate and ensure that your system environment matches the prerequisites mentioned.

Enjoy coding and testing your PCIe DMA enabled devices!