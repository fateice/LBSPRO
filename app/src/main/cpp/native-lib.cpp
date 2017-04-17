#include <jni.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>



extern "C"
jstring
Java_com_example_kimi_lbspro_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_GridDummy(JNIEnv *env, jobject instance,
                                                    jdouble mLongitude, jdouble mLatitude, jint k,
                                                    jdouble s) {
    // TODO
    int c = 0;

    //经度
    double J[100] = {};
    //纬度
    double K[100] = {};

    double kk = double(k);
    c = (int)sqrt(kk);
    int idx=0,idy=0;
    srand(unsigned(time(0)));
    idx = (rand()%(c-1-1))+1;
    idy = (rand()%(c-1-1))+1;
    double g = 0;
    g = sqrt(s)/(c-1);
    for (int i = 0; i < 100; ++i) {
        J[i] = 0;
    }
    for (int i = 0; i < 100; ++i) {
        K[i] = 0;
    }
    for (int i = 0; i < c; ++i) {
        for (int j = 0; j < c; ++j) {
            J[j*c+i] = (i-idx)*g + mLongitude;
            K[j*c+i] = (j-idy)*g + mLatitude;
        }
    }

    jdoubleArray result = env->NewDoubleArray(201);
    double carr[1]={};
    carr[0]=idy*c+idx;
    //carr[0]=mLongitude;
    //carr[1]=mLatitude;
    env->SetDoubleArrayRegion(result,0,100,J);
    env->SetDoubleArrayRegion(result,100,100,K);
    env->SetDoubleArrayRegion(result,200,1,carr);
    return result;
}