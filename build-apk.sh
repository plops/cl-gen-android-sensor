export JAVA_HOME=${HOME}/and/jdk1.8.0_131
export PATH=${JAVA_HOME}/bin:/home/martin/and/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/arm-linux-androideabi/bin/:$PATH
NDK="${HOME}/and/android-ndk-r14b"
ARM_TOOLCHAIN="${NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/"
#X86_TOOLCHAIN="${NDK}/toolchains/x86-4.9/prebuilt/"
#X86_TOOLCHAIN+="linux-x86_64/bin/i686-linux-android-g++"
#AMD64_TOOLCHAIN="${NDK}/toolchains/x86_64-4.9/prebuilt/"
#AMD64_TOOLCHAIN+="linux-x86_64/bin/x86_64-linux-android-g++"
SDK="${HOME}/and/android-sdk-linux"
BUILD_TOOLS="${SDK}/build-tools/26_rc2/"
PLATFORM="${SDK}/platforms/android-5.1.1/"
 
#PLATFORM="/home/martin/Android/Sdk/platforms/android-18/"
#NDK="/home/martin/Android/Sdk/ndk-bundle/"
#ARM_TOOLCHAIN="${NDK}/toolchains/arm-linux-androideabi-4.9/prebuilt/"
mkdir -p build/apk/lib/armeabi-v7a build/apk/lib/x86 build/apk/lib/x86_64

#LDFLAGS="-L/home/martin/and/android-ndk-r14b/platforms/android-23/arch-arm/usr/lib/ -landroid -llog"
LDFLAGS="-landroid -llog"
CFLAGS="-std=c++11 -g"
#"${ARM_TOOLCHAIN}"
#################################################################################################################################
# /home/martin/and/android-ndk-r14b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++  \ #
#     --sysroot="${NDK}/platforms/android-23/arch-arm" -std=c++11 -g \							        #
#     -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -Wl,--fix-cortex-a8 \						        #
#     -no-canonical-prefixes \												        #
#     -fPIC -shared -o build/apk/lib/armeabi-v7a/libhello.so jni/hello.cpp jni/android_native_app_glue.c -landroid -llog        #
#################################################################################################################################

# g++
"${ARM_TOOLCHAIN}"/linux-x86_64/bin/arm-linux-androideabi-g++  \
    --sysroot="${NDK}/platforms/android-18/arch-arm" -std=c++11 -Os \
    -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -Wl,--fix-cortex-a8 \
    -no-canonical-prefixes \
    -fPIC -shared -o build/apk/lib/armeabi-v7a/libhello.so jni/hello.cpp jni/android_native_app_glue.c \
    -landroid -llog \
    -I/home/martin/and/android-ndk-r14b/sources/cxx-stl/llvm-libc++/include \
    -I/home/martin/and/android-ndk-r14b/platforms/android-23/arch-arm/usr/include
#    -I/home/martin/and/android-ndk-r14b/sources/cxx-stl/stlport/stlport
"${ARM_TOOLCHAIN}"/linux-x86_64/bin/arm-linux-androideabi-strip -s build/apk/lib/armeabi-v7a/libhello.so

#-I/home/martin/Android/Sdk/ndk-bundle/sources/cxx-stl/gnu-libstdc++/4.9/include/

# CLANG++ does not support arm

# /home/martin/and/android-ndk-r14b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++  \
#     --sysroot="${NDK}/platforms/android-18/arch-arm" -std=c++11 -g \
#     -march=armv7-a -mfpu=vfpv3-d16 -mfloat-abi=softfp -Wl,--fix-cortex-a8 \
#     -no-canonical-prefixes \
#     -fPIC -shared -o build/apk/lib/armeabi-v7a/libhello.so jni/hello.cpp jni/android_native_app_glue.c \
#     -landroid -llog





 
#"${X86_TOOLCHAIN}" "${CFLAGS}"  --sysroot="${NDK}/platforms/android-23/arch-x86" -fPIC -shared -o build/apk/lib/x86/libhello.so jni/hello.cpp "${LDFLAGS}"
#"${AMD64_TOOLCHAIN}" ${CFLAGS}  --sysroot="${NDK}/platforms/android-23/arch-x86_64" -fPIC -shared -o build/apk/lib/x86_64/libhello.so jni/hello.cpp ${LDFLAGS} 

mkdir -p build/gen build/obj build/apk
"${BUILD_TOOLS}/aapt" package -f -m -J build/gen/ -S res       -M AndroidManifest.xml -I "${PLATFORM}/android.jar"
#javac -source 1.7 -target 1.7 -bootclasspath "${JAVA_HOME}/jre/lib/rt.jar"       -classpath "${PLATFORM}/android.jar" -d build/obj       build/gen/net/hanshq/hello/R.java java/net/hanshq/hello/MainActivity.java
#"${BUILD_TOOLS}/dx" --dex --output=build/apk/classes.dex build/obj/
"${BUILD_TOOLS}/aapt" package -f -M AndroidManifest.xml -S res/       -I "${PLATFORM}/android.jar"       -F build/Hello.unsigned.apk build/apk/
"${BUILD_TOOLS}/zipalign" -f -p 4       build/Hello.unsigned.apk build/Hello.aligned.apk
keytool -genkeypair -keystore keystore.jks -alias androidkey       -validity 10000 -keyalg RSA -keysize 2048       -storepass android -keypass android
../android-sdk-linux/build-tools/25.0.0/apksigner sign --ks keystore.jks       --ks-key-alias androidkey --ks-pass pass:android       --key-pass pass:android --out build/Hello.apk       build/Hello.aligned.apk

