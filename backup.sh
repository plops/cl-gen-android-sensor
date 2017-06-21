SDK="${HOME}/and/android-sdk-linux"
sudo "${SDK}/platform-tools/adb" root
sudo "${SDK}/platform-tools/adb" backup -all -apk -shared -f /mnt/android/backup.ab
 
