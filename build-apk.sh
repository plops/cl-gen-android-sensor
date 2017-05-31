export JAVA_HOME=${HOME}/and/jdk1.8.0_131
export PATH=${JAVA_HOME}/bin:$PATH
NDK="${HOME}/and/android-ndk-r14b"
ARM_TOOLCHAIN="${NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/"
ARM_TOOLCHAIN+="linux-x86_64/bin/arm-linux-androideabi-g++"
X86_TOOLCHAIN="${NDK}/toolchains/x86-4.9/prebuilt/"
X86_TOOLCHAIN+="linux-x86_64/bin/i686-linux-android-gcc"
SDK="${HOME}/and/android-sdk-linux"
BUILD_TOOLS="${SDK}/build-tools/26_rc2/"
PLATFORM="${SDK}/platforms/android-5.1.1/"

mkdir -p build/apk/lib/armeabi-v7a build/apk/lib/x86

LDFLAGS="-L/home/martin/and/android-ndk-r14b/platforms/android-16/arch-arm/usr/lib/ -landroid -llog"

"${ARM_TOOLCHAIN}" --sysroot="${NDK}/platforms/android-16/arch-arm" \
      -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -Wl,--fix-cortex-a8 \
      -fPIC -shared -o build/apk/lib/armeabi-v7a/libhello.so jni/hello.cpp  -std=c++11 "${LDFLAGS}"
 
#"${X86_TOOLCHAIN}" --sysroot="${NDK}/platforms/android-16/arch-x86" \
#      -fPIC -shared -o build/apk/lib/x86/libhello.so jni/hello.cpp "${LDFLAGS}" 

mkdir -p build/gen build/obj build/apk
"${BUILD_TOOLS}/aapt" package -f -m -J build/gen/ -S res       -M AndroidManifest.xml -I "${PLATFORM}/android.jar"
#javac -source 1.7 -target 1.7 -bootclasspath "${JAVA_HOME}/jre/lib/rt.jar"       -classpath "${PLATFORM}/android.jar" -d build/obj       build/gen/net/hanshq/hello/R.java java/net/hanshq/hello/MainActivity.java
#"${BUILD_TOOLS}/dx" --dex --output=build/apk/classes.dex build/obj/
"${BUILD_TOOLS}/aapt" package -f -M AndroidManifest.xml -S res/       -I "${PLATFORM}/android.jar"       -F build/Hello.unsigned.apk build/apk/
"${BUILD_TOOLS}/zipalign" -f -p 4       build/Hello.unsigned.apk build/Hello.aligned.apk
keytool -genkeypair -keystore keystore.jks -alias androidkey       -validity 10000 -keyalg RSA -keysize 2048       -storepass android -keypass android
../android-sdk-linux/build-tools/25.0.0/apksigner sign --ks keystore.jks       --ks-key-alias androidkey --ks-pass pass:android       --key-pass pass:android --out build/Hello.apk       build/Hello.aligned.apk

