#include <jni.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <algorithm>



using namespace std;

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_Casper(JNIEnv *env, jobject instance, jdouble mLongitude,
                                                 jdouble mLatitude, jint k, jdouble s) {

    int jmin = mLongitude * 1000000 - s;
    int jmax = mLongitude * 1000000 + s;
    int kmin = mLatitude * 1000000 - s;
    int kmax = mLatitude * 1000000 + s;

    //经度
    double J[10] = {};
    //纬度
    double K[10] = {};

    srand(unsigned(time(0)));
    for (int i = 0; i < 10; ++i) {
        J[i] = ((rand() % (jmax - jmin + 1)) + jmin) / (double)1000000;
        K[i] = ((rand() % (kmax - kmin + 1)) + kmin) / (double)1000000;
    }

    J[0] = mLongitude;
    K[0] = mLatitude;

    double minJ = 126.63;
    double minK = 45.74;
    double maxJ = 126.65;
    double maxK = 45.76;

    //上一次的结果
    double minJLast = 0;
    double minKLast = 0;
    double maxJLast = 0;
    double maxKLast = 0;


    int find = 0;
    int xsum = 0;

    while(find == 0)
    {
        xsum = 0;
        //比较
        for (int i = 0; i < 10; ++i) {
            if (J[i]>minJ && K[i]>minK && J[i]<maxJ && K[i]<maxK)
            {
                xsum++;
            }
        }
        if(xsum==k || xsum==0)
        {
            break;
        }
        if(xsum < k)
        {
            //break;
            //左下
            if(minJ == minJLast && minK == minKLast) {

                //J
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if (J[i] > minJ && K[i] > minK && J[i] < maxJLast && K[i] < maxK) {
                        xsum++;
                    }
                }
                if (xsum > k || xsum == k) {
                    maxKLast = maxK;
                    break;
                }

                //K
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if (J[i] > minJ && K[i] > minK && J[i] < maxJ && K[i] < maxKLast) {
                        xsum++;
                    }
                }
                if (xsum > k || xsum == k) {
                    maxJLast = maxJ;
                    break;
                }

            }
            //右下
            else if(maxJ == maxJLast && minK == minKLast)
            {
                //J
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if (J[i] > minJLast && K[i] > minK && J[i] < maxJLast && K[i] < maxK) {
                        xsum++;
                    }
                }
                if (xsum > k || xsum == k) {
                    maxKLast = maxK;
                    break;
                }

                //K
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if (J[i] > minJ && K[i] > minK && J[i] < maxJ && K[i] < maxKLast) {
                        xsum++;
                    }
                }
                if (xsum > k || xsum == k) {
                    minJLast = minJ;
                    break;
                }



            }
            //右上
            else if(maxJ == maxJLast && maxK == maxKLast)
            {
                //J
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if(J[i]>minJLast && K[i]>maxK && J[i]<maxJ && K[i]<maxK)
                    {
                        xsum++;
                    }
                }
                if(xsum > k || xsum == k) {
                    minKLast = minK;
                    break;
                }


                //K
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if(J[i]>minJ && K[i]>minKLast && J[i]<maxJ && K[i]<maxK)
                    {
                        xsum++;
                    }
                }
                if(xsum > k || xsum == k){
                    minJLast = minJ;
                    break;
                }
            }

            //左上
            else if(minJ == minJLast && maxK == maxKLast)
            {
                //J
                xsum = 0;
                for (int i = 0; i < 10; ++i) {
                    if(J[i]>minJ && K[i]>minK && J[i]<maxJLast && K[i]<maxK)
                    {
                        xsum++;
                    }
                }
                if(xsum>k||xsum==k)
                {
                    minKLast = minK;
                    break;
                }
                //K
                xsum=0;
                for (int i = 0; i < 10; ++i) {
                    if(J[i]>minJ && K[i]>maxKLast && J[i]<maxJ && K[i]<maxK)
                    {
                        xsum++;
                    }
                }
                if(xsum>k||xsum==k){
                    maxJLast = maxJ;
                    break;
                }

            } else{
                break;
            }
        }

        //保存上一次的结果
        minJLast = minJ;
        minKLast = minK;
        maxJLast = maxJ;
        maxKLast = maxK;

        //划分
        if(mLongitude<(minJ+maxJ)/(double)2)
        {
            maxJ = (minJ + maxJ)/(double)2;
        }
        else
        {
            minJ = (minJ + maxJ)/(double)2;
        }

        if(mLatitude<(minK+maxK)/(double)2)
        {
            maxK = (minK + maxK)/(double)2;
        }
        else{
            minK = (minK + maxK)/(double)2;
        }

    }

    jdoubleArray result = env->NewDoubleArray(24);
    double carr[4]={};
    carr[0]=minJLast;
    carr[1]=minKLast;
    carr[2]=maxJLast;
    carr[3]=maxKLast;
    env->SetDoubleArrayRegion(result,0,4,carr);
    env->SetDoubleArrayRegion(result,4,10,J);
    env->SetDoubleArrayRegion(result,14,10,K);
    return result;

}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_IC(JNIEnv *env, jobject instance, jdouble mLongitude,
                                             jdouble mLatitude, jint k, jdouble s) {
    int jmin = mLongitude * 1000000 - s;
    int jmax = mLongitude * 1000000 + s;
    int kmin = mLatitude * 1000000 - s;
    int kmax = mLatitude * 1000000 + s;

    //经度
    double J[10] = {};
    //纬度
    double K[10] = {};

    srand(unsigned(time(0)));
    for (int i = 0; i < 10; ++i) {
        J[i] = ((rand() % (jmax - jmin + 1)) + jmin) / (double)1000000;
        K[i] = ((rand() % (kmax - kmin + 1)) + kmin) / (double)1000000;
    }

    J[0] = mLongitude;
    K[0] = mLatitude;

    double minJ = 126.63;
    double minK = 45.74;
    double maxJ = 126.65;
    double maxK = 45.76;

    //上一次的结果
    double minJLast = 0;
    double minKLast = 0;
    double maxJLast = 0;
    double maxKLast = 0;


    int find = 0;
    int xsum = 0;

    while(find == 0)
    {
        xsum = 0;
        //比较
        for (int i = 0; i < 10; ++i) {
            if (J[i]>minJ && K[i]>minK && J[i]<maxJ && K[i]<maxK)
            {
                xsum++;
            }
        }
        if(xsum < k)
        {
            break;
        }


        //保存上一次的结果
        minJLast = minJ;
        minKLast = minK;
        maxJLast = maxJ;
        maxKLast = maxK;

        //划分
        if(mLongitude<(minJ+maxJ)/(double)2)
        {
            maxJ = (minJ + maxJ)/(double)2;
        }
        else
        {
            minJ = (minJ + maxJ)/(double)2;
        }

        if(mLatitude<(minK+maxK)/(double)2)
        {
            maxK = (minK + maxK)/(double)2;
        }
        else{
            minK = (minK + maxK)/(double)2;
        }

    }

    jdoubleArray result = env->NewDoubleArray(24);
    double carr[4]={};
    carr[0]=minJLast;
    carr[1]=minKLast;
    carr[2]=maxJLast;
    carr[3]=maxKLast;
    env->SetDoubleArrayRegion(result,0,4,carr);
    env->SetDoubleArrayRegion(result,4,10,J);
    env->SetDoubleArrayRegion(result,14,10,K);
    return result;

}



extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_NNC(JNIEnv *env, jobject instance, jdouble mLongitude,
                                              jdouble mLatitude, jint k, jdouble s) {
    int jmin = mLongitude * 1000000 - s;
    int jmax = mLongitude * 1000000 + s;
    int kmin = mLatitude * 1000000 - s;
    int kmax = mLatitude * 1000000 + s;

    //经度
    double J[10] = {};
    //纬度
    double K[10] = {};

    //排序后距离
    double len[10] = {};
    //排序前距离
    double num[10] = {};


    //第一次候选
    int choose1[10] = {};
    int choose2[10] = {};
    int ch2;

    srand(unsigned(time(0)));
    for (int i = 0; i < 10; ++i) {
        //J[i] = ((rand() % (jmax - jmin + 1)) + jmin) ;
        //K[i] = ((rand() % (kmax - kmin + 1)) + kmin) ;
        J[i] = ((rand() % (jmax - jmin + 1)) + jmin) / (double)1000000;
        K[i] = ((rand() % (kmax - kmin + 1)) + kmin) / (double)1000000;
    }

    J[0] = mLongitude;
    K[0] = mLatitude;

    for (int i = 0; i < 10; i++)
    {
        //距离
        len[i] = pow(J[i] - mLongitude, 2) + pow(K[i] - mLatitude, 2);
        num[i] = len[i];
    }

    sort(len, len + 10);

    for (int i = 0; i < k ; i++)
    {
        for (int j = 0; j <10; j++)
        {
            if (len[i] == num[j])
            {
                choose1[i] = j;
            }
        }
    }

    //选取第二次构建的中心
    srand(unsigned(time(0)));
    ch2 = (rand() % (k - 0)) + 0;



    for (int i = 0; i < 10; i++)
    {
        //距离
        len[i] = pow(J[i] - J[choose1[ch2]], 2) + pow(K[i] - K[choose1[ch2]], 2);
        num[i] = len[i];
    }
    sort(len, len + 10);
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j <10; j++)
        {
            if (len[i] == num[j])
            {
                choose2[i] = j;
            }
        }
    }

    choose2[k] = 0;

    double maxJ = 0;
    double minJ = 10000;
    double maxK = 0;
    double minK = 10000;
    for (int i = 0; i < k+1; i++)
    {
        if (J[choose2[i]] < minJ)
        {
            minJ = J[choose2[i]];
        }
        if (J[choose2[i]] > maxJ)
        {
            maxJ = J[choose2[i]];
        }
        if (K[choose2[i]] < minK)
        {
            minK = K[choose2[i]];
        }
        if (K[choose2[i]] > maxK)
        {
            maxK = K[choose2[i]];
        }
    }

    jdoubleArray result = env->NewDoubleArray(24);
    double carr[4]={};
    carr[0]=minJ;
    carr[1]=minK;
    carr[2]=maxJ;
    carr[3]=maxK;
    env->SetDoubleArrayRegion(result,0,4,carr);
    env->SetDoubleArrayRegion(result,4,10,J);
    env->SetDoubleArrayRegion(result,14,10,K);
    return result;

}

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