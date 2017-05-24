#include <jni.h>
#include <string>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <algorithm>
#include <vector>


using namespace std;
const int UserNum = 6014;//用户数量
const int NUM = 1; //定义划分簇的数目

//数据向量表示
struct Vect
{
    double x;
    double y;
};

double getDist(Vect A, Vect B)  //计算距离的平方
{
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}

//计算每个簇的中心，平均距离表示
Vect getMeansC(vector<Vect> t)
{
    int num = t.size();
    double meansX = 0;
    double meansY = 0;
    for (int i = 0; i<num; i++)
    {
        meansX += t[i].x;
        meansY += t[i].y;
    }
    Vect c;
    c.x = meansX / num;
    c.y = meansY / num;
    return c;
}

//获取算法的准则函数值，当准则函数收敛时算法停止
double getE(vector<Vect> classes[], Vect means[])
{
    double sum = 0;
    for (int i = 0; i<NUM; i++)
    {
        vector<Vect> v = classes[i];
        for (int j = 0; j<v.size(); j++)
            sum += getDist(v[j], means[i]);
    }
    return sum;
}

int searchMinC(Vect t, Vect means[NUM])
{
    int c = 0;
    double d = getDist(t, means[0]);
    for (int i = 1; i<NUM; i++)
    {
        double tmp = getDist(t, means[i]);
        if (tmp < d)
        {
            c = i;
            d = tmp;
        }
    }
    return c;
}

void kMeans(vector<Vect> init)
{


}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_CasperTEST(JNIEnv *env, jobject instance,
                                                     jdouble mLongitude, jdouble mLatitude, jint k,
                                                     jdouble s, jdoubleArray fLatitude_,
                                                     jdoubleArray fLongitude_) {
    jdouble *fLatitude = env->GetDoubleArrayElements(fLatitude_, NULL);
    jdouble *fLongitude = env->GetDoubleArrayElements(fLongitude_, NULL);

    double minJ = 29.70;
    double minK = 62.55;
    double maxJ = 29.80;
    double maxK = 62.65;

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
            if (fLongitude[i]>minJ && fLatitude[i]>minK && fLongitude[i]<maxJ && fLatitude[i]<maxK)
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
                    if (fLongitude[i] > minJ && fLatitude[i] > minK && fLongitude[i] < maxJLast && fLatitude[i] < maxK) {
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
                    if (fLongitude[i] > minJ && fLatitude[i] > minK && fLongitude[i] < maxJ && fLatitude[i] < maxKLast) {
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
                    if (fLongitude[i] > minJLast && fLatitude[i] > minK && fLongitude[i] < maxJLast && fLatitude[i] < maxK) {
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
                    if (fLongitude[i] > minJ && fLatitude[i] > minK && fLongitude[i] < maxJ && fLatitude[i] < maxKLast) {
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
                    if(fLongitude[i]>minJLast && fLatitude[i]>maxK && fLongitude[i]<maxJ && fLatitude[i]<maxK)
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
                    if(fLongitude[i]>minJ && fLatitude[i]>minKLast && fLongitude[i]<maxJ && fLatitude[i]<maxK)
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
                    if(fLongitude[i]>minJ && fLatitude[i]>minK && fLongitude[i]<maxJLast && fLatitude[i]<maxK)
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
                    if(fLongitude[i]>minJ && fLatitude[i]>maxKLast && fLongitude[i]<maxJ && fLatitude[i]<maxK)
                    {
                        xsum++;
                    }
                }
                if(xsum>k||xsum==k){
                    maxJLast = maxJ;
                    break;
                }

            }
//            else{
//                break;
//            }
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


    //伪装区域内的点
    int inarenum = 0;
    double inJ[UserNum];
    double inK[UserNum];

    int start = 0;
    for (int i = 0; i < UserNum; i++)
    {
        if ((fLongitude[i] > minJLast || fLongitude[i] == minJLast) && (fLongitude[i] < maxJLast || fLongitude[i] == maxJLast) && (fLatitude[i] > minKLast || fLatitude[i] == minKLast) && (fLatitude[i] < maxKLast || fLatitude[i] == maxKLast))
        {
            inarenum++;
            inJ[start] = fLongitude[i];
            inK[start] = fLatitude[i];
            start++;
        }
    }

    vector<Vect>init;
    for (int i = 0; i < inarenum; i++)
    {
        Vect t;
        t.x = inJ[i];
        t.y = inK[i];
        init.push_back(t);
    }

    //kMeans(init);
    Vect means[NUM];
    vector<Vect> classes[NUM];
    double newE, oldE = -1;
    srand(time(NULL));
    for (int i = 0; i < NUM; i++)
    {
        int c = rand() % init.size();
        classes[i].push_back(init[c]);
        means[i] = getMeansC(classes[i]);  //计算当前每个簇的中心点
    }
    newE = getE(classes, means);  //计算当前准则函数值
    for (int i = 0; i < NUM; i++)
        classes[i].clear();
    vector<Vect> ans[NUM];
    while (fabs(newE - oldE) >= 1)
    {
        for (int i = 0; i < init.size(); i++)
        {
            int toC = searchMinC(init[i], means);
            classes[toC].push_back(init[i]);
        }
        for (int i = 0; i < NUM; i++)
            ans[i] = classes[i];
        for (int i = 0; i < NUM; i++)
            means[i] = getMeansC(classes[i]);
        oldE = newE;
        newE = getE(classes, means);
        for (int i = 0; i < NUM; i++)
            classes[i].clear();
    }

    jdoubleArray result = env->NewDoubleArray(7);
    double carr[7]={};
    carr[0]=minJLast;
    carr[1]=minKLast;
    carr[2]=maxJLast;
    carr[3]=maxKLast;
    carr[4]=means[0].x;
    carr[5]=means[0].y;

    //距离
    carr[6] = sqrt(pow(carr[4] - mLongitude,2) + pow(carr[5] - mLatitude,2));

    env->SetDoubleArrayRegion(result,0,7,carr);

    env->ReleaseDoubleArrayElements(fLatitude_, fLatitude, 0);
    env->ReleaseDoubleArrayElements(fLongitude_, fLongitude, 0);
    return result;
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_ICTEST(JNIEnv *env, jobject instance, jdouble mLongitude,
                                                 jdouble mLatitude, jint k, jdouble s,
                                                 jdoubleArray fLatitude_,
                                                 jdoubleArray fLongitude_) {
    jdouble *fLatitude = env->GetDoubleArrayElements(fLatitude_, NULL);
    jdouble *fLongitude = env->GetDoubleArrayElements(fLongitude_, NULL);

    double minJ = 29.70;
    double minK = 62.55;
    double maxJ = 29.80;
    double maxK = 62.65;

    double minJLast = 0;
    double minKLast = 0;
    double maxJLast = 0;
    double maxKLast = 0;

    int find = 0;
    int xsum = 0;
    while (find == 0)
    {
        xsum = 0;
        //比较
        for (int i = 0; i < UserNum; ++i) {
            if (fLongitude[i] > minJ && fLatitude[i] > minK && fLongitude[i] < maxJ && fLatitude[i] < maxK)
            {
                xsum++;
            }
        }
        if (xsum < k)
        {
            break;
        }


        //保存上一次的结果
        minJLast = minJ;
        minKLast = minK;
        maxJLast = maxJ;
        maxKLast = maxK;

        //划分
        if (mLongitude < (minJ + maxJ) / (double)2)
        {
            maxJ = (minJ + maxJ) / (double)2;
        }
        else
        {
            minJ = (minJ + maxJ) / (double)2;
        }

        if (mLatitude < (minK + maxK) / (double)2)
        {
            maxK = (minK + maxK) / (double)2;
        }
        else {
            minK = (minK + maxK) / (double)2;
        }
    }

    //伪装区域内的点
    int inarenum = 0;
    double inJ[UserNum];
    double inK[UserNum];

    int start = 0;
    for (int i = 0; i < UserNum; i++)
    {
        if ((fLongitude[i] > minJLast || fLongitude[i] == minJLast) && (fLongitude[i] < maxJLast || fLongitude[i] == maxJLast) && (fLatitude[i] > minKLast || fLatitude[i] == minKLast) && (fLatitude[i] < maxKLast || fLatitude[i] == maxKLast))
        {
            inarenum++;
            inJ[start] = fLongitude[i];
            inK[start] = fLatitude[i];
            start++;
        }
    }

    vector<Vect>init;
    for (int i = 0; i < inarenum; i++)
    {
        Vect t;
        t.x = inJ[i];
        t.y = inK[i];
        init.push_back(t);
    }

    //kMeans(init);
    Vect means[NUM];
    vector<Vect> classes[NUM];
    double newE, oldE = -1;
    srand(time(NULL));
    for (int i = 0; i < NUM; i++)
    {
        int c = rand() % init.size();
        classes[i].push_back(init[c]);
        means[i] = getMeansC(classes[i]);  //计算当前每个簇的中心点
    }
    newE = getE(classes, means);  //计算当前准则函数值
    for (int i = 0; i < NUM; i++)
        classes[i].clear();
    vector<Vect> ans[NUM];
    while (fabs(newE - oldE) >= 1)
    {
        for (int i = 0; i < init.size(); i++)
        {
            int toC = searchMinC(init[i], means);
            classes[toC].push_back(init[i]);
        }
        for (int i = 0; i < NUM; i++)
            ans[i] = classes[i];
        for (int i = 0; i < NUM; i++)
            means[i] = getMeansC(classes[i]);
        oldE = newE;
        newE = getE(classes, means);
        for (int i = 0; i < NUM; i++)
            classes[i].clear();
    }


    jdoubleArray result = env->NewDoubleArray(7);
    double carr[7]={};
    carr[0]=minJLast;
    carr[1]=minKLast;
    carr[2]=maxJLast;
    carr[3]=maxKLast;
    carr[4]=means[0].x;
    carr[5]=means[0].y;

    //距离
    carr[6] = sqrt(pow(carr[4] - mLongitude,2) + pow(carr[5] - mLatitude,2));

    env->SetDoubleArrayRegion(result,0,7,carr);

    env->ReleaseDoubleArrayElements(fLatitude_, fLatitude, 0);
    env->ReleaseDoubleArrayElements(fLongitude_, fLongitude, 0);
    return result;
}

extern "C"
JNIEXPORT jdoubleArray JNICALL
Java_com_example_kimi_lbspro_MainActivity_NNCTEST(JNIEnv *env, jobject instance, jdouble mLongitude,
                                                  jdouble mLatitude, jint k, jdouble s,
                                                  jdoubleArray fLatitude_, jdoubleArray fLongitude_) {
    jdouble *fLatitude = env->GetDoubleArrayElements(fLatitude_, NULL);
    jdouble *fLongitude = env->GetDoubleArrayElements(fLongitude_, NULL);

    //排序后距离
    double len[UserNum] = {};
    //排序前距离
    double num[UserNum] = {};
    //第二次的随机选择
    int randk[100] = {};

    //第一次候选
    int choose1[UserNum] = {};
    int choose2[UserNum] = {};
    int ch2;

    srand((unsigned)time(NULL));
    for (int i = 0; i < 100; i++)
    {
        randk[i] = (rand() % (k - 0)) + 0;
    }

    //100次的结果
    double lastj[100] = {};
    double lastk[100] = {};

    //100次
    for (int dotime = 0; dotime < 100; dotime++)
    {
        //初始化
        for (int i = 0; i < UserNum; i++)
        {
            len[i] = 0;
            num[i] = 0;
            choose1[i] = 0;
            choose2[2] = 0;
        }

        for (int i = 0; i < UserNum; i++)
        {
            //距离
            len[i] = pow(fLongitude[i] - mLongitude, 2) + pow(fLatitude[i] - mLatitude, 2);
            num[i] = len[i];
        }

        sort(len, len + UserNum);

        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < UserNum; j++)
            {
                if (len[i] == num[j])
                {
                    choose1[i] = j;
                }
            }
        }

        ch2 = randk[dotime];

        for (int i = 0; i < UserNum; i++)
        {
            //距离
            len[i] = pow(fLongitude[i] - fLongitude[choose1[ch2]], 2) + pow(fLatitude[i] - fLatitude[choose1[ch2]], 2);
            num[i] = len[i];
        }
        sort(len, len + UserNum);
        for (int i = 0; i < k; i++)
        {
            for (int j = 0; j < UserNum; j++)
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
        for (int i = 0; i < k + 1; i++)
        {
            if (fLongitude[choose2[i]] < minJ)
            {
                minJ = fLongitude[choose2[i]];
            }
            if (fLongitude[choose2[i]] > maxJ)
            {
                maxJ = fLongitude[choose2[i]];
            }
            if (fLatitude[choose2[i]] < minK)
            {
                minK = fLatitude[choose2[i]];
            }
            if (fLatitude[choose2[i]] > maxK)
            {
                maxK = fLatitude[choose2[i]];
            }
        }

        //伪装区域内的点
        int inarenum = 0;
        double inJ[UserNum];
        double inK[UserNum];

        int start = 0;
        for (int i = 0; i < UserNum; i++)
        {
            if ((fLongitude[i] > minJ || fLongitude[i] == minJ) && (fLongitude[i] < maxJ || fLongitude[i] == maxJ) && (fLatitude[i] > minK || fLatitude[i] == minK) && (fLatitude[i] < maxK || fLatitude[i] == maxK))
            {
                inarenum++;
                inJ[start] = fLongitude[i];
                inK[start] = fLatitude[i];
                start++;
            }
        }

        vector<Vect>init;
        for (int i = 0; i < inarenum; i++)
        {
            Vect t;
            t.x = inJ[i];
            t.y = inK[i];
            init.push_back(t);
        }

        //kMeans(init);
        Vect means[NUM];
        vector<Vect> classes[NUM];
        double newE, oldE = -1;
        srand(time(NULL));
        for (int i = 0; i<NUM; i++)
        {
            int c = rand() % init.size();
            classes[i].push_back(init[c]);
            means[i] = getMeansC(classes[i]);  //计算当前每个簇的中心点
        }
        newE = getE(classes, means);  //计算当前准则函数值
        for (int i = 0; i<NUM; i++)
            classes[i].clear();
        vector<Vect> ans[NUM];
        while (fabs(newE - oldE) >= 1)
        {
            for (int i = 0; i<init.size(); i++)
            {
                int toC = searchMinC(init[i], means);
                classes[toC].push_back(init[i]);
            }
            for (int i = 0; i<NUM; i++)
                ans[i] = classes[i];
            for (int i = 0; i<NUM; i++)
                means[i] = getMeansC(classes[i]);
            oldE = newE;
            newE = getE(classes, means);
            for (int i = 0; i<NUM; i++)
                classes[i].clear();
        }
        //end kmean

        lastj[dotime] = means[0].x;
        lastk[dotime] = means[0].y;
    }

    //距离真实位置的平均距离
    double aves = 0;
    for (int i = 0; i < 100; ++i) {
        aves += sqrt(pow(lastj[i] - mLongitude, 2) + pow(lastk[i] - mLatitude, 2));
    }
    aves = aves / 100;

    // 方差
    double ex = 0;
    for (int i = 0; i < 100; i++)
    {
        ex += pow(sqrt(pow(lastj[i] - mLongitude, 2) + pow(lastk[i] - mLatitude, 2)) - aves, 2);
    }
    ex = ex / 100;

    jdoubleArray result = env->NewDoubleArray(202);
    double carr[2]={};
    carr[0]=aves;
    carr[1]=ex;

    env->SetDoubleArrayRegion(result,0,100,lastj);  //经度
    env->SetDoubleArrayRegion(result,100,100,lastk);  //纬度
    env->SetDoubleArrayRegion(result,200,2,carr);

    env->ReleaseDoubleArrayElements(fLatitude_, fLatitude, 0);
    env->ReleaseDoubleArrayElements(fLongitude_, fLongitude, 0);
    return result;
}

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

    //伪装区域内的点
    int inarenum = 0;
    double inJ[15];
    double inK[15];

    int start = 0;
    for (int i = 0; i < 10; i++)
    {
        if ((J[i] > minJ || J[i] == minJ) && (J[i] < maxJ || J[i] == maxJ) && (K[i] > minK || K[i] == minK) && (K[i] < maxK || K[i] == maxK))
        {
            inarenum++;
            inJ[start] = J[i];
            inK[start] = K[i];
            start++;
        }
    }

    vector<Vect>init;
    for (int i = 0; i < inarenum; i++)
    {
        Vect t;
        t.x = inJ[i];
        t.y = inK[i];
        init.push_back(t);
    }

    //KEMANS
    Vect means[NUM];
    vector<Vect> classes[NUM];
    double newE, oldE = -1;
    srand(time(NULL));
    for (int i = 0; i<NUM; i++)
    {
        int c = rand() % init.size();
        classes[i].push_back(init[c]);
        means[i] = getMeansC(classes[i]);  //计算当前每个簇的中心点
    }
    newE = getE(classes, means);  //计算当前准则函数值
    for (int i = 0; i<NUM; i++)
        classes[i].clear();
    vector<Vect> ans[NUM];
    while (fabs(newE - oldE) >= 1)
    {
        for (int i = 0; i<init.size(); i++)
        {
            int toC = searchMinC(init[i], means);
            classes[toC].push_back(init[i]);
        }
        for (int i = 0; i<NUM; i++)
            ans[i] = classes[i];
        for (int i = 0; i<NUM; i++)
            means[i] = getMeansC(classes[i]);
        oldE = newE;
        newE = getE(classes, means);
        for (int i = 0; i<NUM; i++)
            classes[i].clear();
    }

    double kmean[2]={};
    kmean[0] = means[0].x;
    kmean[1] = means[0].y;
    //KEANS END

    //24，25是k-mean坐标
    jdoubleArray result = env->NewDoubleArray(26);
    double carr[4]={};
    carr[0]=minJ;
    carr[1]=minK;
    carr[2]=maxJ;
    carr[3]=maxK;
    env->SetDoubleArrayRegion(result,0,4,carr);
    env->SetDoubleArrayRegion(result,4,10,J);
    env->SetDoubleArrayRegion(result,14,10,K);
    env->SetDoubleArrayRegion(result,24,2,kmean);

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