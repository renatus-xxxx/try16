zcc +msx -create-app -O3 -subtype=msxdos -lm -DMSX main.c -bn TRY16.COM
del TRY16.img
move /y TRY16.COM bin
cd bin
START /WAIT DISKMGR.exe -A -F -C TRY16.DSK TRY16.COM
cd ../
