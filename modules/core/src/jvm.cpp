#ifdef __ANDROID__

#include "juglans/core/jvm.hpp"

namespace juglans {

static JavaVM *gJVM = nullptr;

void SetJVM(JavaVM *jvm) { gJVM = jvm; }

JavaVM *GetJVM() { return gJVM; }

struct EnvGuard {
 public:
  EnvGuard() {
    gJVM->GetEnv((void **)&env, JNI_VERSION_1_6);
    attach = env == nullptr;
    if (attach) gJVM->AttachCurrentThread(&env, nullptr);
  }

  ~EnvGuard() {
    if (attach) gJVM->DetachCurrentThread();
  }

  JNIEnv *Env() { return env; }

 private:
  bool attach;
  JNIEnv *env = nullptr;
};

jobject GetApplicationContext() {
  if (gJVM == nullptr) return nullptr;
  EnvGuard guard;
  JNIEnv *env = guard.Env();

  jclass CPlayer = env->FindClass("com/unity3d/player/UnityPlayer");
  jfieldID FActivity = env->GetStaticFieldID(CPlayer, "currentActivity", "Landroid/app/Activity;");
  jclass CActivity = env->FindClass("android/app/Activity");
  jmethodID FContext =
    env->GetMethodID(CActivity, "getApplicationContext", "()Landroid/content/Context;");

  jobject activity = env->GetStaticObjectField(CPlayer, FActivity);
  jobject context = env->CallObjectMethod(activity, FContext);
  return env->NewGlobalRef(context);
}

void ReleaseJObject(jobject obj) {
  if (gJVM == nullptr || obj == nullptr) return;
  EnvGuard guard;
  guard.Env()->DeleteGlobalRef(obj);
}

}  // namespace juglans

#endif
