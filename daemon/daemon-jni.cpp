#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <nativehelper/jni.h>
#include <android_runtime/AndroidRuntime.h>
#include "strtoargv.h"
#include "multithread.h"

#define NAME1(CLZ, FUN) CLZ##_##FUN
#define NAME2(CLZ, FUN) NAME1(CLZ, FUN)
#define FUNCTION(FUN) NAME2(PACKAGE_NAME, FUN)

extern "C"
{

#define STATIC 

using namespace android;

typedef struct Thread_Data
{
	int	   ThreadStarted;
	void*  Event;
	void*  Lock;
}Thread_State;

static JavaVM*	   	mjvm=NULL;

typedef struct trans_daemon
{
	char**			   	mArgv;
	int  				mArgc;
	void*  				ThreadHandle;
	Thread_State		ThreadState;
}trans_daemon;

static trans_daemon* mdaemon = NULL;
extern bool closing; 
extern int MAIN( int argc, char ** argv );

STATIC jint FUNCTION(Init)(JNIEnv *env, jobject thiz,jstring Cmd)
{
	jint ret = 0;
	jboolean iscopy;
	const char* szCmd;
	
	if(mdaemon != NULL)
	{
		return -1;
	}
		
	mdaemon = (trans_daemon*)malloc(sizeof(trans_daemon));
		
	memset(mdaemon,0,sizeof(struct trans_daemon));
		
	szCmd = env->GetStringUTFChars(Cmd, &iscopy);
	
	if(szCmd == NULL)
		return -2;
	
	if(mdaemon->mArgv != NULL)
		freeparsedargs(mdaemon->mArgv);
	
	mdaemon->mArgv = parsedargs((char*)szCmd,&(mdaemon->mArgc));
		
	env->ReleaseStringUTFChars(Cmd, szCmd);
	
	return ret;
}

STATIC jint FUNCTION(UnInit)(JNIEnv *env, jobject thiz)
{
	jint ret = 0;
	
	if(mdaemon == NULL)
	{
		return -1;
	}
	
	if(mdaemon->mArgv != NULL)
		freeparsedargs(mdaemon->mArgv);
	
	if(mdaemon != NULL)
		free(mdaemon);

	mdaemon = NULL;
	
	return ret;
}
static void *Run(void* This)
{
	if(mdaemon == NULL)
	{
		return NULL;
	}

	LockEnter(mdaemon->ThreadState.Lock);
	mdaemon->ThreadState.ThreadStarted = 1;
	EventSet(mdaemon->ThreadState.Event);
	LockLeave(mdaemon->ThreadState.Lock);
	
	MAIN(mdaemon->mArgc,mdaemon->mArgv);
}
STATIC jint FUNCTION(Start)(JNIEnv *env, jobject thiz)
{
	jint ret = 0;

	if(mdaemon == NULL)
	{
		return -1;
	}

	closing = 0;
	
	mdaemon->ThreadState.ThreadStarted = 0;
	mdaemon->ThreadState.Event=EventCreate(0,0);
	mdaemon->ThreadState.Lock = LockCreate();
	
	mdaemon->ThreadHandle = ThreadCreate(Run,NULL,25);
	
	LockEnter(mdaemon->ThreadState.Lock);
	while (!mdaemon->ThreadState.ThreadStarted) {
		EventWait(mdaemon->ThreadState.Event, -1,mdaemon->ThreadState.Lock);
	}
	LockLeave(mdaemon->ThreadState.Lock);
	
	EventClose(mdaemon->ThreadState.Event);
	LockDelete(mdaemon->ThreadState.Lock);
	mdaemon->ThreadState.Event= NULL;
	mdaemon->ThreadState.Lock = NULL;
	
	return ret;
}

STATIC void FUNCTION(Stop)(JNIEnv *env, jobject thiz)
{
	if(mdaemon == NULL)
		return;
	
	closing = 1;
	
	ThreadTerminate(mdaemon->ThreadHandle);
	mdaemon->ThreadHandle = NULL;
}

STATIC jboolean FUNCTION(IsRun)(JNIEnv *env, jobject thiz)
{
	if(mdaemon == NULL)
		return 0;
	
	if(mdaemon->ThreadHandle != NULL)
	{
		return 1;
	}
		
	return 0;
	
}
// ----------------------------------------------------------------------------
//the native method need to be registered	
static JNINativeMethod gMethods[] = {
    {"Init",           	"(Ljava/lang/String;)I",        (void *)FUNCTION(Init)},
    {"Start",           "()I",      					(void *)FUNCTION(Start)},
    {"IsRun",           "()Z",      					(void *)FUNCTION(IsRun)},
    {"Stop",            "()V",                        	(void *)FUNCTION(Stop)},
    {"UnInit",     		"()I",                        	(void *)FUNCTION(UnInit)}
};

// This function only registers the native methods
static int register_methods(JNIEnv *env)
{
	const char * pkg = PACKAGE_DIR;
	
    return AndroidRuntime::registerNativeMethods(env,
			pkg, gMethods, sizeof(gMethods)/sizeof(JNINativeMethod));
}
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    assert(env != NULL);

	env->GetJavaVM(&mjvm);

	if (register_methods(env) < 0) {
        goto bail;
    }
    /* success -- return valid version number */
    result = JNI_VERSION_1_4;

bail:
    return result;
}
}