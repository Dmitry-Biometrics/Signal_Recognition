#ifndef SELRULE
#define SELRULE

#include <QVector>
#include "sigdata.h"

/*
class Rule {
public:

    enum Op {
        Null,
        AnyValue,
        Equal,
        Less,
        Great,
        LessOrEqual,
        GreatOrEqual,
        NotEqual,
        InRange,
        OutRange,
        InSet,
        NotInSet
    };

    enum RefType {
        YData,
        XData,
        Label0,
        Label1
    };

    virtual ~Rule(){}
    virtual bool set(const SigMark& m)=0;
    virtual bool compare()=0;
};

class Rule1i: public Rule {
public:
    Rule1i(Rule::RefType what, Rule::Op op, int value):_rt(what),_op(op),_y(1,value){}
    Rule1i(Rule::RefType what, Rule::Op op, const QVector<int> &value):_rt(what),_op(op),_y(value){}

    bool set(int value){ _y.resize(1); _y[0]=value; return true; }
    bool set(QVector<int> &value){ _y=value; return true; }

    virtual bool set(const SigMark& m){
        switch (_rt){
        case Label0: _x = m.label0; break;
        case Label1: _x = m.label1; break;
        default: return false;
        }
    }

    virtual bool compare(){
        switch (_op){
        case Null:          return false;
        case AnyValue:      return true;
        case Equal:         return _x==_y[0];
        case Less:          return _x<_y[0];
        case Great:         return _x>_y[0];
        case LessOrEqual:   return _x<=_y[0];
        case GreatOrEqual:  return _x>=_y[0];
        case NotEqual:      return _x!=_y[0];
        case InRange:       return (_x>=_y[0] && _x<=_y[1]);
        case OutRange:      return (_x<_y[0] || _x>_y[1]);
        case InSet:         for (int i=0; i<_y.size(); ++i)
                                if (_x==_y[i]) return true;
                            return false;
        case NotInSet:      for (int i=0; i<_y.size(); ++i)
                                if (_x==_y[i]) return false;
                            return true;
        default: return false;
        }
    }

protected:
    RefType _rt;
    Op _op;
    int _x;
    QVector<int> _y;
};

class Rule2i:public Rule {
public:
    Rule2i(Rule::RefType x, Rule::Op op, Rule::RefType y):_rt1(x),_op(op),_rt2(y){}
    Rule2i(Rule::RefType x, Rule::Op op, Rule::RefType y):_rt1(x),_op(op),_rt2(y){}

    virtual bool set(const SigMark& m){
        switch (_rt1){
        case Label0: _x = m.label0; break;
        case Label1: _x = m.label1; break;
        default: return false;
        }
        switch (_rt2){
        case Label0: _y = m.label0; break;
        case Label1: _y = m.label1; break;
        default: return false;
        }
    }

    virtual bool compare(){
        switch (_op){
        case Null:          return false;
        case AnyValue:      return true;
        case Equal:         return _x==_y;
        case Less:          return _x<_y;
        case Great:         return _x>_y;
        case LessOrEqual:   return _x<=_y;
        case GreatOrEqual:  return _x>=_y;
        case NotEqual:      return _x!=_y;
        case InRange:
        case OutRange:
        case InSet:
        case NotInSet:
        default: return false;
        }
    }

protected:
    RefType _rt1,  _rt2;
    Op _op;
    int _x, _y;
};


class RuleNot: public Rule {
public:
    RuleNot(Rule* rule):_rule(rule){}
    virtual ~RuleNot(){ if (_rule) delete _rule; }
    virtual bool set(const SigMark &m){ return _rule->set(m); }
    virtual bool compare(){ return !_rule->compare(); }
protected:
    Rule *_rule;
};

class RuleAnd:public Rule {
public:
    RuleAnd(){}
    RuleAnd(Rule* rule, Rule* rule2){ _rule.push_back(rule); _rule.push_back(rule);}
    virtual ~RuleAnd(){ for (int i=0; i<_rule.count(); ++i) delete _rule[i]; }
    void add(Rule* rule){ _rule.push_back(rule); }

    virtual bool set(const SigMark &m){
        for (int i=0; i<_rule.count(); ++i)
            if (!_rule[i]->set(m)) return false;
        return true;
    }
    virtual bool compare(){
        for (int i=0; i<_rule.count(); ++i)
            if (!_rule[i]->compare()) return false;
        return true;
    }
protected:
    QVector<Rule*> _rule;
};


class RuleOr:public RuleAnd {
public:
    RuleOr(){}
    RuleOr(Rule* rule, Rule* rule2):RuleAnd(rule1,rule2){}
    virtual bool compare(){
        for (int i=0; i<_rule.count(); ++i)
            if (_rule[i]->compare()) return true;
        return false;
    }
};

*/

#endif // SELRULE

