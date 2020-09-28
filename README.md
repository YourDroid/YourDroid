# YourDroid
## Descriprion
**YourDroid** is an open-source program that installs Android as a second OS through Windows or Linux (it neither replaces nor deletes the main OS). 
It isn't an emulator! **YourDroid** installs Android x86 and adds it to the bootloader of the main OS or installs Grub as the bootloader ([The official website for Android x86](http://www.android-x86.org/)).

Our Discord server: https://discord.gg/V9ayRXr

## Requirements
Operating system:
- **Windows 7-10**
- **Linux**

## Features
- You can choose where you want to install Android: 
	- To a flash drive, which allows you to run Android on any PC (saving all the data) or install it 
	- To a directory, which is the simplest way. You'll have a folder with all Android files on any drive you want just as a normal folder
	- To a separate partition formatted to ext3 (requires Ext2Fsd). This permits you to make /data a folder, which gives you access to all the files stored there in other OS
- You can update installed Android systems 
- You can install my modified version of Grub2 to make it able to be used with tablets (controlled with volume keys) 
- There is an option for replacing Windows bootmanager with Grub2 for tablets whose UEFI doesn't permit adding new OSs
- You can choose how much space you want to make available for your Android
- It works on your main OS
- Simple intuitive UI
- Easy to remove installed Androids
- It works on any PC (either BIOS or UEFI)
- It supports all the versions of Android x86 starting with 4.4. You can install Android image directly from **YourDroid**, otherwise you can download any on the [Android x86's website](http://www.android-x86.org/download)
- It automatically detects features of your PC, so you don't have to remember them
## Getting started
1. Download an ISO-image **(.ISO)** of any Android version you want [here](http://www.android-x86.org/download) or choose the version you want to install right in **YourDroid**
2. Download or clone this repository, unarchive it anywhere, then run *bin/yourdroid* **(Linux)** or *bin/yourdroid.exe* **(Windows)**
5. Fill in every form
6. Press the install button again
7. Wait a bit
8. ***Profit :)***
