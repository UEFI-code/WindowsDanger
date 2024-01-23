# Dev Notice

If you are trying to Debug Windows Kernel Under Hyper-V, Please notice the following things.

## Virtual Network LAN Switcher

If you created an external virtual switch which points to your physical network adapter, and you used it as the network adapter of your virtual machine, you may encounter the following problems:

- The VM might not able to connect back to the host machine for Kernel Debugging.
- The Host machine might be CRASH or Extremely Slow when your VM booting up with Kernel Debugging Mode.

So, it is better to create an internal virtual switch and use it as the network adapter of your virtual machine.

## DHCP Server

It is necessary to have a DHCP server in your network, because the VM Kernel will try to get an IP address from DHCP server when it booting up with Kernel Debugging Mode. Or your VM will Extremly Slow when booting up and might not able to connect back to the host machine for Kernel Debugging.

## Firewall

Please notice that Kernel Debugging using UDP protocol!

## NAT

Creating NAT on your host machine is not necessary, but if you want connect your VM to the Internet you can do it.