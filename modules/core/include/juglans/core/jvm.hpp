#pragma once

#ifdef __ANDROID__

#include <memory>

#include <jni.h>

namespace juglans {

void SetJVM(JavaVM *jvm);
JavaVM *GetJVM();

jobject GetApplicationContext();
void ReleaseJObject(jobject obj);

}  // namespace juglans

#endif
