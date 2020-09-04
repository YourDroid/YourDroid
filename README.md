# YourDroid
## Descriprion
YourDroid is an open-source programm that installs Android to pc on windows or linux as a second OS (it doesn't replace\delete the main OS). It isn't an emulator! YourDroid installs Andoid x86 and adds it to the bootloader of the main OS or installs grub as the bootloader (here is the official web site of android x86: http://www.android-x86.org/).
Our discord server: https://discord.gg/V9ayRXr
## Requirements
It installs android to PCs with following features:
- Windows 7-10 
- (Linux coming soon)
## Features
- You can choose where you want to install android: 
	- To a flash drive, which allows you to run Android (saving all the data) on any PC or install it 
	- To a dir, which is the simplest way. You'll have a folder with all android files on any drive you likem just as a normal folder
	- To a separate partition formatted to ext3 (requires Ext2fsd). This permits you to make /data a folder, which gives you access to all the diles stored there from other OS
- You can update installed Androids 
- You can install my modified version of Grub2 so it can be used with tablets (controled with volume keys) 
- There is an option of replacing Windows bootmanager with Grub2 for tablets whose UEFI doesn't permit to add new OSs
- You can choose how much space you want to make available for your Android
- It works on your main os
- Simple UI
- Easy to remove installed Androids
- It works on any PC (both BIOS and UEFI)
- It supports any versions of Android x86 starting with 4.4. You can download one on the android x86 web site or, alternatively, you can choose the version you want to install in YourDroid and it'll install it.
- It automatically detects features of your pc, so you don't need to know them
## Getting started
1. Download an iso-image (.ISO!) of any android version you want [here](http://www.android-x86.org/download) or choose the version you want to install in YourDroid so it'll download the image
2. Dowload or clone this repo, unarchive it anywhere, then run bin/yourdroid or bin/yourdroid.exe if you're using is windows
5. Fill in every form
6. Press install-button again
7. Wait
8. Profit :) 
