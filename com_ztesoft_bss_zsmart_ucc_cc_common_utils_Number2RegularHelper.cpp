#include "com_ztesoft_bss_zsmart_ucc_cc_common_utils_Number2RegularHelper.h"
#include <stdio.h>
#include "num2re.h"
#include <stdlib.h>
#include <memory.h>
extern "C"
JNIEXPORT jobjectArray JNICALL Java_com_ztesoft_bss_zsmart_ucc_cc_common_utils_Number2RegularHelper_number2Regular
    (JNIEnv *env, jobject obj, jintArray numList)
{
    jint *c_array;
    jint len = (env)->GetArrayLength(numList);
    printf("numListLength=%d", len);

    c_array = (jint*)malloc(sizeof(jint) * len);
    memset(c_array, 0, sizeof(jint)*len);
    (env)->GetIntArrayRegion(numList, 0, len, c_array);

    Num2Re num2Re;
    for (int i = 0; i < len; i++)
    {
        num2Re.addNum(c_array[i]);
    }

    free(c_array);

    std::list<std::string> retList = num2Re.toRegexs();
    printf("retLength=%lu", retList.size());

    jclass objClass = (env)->FindClass("java/lang/String");
    jobjectArray texts = (env)->NewObjectArray((jsize)retList.size(), objClass, 0);
    jstring jstr;

    int i = 0;
    std::list<std::string>::iterator it = retList.begin();
    while (it != retList.end()) {
        jstr = (env)->NewStringUTF((*it).c_str());
        (env)->SetObjectArrayElement(texts, i, jstr);
        i++;
        it++;
    }

    return texts;
}

JNIEXPORT jstring JNICALL Java_com_ztesoft_bss_zsmart_ucc_cc_common_utils_Number2RegularHelper_test
  (JNIEnv *env, jobject, jint)
{
    jstring jstr = (env)->NewStringUTF( "test");
    return jstr;
}
