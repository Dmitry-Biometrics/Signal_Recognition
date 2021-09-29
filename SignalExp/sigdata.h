#ifndef SIGDATA_H
#define SIGDATA_H

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QVector>

typedef QVector<float>  VecF32;
typedef QVector<int>    VecI32;

class SigClassificator;


QString respToString(const VecF32 &resp, bool defaultHex);

//Преобразование строки к вектору откликов
//  s      - строка вида "1", "1.0, -2.3", "b 11010 1011", "xF34AB19"
//  length - ожидаемая длина кода
VecF32 stringToResp(const QString &s, int length);

//Пример
struct SigData {
    QString name;
    VecF32 data;

    SigData(){}
    SigData(const QString &name, const VecF32& x):name(name),data(x){}
};

//Метка
struct SigMark {
    QSharedPointer<SigData> _x;
    VecF32 _y;  //отклик
    int _lb[2]; //метки {эталонный класс, фактический класс}

    SigMark(){ _lb[0]=-1; _lb[1]=-1; }
    SigMark(const SigMark &m):_x(m._x),_y(m._y){ _lb[0]=m._lb[0]; _lb[1]=m._lb[1]; }
    SigMark(const QString &name, const VecF32 &x):_x(new SigData(name,x)){ _lb[0]=-1; _lb[1]=-1; }
    SigMark(QSharedPointer<SigData> &x):_x(x){ _lb[0]=-1; _lb[1]=-1; }

    const QString& name() const     { return _x->name; }
    const VecF32& x() const { return _x->data; }

    VecF32* x_ptr() { return &_x->data; }
    VecF32* y_ptr() { return &_y; }
    const VecF32* x_ptr() const { return &_x->data; }
    const VecF32* y_ptr() const { return &_y; }

    void mark(int id, int val)  { _lb[id]=val; }

    void y(const VecF32 &y){ _y=y; }
    const VecF32& y() const { return _y; }
    void reset(){ _lb[0]=-1; _lb[1]=-1; _y.clear(); }
};

//Набор меток
struct SigMarks {
    QVector<SigMark> _m;                //список меток
    QSharedPointer<SigClassificator> _c;


    SigMarks(){}
    SigMarks(const SigMarks &marks)     { append(marks); }
    SigMarks(const QVector<VecF32> &x)  { append(x); }

    int count() const { return _m.count(); }
    int size() const  { return _m.size(); }
    void reserve(int n){ _m.reserve(n); }

    const SigMark&  at(int i) const { return _m.at(i); }
    SigMark& operator[](int i)              { return _m[i]; }
    const SigMark& operator[](int i) const  { return _m[i]; }

    //Доступ к данным
    QVector<VecF32*> x();
    QVector<VecF32*> x(const VecI32 &pos);
    QVector<VecF32*> y();
    QVector<VecF32*> y(const VecI32 &pos);

    VecI32          label(int id) const;    //вектор меток с заданным индексом (id = {0,1})
    VecI32          label(int id, const VecI32 &pos) const;    //вектор меток с заданным индексом (id = {0,1})

    void mark(int id, const VecI32 &pos, const VecI32 &val);    //установка меток
    void mark(int id, const VecI32 &val);                       //установка меток


    void append(const SigMark &mark);           //добавление примера
    void append(const SigMarks &marks);         //добавление метки
    void append(const QVector<VecF32> &data);   //добавление набора данных
    void append(const VecF32 &data);            //добавление данных

    int  appendFromFile(const QString &path, const QString &name);
    bool writeToTxtFile(const QString &pathName, int id);
    bool writeToBinFile(const QString &pathName, const QVector<int> &id);

    void reset();                           //сброс результатов (но не списка данных)
    void remove(const QVector<int> &pos);   //удаление по позициям
    void clear();                           //полный сброс

    QMap<int, QMap<int,int> > stats() const;

    void classif(QSharedPointer<SigClassificator> c){ _c=c; }  //установка классификатора
    QSharedPointer<SigClassificator> classif(){ return _c; }

    SigMarks slice(const VecI32 &pos) const;   //вырезание данных

    enum CompareOperation {
        Null,
        AnyValue,
        Equal,
        NotEqual,
        Less,
        LessOrEqual,
        Great,
        GreatOrEqual,
        InRange,
        NotInRange,
        InSet,
        NotInSet
    };

    struct SelRule {
        CompareOperation _op; //операция
        VecF32           _v;  //значение

        SelRule():_op(AnyValue),_v(1,0){}
        SelRule(const SelRule &x):_op(x._op),_v(x._v){}
        SelRule(CompareOperation op, float value=0.0):_op(op),_v(1,value){}
        SelRule(CompareOperation op, const VecF32 &value):_op(op),_v(value){}

        bool check(const VecF32 &value) const {
            if (_op==Null)     return value.isEmpty();
            switch (_op){
            case Null:      return value.isEmpty();
            case AnyValue:  return true;
            case Equal:     if (value.size()!=_v.size()) return false;
                            for (int i=0; i<_v.size(); ++i)
                                if (value[i]!=_v[i]) return false;
                            return true;
            case NotEqual:  if (value.size()!=_v.size()) return false;
                            for (int i=0; i<_v.size(); ++i)
                                if (value[i]!=_v[i]) return true;
                            return true;
            default: return false;
            }
        }

        bool check(float value) const {
            switch (_op){
            case Null:      return false;
            case AnyValue:  return true;
            case Equal:     if (_v.size()!=1) return false;
                            return value==_v[0];
            case NotEqual:  if (_v.size()!=1) return false;
                            return value!=_v[0];
            case Less:      if (_v.size()!=1) return false;
                            return value<_v[0];
            case LessOrEqual: if (_v.size()!=1) return false;
                            return value<=_v[0];
            case Great:     if (_v.size()!=1) return false;
                            return value>_v[0];
            case GreatOrEqual: if (_v.size()!=1) return false;
                            return value>=_v[0];
            case InRange:   if (_v.size()!=2) return false;
                            return (value>=_v[0] && value<=_v[1]);
            case NotInRange:if (_v.size()!=2) return false;
                            return (value<_v[0] || value>_v[1]);
            case InSet:     for (int i=0; i<_v.size(); ++i)
                                if (value==_v[i]) return true;
                            return false;
            case NotInSet:  for (int i=0; i<_v.size(); ++i)
                                if (value==_v[i]) return false;
                            return true;
            default: return false;
            }
        }
    };

    static SelRule notEqual(int value){ return SelRule(NotEqual,value); }
    static SelRule equal(int value){ return SelRule(Equal,value); }
    static SelRule anyValue(){ return SelRule(AnyValue); }


    //Выбор элементов по набору условий
    //ruleY - требования к y (или AnyValue)
    //rule0 - требования к _lb0 (или AnyValue)
    //rule1 - требования к _lb1 (или AnyValue)
    //rule0_1 - требования к значению _lb0 относительно _lb1 (или AnyValue)
    VecI32 select(const SelRule &ruleY, const SelRule &rule0, const SelRule &rule1, const CompareOperation &rule0_1, const QVector<int> *pos=0) const;

    //Выбор элементов с заданными номерами
    VecI32 select(int from, int count, int step, const QVector<int> *pos=0) const;

};


Q_DECLARE_METATYPE(SigMarks*)

#endif // SDATASET_H
