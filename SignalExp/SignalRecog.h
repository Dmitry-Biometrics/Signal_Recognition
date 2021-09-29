#ifndef SIGNALRECOG_H
#define SIGNALRECOG_H

#include <QVector>

class SignalRecogPrivate;

enum ClassificatorMode {EuclidMode, NormEuclidMode, SVMMode, NBCCMode};

class SignalRecog {
public:
    SignalRecog();
    ~SignalRecog();
    int setNormCoeffs(const QVector<float> &mean, const QVector<float> &stdDev);
    int train(const QVector<QVector<float> *> &ownBase, const QVector<QVector<float> *> &foreignBase, ClassificatorMode mode);
    int predict(const QVector<float> &sample, QVector<float> &y, int &label);
    int reset();
private:
    SignalRecogPrivate *_srp;
};




#endif // SIGNALRECOG_H
