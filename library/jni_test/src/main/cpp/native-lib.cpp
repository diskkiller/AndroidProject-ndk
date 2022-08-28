#include <jni.h>
#include <string>
#include <iostream>

/**
 * 1. 基本类型签名：

Java	                Native	              Signature

byte	                jbyte	              B
char	                jchar	              C
double	                jdouble	              D
float	                jfloat	              F
int	                    jint	              I
short	                jshort	              S
long	                jlong	              J
boolean	                jboolean	          Z
void	                void	              V

 2. 引用数据类型的转换

Java	              Native	                 Signature

所有对象	              jobject        	         L+classname +;
Class	              jclass	                 Ljava/lang/Class;
String     	          jstring	                 Ljava/lang/String;
Throwable	          jthrowable	             Ljava/lang/Throwable;
Object[] 	          jobjectArray	             [L+classname +;
byte[]     	          jbyteArray     	         [B
char[]	              jcharArray	             [C
double[]	          jdoubleArray	             [D
float[]	              jfloatArray	             [F
int[]	              jintArray      	         [I
short[]  	          jshortArray	             [S
long[]	              jlongArray	             [J
boolean[]	          jbooleanArray	             [Z

如下Java 方法:
long f (int n, String s, int[] arr);
signature: "(ILjava/lang/String;[I)J"



编码过程中，对象的签名2中方法都是可以行的。
例如： java 中 String
          1.直接签名：java/lang/String
          2.标准签名：Ljava/lang/String;


 */

//日志输出
#include <android/log.h>

#define TAG "DISKKILLER"
//__VA_ARGS__ 代表 ... 的可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__);
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__);

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_jni_1test_MainActivity_1jni_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jni_1test_MainActivity_1jni_arrayFromJNI(JNIEnv *env, jobject thiz, jint count,
                                                          jstring text_info, jintArray ints,
                                                          jobjectArray strs) {
    int countint = count;
    LOGI("参数一 countint: %d",countint);

    const char * textinfo = env->GetStringUTFChars(text_info,NULL);
    LOGI("参数二 textinfo: %s",textinfo);

    int * jintarry = env->GetIntArrayElements(ints,NULL);

    int size = env->GetArrayLength(ints);

    for (int i = 0; i < size; ++i) {
        *(jintarry+i)+=100;//无法改变java层数据
        LOGI("参数三 ints: %d",*jintarry+i);
    }

    /**
     * JNIEnv：相当于中间人，通过env可以使Java层和c++层进行相互通信
     *
     * 0:刷新Java数组，并释放c++层数组（常用）
     * JNI_COMMIT:刷新Java数组，不释放c++层数组
     */
    env->ReleaseIntArrayElements(ints,jintarry,0);

    jsize strsize = env->GetArrayLength(strs);
    for (int i = 0; i < strsize; ++i) {
        jstring str = static_cast<jstring>(env->GetObjectArrayElement(strs, i));//JNI层
        const char * strinfo = env->GetStringUTFChars(str,NULL);//c++层
        LOGI("参数四 strs: %s",strinfo);
        //释放
        env->ReleaseStringUTFChars(str,strinfo);
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jni_1test_MainActivity_1jni_putObjectFromJNI(JNIEnv *env, jobject thiz,
                                                              jobject student, jstring info) {

    const char * strinfo = env->GetStringUTFChars(info,NULL);//c++层
    LOGI("对象测试 参数 strinfo: %s",strinfo);
    //如果后面不用，记得用完释放 节省native层内存
    env->ReleaseStringUTFChars(info,strinfo);

    //1.找到对象class
    // jclass studentClass = env->FindClass("com/example/jni_test/Student");//第一种 通过包名
    jclass studentClass = env->GetObjectClass(student);//第二种 通过对象

    //2.Student内部函数签名
    jmethodID setName = env->GetMethodID(studentClass,"setName","(Ljava/lang/String;)V");
    jmethodID getName = env->GetMethodID(studentClass,"getName","()Ljava/lang/String;");
    jmethodID showInfo = env->GetStaticMethodID(studentClass,"showInfo","(Ljava/lang/String;)V");

    //3.调用
    //CallVoidMethod调用Java层的方法参数为String,JNI层必须传入jstring
    jstring value = env->NewStringUTF("diskkiller");
    env->CallVoidMethod(student,setName,value);

    jstring name = static_cast<jstring>(env->CallObjectMethod(student, getName));
    /**
     * 这个调用时c++层的，必须将 jstring的name 转换城指针
     */
     const char * _name = env->GetStringUTFChars(name,NULL);
    LOGI("调用getName: %s",_name);

    //CallStaticVoidMethod调用Java层的方法参数为String,JNI层必须传入jstring
    jstring staticvalue = env->NewStringUTF("diskkiller show info");
    env->CallStaticVoidMethod(studentClass,showInfo,staticvalue);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jni_1test_MainActivity_1jni_creatObjectFromJNI(JNIEnv *env, jobject thiz) {

    //1.通过包名+类名的方式
    jclass studentClass = env->FindClass("com/example/jni_test/Student");

    //2.实例化对象
    jobject studentOblect = env->AllocObject(studentClass);//只实例化对象，不会调用对象的构造函数

    //3.Student内部函数签名
    jmethodID setAge = env->GetMethodID(studentClass,"setAge", "(I)V");
    jmethodID setName = env->GetMethodID(studentClass,"setName","(Ljava/lang/String;)V");

    //4.调用
    //CallVoidMethod调用Java层的方法参数为String,JNI层必须传入jstring
    jstring value = env->NewStringUTF("朱棣");
    env->CallVoidMethod(studentOblect,setName,value);

    env->CallVoidMethod(studentOblect,setAge,18);

}