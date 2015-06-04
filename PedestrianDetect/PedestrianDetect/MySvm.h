#ifndef MYSVM_H
#define MYSVM_H

#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
//�̳���CvSVM���࣬��Ϊ����setSVMDetector()���õ��ļ���Ӳ���ʱ����Ҫ�õ�ѵ���õ�SVM��decision_func������
//��ͨ���鿴CvSVMԴ���֪decision_func������protected���ͱ������޷�ֱ�ӷ��ʵ���ֻ�ܼ̳�֮��ͨ����������
class MySVM : public CvSVM
{
public:
    //���SVM�ľ��ߺ����е�alpha����
    double * get_alpha_vector()
    {
        return this->decision_func->alpha;
    }

    //���SVM�ľ��ߺ����е�rho����,��ƫ����
    double get_rho()
    {
        return this->decision_func->rho;
    }
};

#endif // MYSVM_H