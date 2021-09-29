#include <QTextStream>
#include <QFile>
#include <QString>
#include <QDataStream>
#include "msg.h"
#include "res.h"
#include "sigdata.h"
#include "xlsxdocument.h"



//Преобразование вектора отклика к вектору строк
QString respToString(const VecF32 &resp, bool defaultHex){
    if (resp.size()==0) return QString();
    if (resp.size()==1) return QString::number(resp[0]);
    bool allBin=true;

    foreach(float f, resp){
        if (f!=1.0f && f!=0.0f) {
            allBin=false; break;
        }
    }
    if (allBin){
        if (defaultHex){    //Шестнадцатиричная строка
            QString s = "x";
            static const QString hex = "0123456789ABCDEF";
            for (int j=0,size=resp.size(); size>0; size-=4, j+=4){
                int irem = (size>=4)? 4 : size;
                int code=0;
                for (int i=0; i<irem; ++i){
                    if (resp[j+i]!=0.0f)
                        code = (1<<i) | code;   //установить только единичные значения
                }
                s.append(hex[code]);
            }
            return s;
        } else {    //Бинарная строка
            QString s;
            s.resize(resp.size()+1);
            s[0]='b';
            for (int i=0; i<resp.size(); ++i)
                if (resp[i]==0.0f)  s[1+i]='0';
                else                s[1+i]='1';
            return s;
        }
    } else {    //Строка вещественных значений
        QString s;
        for (int i=0; i<resp.size(); ++i){
            if (i) s.append(", ");
            s.append(QString::number(resp[i]));
        }
        return s;
    }
}


int charToHex(char c){
    if (c>='0' && c<='9') return (int)(c-'0');
    if (c>='A' && c<='F') return (int)((unsigned char)c-(unsigned char)'A')+10;
    if (c>='a' && c<='f') return (int)((unsigned char)c-(unsigned char)'a')+10;
    return -1;
}

//Преобразование строки к вектору откликов, length - ожидаемая длина кода
VecF32 stringToResp(const QString &s, int length){
    VecF32 v; v.reserve(length); //вектор значений
    QString t1 = s;
    QString t2 = t1.remove(QRegExp("[ \t\n\r]"));   //Убрать пробелы
    if (t2.length()==0) {
        if (length>0) Msg::info(STR_RES("Значение не задано"));
        return VecF32();
    }

    if (t2[0]=='b'){            //Обработать как бинарный код
        if (length<0) length=t2.length()-1;
        if (t2.length()!=(1+length)){
            Msg::info(STR_RES("Число введенных значений (%1) отличается от ожидаемого (%2)").arg(t2.length()-1).arg(length));
            return VecF32();
        }
        for (int i=1; i<t2.length(); ++i){
            if (t2[i]=='0'){ v.push_back(0.0f); continue; }
            if (t2[i]=='1'){ v.push_back(1.0f); continue; }
            Msg::info(STR_RES("Значение элемента не бинарное [0,1]"));
            return VecF32();
        }
        return v;
    }

    if (t2[0]=='x'){     //Обработать как шестнадцатиричный код
        if (length<0) length=(t2.length()-1)*4;
        if (t2.length() != (1+(length+3)/4)){
            Msg::info(STR_RES("Число введенных значений (%1) отличается от ожидаемого (%2)").arg(((t2.length()-1)*4)).arg(length));
            return VecF32();
        }
        for (int i=1; i<t2.length(); ++i){
            int code = charToHex(t2[i].toLatin1());
            if (code>=0){
                for (int p=0; p<4; ++p)
                    v.push_back(( code >> p) & 1);
                continue;
            }
            Msg::info(STR_RES("Значение элемента не шестнадцатиричное [0-9,A-F,a-f]"));
            return VecF32();
        }
        v.resize(length);//уменьшить длину, если нужно
        return v;
    }

    {   //Обработать как набор вещественных чисел
        QStringList l2 = t2.split(QRegExp("[,;]"),QString::SkipEmptyParts);
        if (length<0) length=l2.length();
        if (l2.count() != length){
            Msg::info(STR_RES("Число введенных значений (%1) отличается от ожидамого (%2)").arg(l2.count()).arg(length));
            return VecF32();
        }
        bool ok;
        for (int i=0; i<l2.count(); ++i){
            v.push_back(l2[i].toFloat(&ok));
            if (ok) continue;
            Msg::info(STR_RES("Значение элемента не вещественное"));
            return VecF32();
        }
    }
    return v;
}

//Чтение из текстового файла
bool readFromTxtFile(const QString &name, QVector<float> &data){
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) return false;
    data.clear();
    QTextStream stream(&file);
    bool ok;
    while (!stream.atEnd()) {
        QString s = stream.readLine();
        if (!s.isEmpty()) {
            data.push_back(s.toFloat(&ok));
            if (!ok) data.pop_back();
        }
    }
    file.close();
    return true;
}

//В одном файле только единицы (столбец 2) или только нули (столбец 3)
//если все значения 0, то данных нет
bool readFromXlsxFile(const QString &name, QVector<QVector<float> > &data){

  QXlsx::Document xlsx(name);
  int row = xlsx.dimension().lastRow();

  data.clear();
  for (int col=2; col<=3; ++col){
    QVector<float> v; v.reserve(row);
    bool not_empty=false;
    for (int i=1; i<=row; ++i){
        QXlsx::Cell *cell=xlsx.cellAt(i, col);
        if (!cell) return false;
        float f = cell->value().toFloat();
        if (f!=0.0f) not_empty=true;    //если все 0.0, то данные не добавляются
        v.push_back(f);
    }
    if (not_empty) data.push_back(v);
  }

  return true;
}



bool readFromBinFile(const QString &name, QVector<QVector<float> > &data){
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly)) return false;
    QDataStream stream(&file);
    int col;    //число элементов
    int row;    //число записей
    if (!stream.readRawData((char*)&row,sizeof(row))) return false;
    if (!stream.readRawData((char*)&col,sizeof(col))) return false;

    data.resize(row);
    for (int r=0; r<row; ++r){
        data[r].resize(col);
        for (int c=0; c<col; ++c)
            if (!stream.readRawData((char*)&data[r][c],sizeof(float)))
                return false;
    }
    return true;
}

bool writeToTxtFile(const QString &name, const QVector<float> &data){
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QTextStream stream(&file);
    for (int i=0; i<data.size(); ++i)
        stream<<data[i]<<'\n';
    return true;
}

bool writeToBinFile(const QString &name, const QVector< QVector<float> > &data){
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QDataStream stream(&file);
    int row = data.count();    //число элементов
    int col = (data.count())? data[0].count() : 0; //число записей

    if (!stream.writeRawData((const char*)&row,sizeof(row))) return false;
    if (!stream.writeRawData((const char*)&col,sizeof(col))) return false;

    for (int r=0; r<row; ++r){
        for (int c=0; c<col; ++c){
            if (data[r].count()!=col) return false; //отличающееся число столбцов
            if (!stream.writeRawData((const char*)&data[r][c],sizeof(float)))
                return false;
        }
    }
    return true;
}



void SigMarks::append(const SigMark &mark){
    _m.append(mark);
}



void SigMarks::append(const SigMarks &marks){
    for (int i=0; i<marks.count(); ++i)
        _m.append(marks[i]);
}

void SigMarks::append(const VecF32 &data){
    QSharedPointer<SigData> p(new SigData());
    p->name = QString::number(_m.count());
    p->data = data;
    _m.push_back(SigMark(p));
}

//Добавление данных
void SigMarks::append(const QVector<VecF32> &data){
    for (int i=0; i<data.size(); ++i)
        append(data[i]);
}



//Добавление одной записи
int SigMarks::appendFromFile(const QString &path, const QString &name){
    bool rslt=false;
    QVector< QVector<float> > data; data.resize(1);
    QString ext = name.split('.').last();
    if (ext=="txt")       rslt = readFromTxtFile(path+'/'+name, data[0]);
    else if (ext=="xlsx") rslt = readFromXlsxFile(path+'/'+name, data);
    else if (ext=="bin")  rslt = readFromBinFile(path+'/'+name, data);
    if (!rslt) return 0;
    for (int i=0; i<data.count(); ++i){
        QSharedPointer<SigData> p(new SigData());
        p->name = name + ':' + QString::number(i);
        p->data = data[i];
        _m.push_back(SigMark(p));
    }
    return data.count();
}

bool SigMarks::writeToTxtFile(const QString &pathName, int id){
    return ::writeToTxtFile(pathName,_m[id].x());
}

bool SigMarks::writeToBinFile(const QString &pathName, const QVector<int> &id){
    QVector< QVector<float> > data; data.reserve(id.count());
    for (int i=0; i<id.count(); ++i)
        data.push_back(_m[id[i]].x());
    return ::writeToBinFile(pathName,data);
}


void SigMarks::reset(){
    for (int i=0; i<_m.count(); ++i){
        _m[i].reset();
    }
}

void SigMarks::clear(){
    this->_m.clear();
}

//Добавление всех записей из каталога
//int SigMarks::appendFromDir(const QString &path){
//
//}

void  SigMarks::remove(const QVector<int> &pos){
    QMap<int,bool> has;  //ассоц. карта N-есть/нет в списке
    for (int i=0; i<pos.count(); ++i){
        has.insert(pos[i],true);
    }
    QVector<SigMark> m;
    for (int i=0; i<_m.count(); ++i){   //Занести элементы, отсутствующие в pos
        if (!has.value(i,false))
            m.push_back(_m[i]);
    }
    _m = m;

}


QMap<int, QMap<int,int> > SigMarks::stats() const {
    //Сбор статистики по классам
    QMap<int, QMap<int,int> > r;
    for (int i=0; i<_m.count(); ++i){
        int lb0 = _m[i]._lb[0];
        int lb1 = _m[i]._lb[1];
        if (!r.contains(lb0)){  //не было такого элемента
            r[lb0] = QMap<int,int>();
            r[lb0][lb1] = 1;
            continue;
        }
        QMap<int,int> &r2 = r[lb0];
        if (!r2.contains(lb1)){
            r2[lb1] = 1;
            continue;
        }
        r2[lb1]++;
    }
    return r;
}


SigMarks SigMarks::slice(const VecI32 &pos) const {
    SigMarks w;
    w._c = _c;
    w._m.reserve(pos.count());
    foreach(int i, pos) w._m.push_back(_m[i]);
    return w;
}



VecI32 SigMarks::select(const SelRule &ruleY, const SelRule &rule0, const SelRule &rule1, const CompareOperation &rule10, const QVector<int> *pos) const{
    VecI32 to;
    if (pos==0){  //Найти среди всех элементов
        to.reserve(count());
        for (int i=0; i<count(); ++i){
            if (!ruleY.check(_m[i]._y))      continue;
            if (!rule0.check(_m[i]._lb[0])) continue;
            if (!rule1.check(_m[i]._lb[1])) continue;

            if (rule10!=AnyValue && !SelRule(rule10,_m[i]._lb[1]).check(_m[i]._lb[0]))
                continue;
            to.push_back(i);
        }
    } else {    //Найти только среди ранее выбранных
        to.reserve(pos->count());
        for (int i=0; i<pos->count(); ++i)
            if (ruleY.check(_m[pos->at(i)]._y) &&
                rule0.check(_m[pos->at(i)]._lb[0]) &&
                rule1.check(_m[pos->at(i)]._lb[1])){

                if (rule10==AnyValue || SelRule(rule10,_m[pos->at(i)]._lb[1]).check(_m[pos->at(i)]._lb[0]))
                    to.push_back(pos->at(i));

            }
    }
    return to;
}



VecI32 SigMarks::select(int from, int count, int step, const VecI32 *pos) const {
    VecI32 id;
    int c = this->count();
    if (from<0) { //можно задавать смещение от конца
        if (pos) from+=pos->size();
        else     from+=c;
    }
    if (count<0) {//можно задавать число элементов от всего количества
        if (pos) count+=1+pos->size();
        else     count+=1+c;
    }
    id.reserve(count);

    for (int i=0; i<count; ++i) {
        int k = (pos)? pos->at(from+i*step): from+i*step;
        if (k<0 || k>=c) continue; //недопустимое значение индекса
        id.push_back(k);
    }
    return id;
}




QVector<VecF32*> SigMarks::x(){
    QVector<VecF32*> v;
    v.reserve(_m.count());
    foreach(const SigMark &m, _m)
        v.push_back((VecF32*)m.x_ptr());
    return v;
}

QVector<VecF32*> SigMarks::x(const VecI32 &pos) {
    QVector<VecF32*> v;
    v.reserve(pos.count());
    foreach(int i, pos) v.push_back((VecF32*)_m[i].x_ptr());
    return v;
}

QVector<VecF32*> SigMarks::y(){
    QVector<VecF32*> v;
    v.reserve(_m.count());
    for (int i=0; i<_m.count(); ++i)
        v.push_back(&_m[i]._y);
    return v;
}

QVector<VecF32*> SigMarks::y(const VecI32 &pos){
    QVector<VecF32*> v;
    v.reserve(pos.count());
    for (int i=0; i<pos.count(); ++i)
        v.push_back(&_m[pos[i]]._y);
    return v;
}



VecI32  SigMarks::label(int id) const{    //вектор меток с заданным индексом (id = {0,1})
    Q_ASSERT(id>=0 && id<2);
    VecI32 v;
    v.reserve(_m.count());
    foreach(const SigMark& m, _m) v.push_back(m._lb[id]);
    return v;
}

VecI32  SigMarks::label(int id, const VecI32 &pos) const{    //вектор меток с заданным индексом (id = {0,1})
    Q_ASSERT(id>=0 && id<2);
    VecI32 v;
    v.reserve(_m.count());
    foreach(int i, pos) v.push_back(_m[i]._lb[id]);
    return v;
}

void SigMarks::mark(int id, const VecI32 &pos, const VecI32 &val){    //установка меток
    Q_ASSERT(id>=0 && id<2);
    Q_ASSERT(pos.count()==val.count());
    for (int i=0; i<val.count(); ++i) _m[pos[i]]._lb[id]=val[i];
}

void SigMarks::mark(int id, const VecI32 &val){                       //установка меток
    Q_ASSERT(id>=0 && id<2);
    Q_ASSERT(val.count()==_m.count());
    for (int i=0; i<val.count(); ++i) _m[i]._lb[id]=val[i];
}




















