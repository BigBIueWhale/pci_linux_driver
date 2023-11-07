#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>

// These are the real values:
#define MY_VENDOR_ID 0x1363 // Replace with your vendor ID
#define MY_DEVICE_ID 0x7 // Replace with your device ID

// These are the fake values:
//#define MY_VENDOR_ID 0x8086
//#define MY_DEVICE_ID 0x2a10

// This structure holds information about the PCIe device
struct pcie_dev_info {
    struct pci_dev *pdev;
    void __iomem *bar0; // Base address register 0
    dma_addr_t dma_handle; // DMA handle
    void *dma_buffer; // DMA buffer
};

// This function is called when the PCIe device is found
static int pcie_probe(struct pci_dev *pdev, const struct pci_device_id *ent) {
    struct pcie_dev_info *info;
    int bar = 0; // BAR 0

    // Allocate memory for our device info
    info = kzalloc(sizeof(*info), GFP_KERNEL);
    if (!info) {
        return -ENOMEM;
    }
    
    // Save the pci device structure
    info->pdev = pdev;

    // Enable the device
    if (pci_enable_device(pdev)) {
        printk(KERN_ERR "Failed to enable PCI device\n");
        kfree(info);
        return -EIO;
    }

    // Request regions and I/O memory
    if (pci_request_region(pdev, bar, "my_pci_driver")) {
        printk(KERN_ERR "Failed to request BAR0\n");
        pci_disable_device(pdev);
        kfree(info);
        return -EIO;
    }

    // Get the virtual address for BAR 0
    info->bar0 = pci_iomap(pdev, bar, pci_resource_len(pdev, bar));
    if (!info->bar0) {
        printk(KERN_ERR "Failed to map BAR0\n");
        pci_release_region(pdev, bar);
        pci_disable_device(pdev);
        kfree(info);
        return -EIO;
    }
    // Allocate a DMA buffer
    info->dma_buffer = dma_alloc_coherent(&pdev->dev, PAGE_SIZE, &info->dma_handle, GFP_KERNEL);
    if (!info->dma_buffer) {
        printk(KERN_ERR "Failed to allocate DMA buffer\n");
        pci_iounmap(pdev, info->bar0);
        pci_release_region(pdev, bar);
        pci_disable_device(pdev);
        kfree(info);
        return -ENOMEM;
    }

    // Print the physical address of the DMA transfer
    printk(KERN_INFO "DMA handle (physical address) is %pad\n", &info->dma_handle);

    // Set up the DMA here with the handle.
    // info->dma_handle is what we need to write
    // to the DMA register that Daniel's FPGA expects.
    // THAT IS THE PHYSICAL ADDRESS!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // The offset from the beginning of the BAR0
    #define WRITE_OFFSET 0x1000
    // The value to write
    //#define WRITE_VALUE 0x11111111 // For turn on
    #define WRITE_VALUE 0x0 // For turn off

    // After successful DMA buffer allocation, write to the device registers
    // of Daniel's FPGA, at BAR0
    // (Consider doing this in another function if it's not part of the initial setup.)

    // Check if the mapped BAR0 range is sufficient
    if (pci_resource_len(pdev, bar) < (WRITE_OFFSET + sizeof(u32))) {
        printk(KERN_ERR "BAR0 range too small\n");
        // Clean up as before
    } else {
        // Perform the write operation
        iowrite32(WRITE_VALUE, info->bar0 + WRITE_OFFSET);
        printk(KERN_INFO "Wrote 0x%x to BAR0 + 0x%x\n", WRITE_VALUE, WRITE_OFFSET);
    }

    // Save our info structure to the device's driver_data
    pci_set_drvdata(pdev, info);

    printk(KERN_INFO "PCIe device initialized\n");

    return 0;
}

// This function is called when the device is removed or the driver is unloaded
static void pcie_remove(struct pci_dev *pdev) {
    struct pcie_dev_info *info = pci_get_drvdata(pdev);

    // Clean up our DMA buffer
    dma_free_coherent(&pdev->dev, PAGE_SIZE, info->dma_buffer, info->dma_handle);

    // Release the I/O memory
    pci_iounmap(pdev, info->bar0);
    pci_release_region(pdev, 0);

    // Disable the device
    pci_disable_device(pdev);

    // Free our device info structure
    kfree(info);

    printk(KERN_INFO "PCIe device removed\n");
}

// PCI device ID table
static const struct pci_device_id id_table[] = {
    { PCI_DEVICE(MY_VENDOR_ID, MY_DEVICE_ID) },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, id_table);

// PCI driver structure
static struct pci_driver pcie_driver = {
    .name = "my_pcie_driver",
    .id_table = id_table,
    .probe = pcie_probe,
    .remove = pcie_remove,
};

// Initialize the PCIe driver
static int __init pcie_init(void) {
    printk(KERN_INFO "PCIe driver loaded\n");  // Print a message when the driver is loaded
    return pci_register_driver(&pcie_driver);
}

// Exit the PCIe driver
static void __exit pcie_exit(void) {
    pci_unregister_driver(&pcie_driver);
    printk(KERN_INFO "PCIe driver unloaded\n");  // Print a message when the driver is unloaded
}

module_init(pcie_init);
module_exit(pcie_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("PCIe DMA Example Driver");
MODULE_VERSION("0.1");
