# msrutil

## About
`msrutil` is a work-in-progress MacOS kernel extension (or KEXT) designed to access kernel-specific data structures anad other information that is privy to solely the kernel.

This was designed to help with another project focusing on displaying low-level computer stats and information.

## Usage
# Setup
0. Are you on a MacOS version greater than 10.14? I'm sorry to say that this project is not for you. With the Mojave update, Apple removed partial disabling of SIP (i.e. only allowing KEXTs but leaving all the other security features intact), already making this process more difficult. [However Catalina has begun phasing out KEXTs entirely.](https://www.zdnet.com/article/apple-deprecating-macos-kernel-extensions-kexts-is-a-great-win-for-security/)
1. Reboot your computer and hold CMD-R while it restarts to boot into Recovery Mode.
2. While in Recovery Mode, navigate to the terminal through the top menu.
3. Run `csrutil enable --without kext` to allow unsigned KEXTs to run on your computer. If you are on 10.14, you will have to fully disable SIP by running `csrutil disable`.
4. Reboot your computer oncemore.
# Loading
1. Run `xcodebuild --configuration Release` to build the KEXT.
2. Navigate to the KEXT executable.
3. Run `sudo cp -R msrutil.kext /private/tmp && cd /private/tmp` to create a copy of the KEXT executable that you own, allowing you to load it.
4. Run `sudo kextload msrutil.kext` to load the KEXT.
5. Check the system logs (either through Console.app or a UNIX command) and if you see the message "msrutil successfully loaded", you've done it!
   - You can double-check that the KEXT is running with `kextstat`.
6. Unload the kext with `sudo kextunload msrutil.kext` and once again check the system logs for the message "msrutil successfully unloaded".
# Calling
As of now communicating with the kernel extension does not work. 

