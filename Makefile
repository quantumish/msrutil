load:
	rm -rf ./build/Debug/msrutil.kext
	rm -rf /private/tmp/msrutil.kext
	xcodebuild -configuration Debug
	cp -R ./build/Debug/msrutil.kext /private/tmp
	kextload /private/tmp/msrutil.kext

unload:
	kextunload /private/tmp/msrutil.kext
