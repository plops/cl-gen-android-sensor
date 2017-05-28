#include <stdlib.h>
#include <jni.h>

static const char *const messages[] = {
        "Hello, world!",
        "Hej världen!",
        "Bonjour, monde!",
        "Hallo Welt!"
};

JNIEXPORT jstring JNICALL
Java_net_hanshq_hello_MainActivity_getMessage(JNIEnv *env, jobject obj)
{
        int i;

        i = rand() % (sizeof(messages) / sizeof(messages[0]));

        return (*env)->NewStringUTF(env, messages[i]);
}
