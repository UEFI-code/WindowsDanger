# Debugging Windows Kernel On Azure (x64)

### Introduction

As we know, most popular Virtual Machine backend on Azure is Hyper-V (You can even use ```dmesg``` command in Linux VM to see the Host Version!)

On other hand, newer Windows Kernel supported Network Debugging, and Support Hyper-V NIC card. DHCP is needed to allocate the target machine an IP address.

You know, Azure's Virtual Network has ***shipped with DHCP and NAT support***. That means, you almost do not need do anything around DHCP and NAT settings!

### Go

0. Create 2 ***Windows Server 2022 with Desktop Experience*** VMs. name it as ***Host*** and ***Target***. ***Host*** is suggest as minimal 2 Cores with 8GB RAM. ***Target*** is 2 Cores 4GB RAM. You can use ***Azure Spot*** if you are running on ***Pay-As-You-Go*** Subscription which can be more cheap. Same Login token on two VM is suggested for SMB easy use.

1. Configure the ***Target*** VM to Disable TPM, Secure Boot, and Enable Boot Diag (To see the Virtual Screen and Access Serial Port). Configure the ***Target*** VM's Firewall to Allow ***TCP 445, 135 ports*** inbound trafic to Enable SMB access

2. Follow the Microsoft Learn Guide to [Install Visual Studio 2022 with Windows Driver Kits](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk?wt.mc_id=studentamb_188195) on ***Host*** VM. 

3. Follow the Microsoft Learn Guide to [Auto Setup Network Kernel Debugging](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-a-network-debugging-connection-automatically?wt.mc_id=studentamb_188195). Note the Network Kernel Debugging protocol is based on ***UDP Reverse Connect Session***, if you get trouble you should configure Firewall on ***Host*** VM to allow the inbound connection.

4. If the ***Target*** successfully connect to ***Host*** at ***Boot Time***, the ***PatchGuard*** might be partial Disabled, that means you can do more Experiment!

5. You can download the New ***WinDbg Preview*** which having good GUI experience on ***Host*** with Following Steps:
    - ```curl https://aka.ms/windbg/download -o windbgp.xml```
    - Read the windbgp.xml, and found ```MainBundle.Uri``` inside it. Download it as ```windbgp.zip```
    - Unzip the ```windbgp.zip``` file, found ```windbg_win7-x64.msix``` inside it. Then unzip this ```msix``` Again
    - Double click ```DbgX.Shell.exe``` to run!

6. Go ahead to explorer how to use New ***WinDbg Preview*** in Kernel Debug ***Yourself***