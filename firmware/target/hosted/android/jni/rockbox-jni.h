#include <jni.h>

#ifndef _Included_org_rockbox_jni_RockboxNativeInterface
#define _Included_org_rockbox_jni_RockboxNativeInterface
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_org_rockbox_jni_RockboxNativeInterface_powerOff
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif

