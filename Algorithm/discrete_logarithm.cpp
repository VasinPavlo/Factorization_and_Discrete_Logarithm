#include "discrete_logarithm.h"
#define qD QString("discrete_logarithm.cpp:")
DiscreteLogarithm::DiscreteLogarithm()
{
}

LongInt DiscreteLogarithm::Primitive(LongInt g,LongInt b,LongInt n)
{
    if(g>=n||b>=n)
    {
        return (-LongInt(0));
    }
    LongInt _g=1;
    LongInt k=0;
    while(1)
    {
        _g*=g;
        _g=_g%n;
        ++k;
        if(_g==b)
        {
            return k;
        }
        if(_g==1)
        {
            return (-LongInt(0));
        }
    }
}

LongInt DiscreteLogarithm::Shank(LongInt g,LongInt b,LongInt n)
{
    if(g>=n||b>=n)
    {
        return (-LongInt(0));
    }
    qDebug()<<n<<(Algorithm::Agrawal_Kayal_Saxena(n)?"prime":"");
    LongInt _g(1),g_a,i(1),j,begin;
    LongInt a=LongInt::Sqrt(n-1)+1;
    qDebug()<<"a="<<a;
    QVector<LongInt> L_1,L_2;
    L_1<<1;
    //L_2<<b;
    qDebug()<< (g_a=Modular_exponentiation(g,n,a));
    //g_a=(g^a)%n;

    for(;i<a;++i)
    {
        _g*=g_a;
        _g=_g%n;
        if(_g==1)
            break;
        L_1<<_g;
    }
    _g=b;
    begin=_g;
    qDebug()<<L_1;
    for(j=0;j<L_1.length();++j)
    {
        qDebug()<<L_1[j.toInt()]<<"?"<<_g;
        if(L_1[j.toInt()]==_g)
        {
            qDebug()<<"Test:"<<g<<"^"<<(a*j)<<" mod"<<n<<"="<<((g^(a*j ))%n)<<"=="<<b;
            return (a*j );
        }
    }
    for(i=1;i<a;++i)
    {
        _g*=g;
        _g=_g%n;
        //L_2<<_g;
        qDebug()<<"_g="<<_g;
        if(_g==begin)
        {
            return (-LongInt(0));
        }
        for(j=0;j<L_1.length();++j)
        {
            qDebug()<<L_1[j.toInt()]<<"?"<<_g;
            if(L_1[j.toInt()]==_g)
            {
                qDebug()<<"Test:"<<g<<"^"<<(a*j - i)<<" mod"<<n<<"="<<((g^(a*j - i))%n)<<"=="<<b;
                return (a*j - i);
            }
        }
    }
    qDebug()<<L_1;
    return -LongInt(0);
}
//================================Pollard============================
//#define mod_n(n) (n-1)
LongInt DiscreteLogarithm::Pollard(LongInt g,LongInt b,LongInt n)
{
    //qDebug()<<"mod_n(n):"<<mod_n(n);
    LongInt u,U,v,V,z(1),Z(1);
    LongInt i(0);
    new_iter(u,v,z,&g,&b,&n);
    u=0;v=0;z=1;
    qDebug()<<"start";
    while(1)
    {
        ++i;
        qDebug()<<"i:"<<i;
        new_iter(u,v,z);
        new_iter(U,V,Z);//qDebug()<<"Z:"<<Z<<V<<U<<"|";
        new_iter(U,V,Z);qDebug()<<" z:"<<z<<v<<u<<" Z:"<<Z<<V<<U<<"|";

        if(z==Z)
        {
            //qDebug()<<z<<"=="<<Z;
            //qDebug()<<"HCD:"<<(HCD(U-u,n-1));
            //qDebug()<<i<<"result:"<<(Modular_Multiplicative_Inverse(U-u,n)*(+(v-V)))%(n);
            //if(HCD(U-u,n-1)==1)
            {
                //qDebug()<<n;
                //qDebug()<<Modular_Multiplicative_Inverse(38,1019)<<(+(v-V));
                return (Modular_Multiplicative_Inverse(+(U-u),n)*(+(v-V)))%(n);
            }
        }
        if(i==(n-1))
            return -LongInt(0);
    }
}

void DiscreteLogarithm::new_iter(LongInt &u, LongInt &v, LongInt& z,const LongInt* a, const LongInt* b, const LongInt* mod)
{
    static const LongInt* _mod;
    static const LongInt* _a;
    static const LongInt* _b;
    //
    if(mod!=NULL)
    {
        _mod=mod;
        //qDebug()<<"discrete_logarithm.cpp:_mod="<<*_mod;
    }
    if(a!=NULL)
    {
        _a=a;
        //qDebug()<<"discrete_logarithm.cpp:_a="<<*_a;
    }
    if(b!=NULL)
    {
        _b=b;
        //qDebug()<<"discrete_logarithm.cpp:_b="<<*_b;
    }
    switch((z%3).toInt())
    {
    case 0:
        u=(u*2)%((*_mod)-1);
        v=(v*2)%((*_mod)-1);
        //qDebug()<<"case 0:"<<z;
        //qDebug()<<"case 0: result:"<<
                  (z=(z^2)%(*_mod));
        break;
    case 1:
        v=(v+1)%((*_mod)-1);
        //qDebug()<<"case 1:"<<z;
        //qDebug()<<"case 1: result:"<<
                  (z=((*_a)*z)%(*_mod));
        break;
    case 2:
        u=(u+1)%((*_mod)-1);
        //qDebug()<<"z:"<<z;
        //qDebug()<<"case 2:"<<z;//<<(*_b*z)%(*_mod);
        //qDebug()<<"case 2: result:"<<
                  (z=((*_b)*z)%(*_mod));
        break;
    }
}

//================================Pollard============================


LongInt DiscreteLogarithm::Index(LongInt g,LongInt b,LongInt n)
{
    QVector<LongInt> Base;
    QVector<LongInt>::const_iterator iter;
    static const int C=100;
    LongInt d=1,i=2,rand;
    LongInt inverse;
    const LongInt _n=(Agrawal_Kayal_Saxena(n)?n-1:Algorithm::Eulers_totient(n));
    qDebug()<<qD<<"Eulers totient="<<_n;

    int j,k;
    while(d<n)
    {
        if(Agrawal_Kayal_Saxena(i))
        {
            Base<<i;
            d*=i;
        }
        ++i;
    }
    qDebug()<<qD+"Base-"<<Base;
    QVector<QVector<LongInt>> matrix(C+Base.length(),QVector<LongInt>(Base.length()+1));
    QVector<LongInt> result(Base.length()+1);
    i=0;
    while(i.toInt()<matrix.length())
    {
        rand=Random(0,n-1);
        /*/
        static int test=0;
        switch(test)
        {
        case 0:
            rand =2;
            test++;
            break;
        case 1:
            rand =15;
            test++;
            break;
        case 2:
            rand =11;
            test++;
            break;
        }
        /*/
        d=Modular_exponentiation(g,n,rand);
        if(d==1)
            continue;
        for(iter=Base.constBegin(),j=0;iter<Base.constEnd();iter++)
        {
            //qDebug()<<qD<<d<<d%(*iter);
            while(d%(*iter)==0)
            {
                d/=(*iter);
                //qDebug()<<qD<<i<<j;
                matrix[i.toInt()][j]+=1;
            }
            j++;
        }
        if(d==1)
        {
            matrix[i.toInt()][Base.length()]=rand;
            ++i;
        }
    }
    /*/
    for(i=0;i<matrix.length();++i)
    {
        qDebug()<<qD<<matrix[i.toInt()];
    }
    qDebug()<<"----------------";
    /*/
    for(j=0;j<Base.length();++j)//стовбчик
        for(i=j;i<matrix.length();++i)//рядочок
        {
            if(matrix[i.toInt()][j]!=0)
            {
                if(i!=j)
                {
                    matrix.insert(j,matrix.takeAt(i.toInt()));//пересуваєм на потрібне місце
                    /*/
                    for(int _i=0;_i<matrix.length();++_i)
                    {
                        qDebug()<<qD<<matrix[_i];
                    }
                    qDebug()<<"----------------";
                    /*/
                }
                //------------зведення-(j,j)-елемента-до-1-------------------------
                d=matrix[j][j];
                if(d!=1)
                {
                    inverse=Modular_Multiplicative_Inverse(matrix[j][j],n);
                    //qDebug()<<qD+"inverse-"<<d<<inverse;
                    for(i=j;i<=Base.length();++i)
                    {
                        //qDebug()<<qD<<"( "<<j<<" ) ,"<<i<<"-"<<inverse<<
                        (matrix[j][i.toInt()]=(matrix[j][i.toInt()]*inverse)%(n));
                    }
                    /*/
                    for(int _i=0;_i<matrix.length();++_i)
                    {
                        qDebug()<<qD<<matrix[_i];
                    }
                    qDebug()<<"----------------";
                    /*/
                }
                //-------------------------------------
                for(i=0;i<matrix.length();++i)
                {
                    if(i!=j)
                    {
                        if(matrix[i.toInt()][j]!=0)
                        {
                            d=matrix[i.toInt()][j];
                            for(k=j;k<=Base.length();++k)
                            {
                                //qDebug()<<qD<<i<<", ( "<<k<<" ) -"<<matrix[i.toInt()][k]<<d*matrix[j][k]<<
                                (matrix[i.toInt()][k]=(matrix[i.toInt()][k]-d*matrix[j][k])%_n);
                                (matrix[i.toInt()][k]=((matrix[i.toInt()][k]+1<LongInt(1))?matrix[i.toInt()][k]+_n:matrix[i.toInt()][k]));
                            }
                            /*/
                            for(int _i=0;_i<matrix.length();++_i)
                            {
                                qDebug()<<qD<<matrix[_i];
                            }
                            qDebug()<<"----------------";
                            /*/
                        }
                    }
                }
                break;
            }
            /*/
            else
            {
                if(i==matrix.length()-1)
                {
                    qDebug()<<qD<<i<<j;
                    i=j;
                    int _j=0;
                    while(i<matrix.length())
                    {
                        rand=Random(0,n-1);
                        d=Modular_exponentiation(g,n,rand);
                        if(d==1)
                            continue;
                        for(iter=Base.constBegin(),_j=0;iter<Base.constEnd();iter++)
                        {
                            //qDebug()<<qD<<d<<d%(*iter);
                            static bool isEmpty=false;
                            if(isEmpty)
                            {
                                for(k=0;k<Base.length();k++)
                                {
                                    matrix[i.toInt()][k]=0;
                                }
                                isEmpty=1;
                            }
                            while(d%(*iter)==0)
                            {
                                d/=*iter;
                                matrix[i.toInt()][_j]+=1;
                            }
                            _j++;
                            isEmpty=1;
                        }
                        if(d==1)
                        {
                            matrix[i.toInt()][Base.length()]=rand;
                            ++i;
                        }
                    }
                    i=j;
                }
            }
                /*/
        };
    /*/
    for(i=0;i<matrix.length();++i)
    {
        qDebug()<<qD<<matrix[i.toInt()];
    }
    /*/
    while(1)
    {
        rand=Random(0,n-1);
        /*/
        static int test=0;
        switch(test)
        {
        case 0:
            rand =3;
            test++;
            break;
        case 1:
            rand =7;
            test++;
            break;
        case 2:
            rand =11;
            test++;
            break;
        }
        /*/
        d=(b*Modular_exponentiation(g,n,rand))%n;
        if(d==1)
        {
            continue;
        }
        for(iter=Base.constBegin(),j=0;iter<Base.constEnd();iter++)
        {
            //qDebug()<<qD<<d<<d%(*iter);
            while(d%(*iter)==0)
            {
                d/=(*iter);
                //qDebug()<<qD<<i<<j;
                result[j]+=1;
            }
            j++;
        }
        if(d==1)
        {
            result[Base.length()]=rand;
            break;
        }
    }
    qDebug()<<qD+"result-"<<result;
    d=0;
    for(j=0;j<Base.length();++j)
    {
        if(result[j]!=0)
        {
            d+=matrix[j][Base.length()]*result[j];
        }
    }
    d-=result[Base.length()];
    d=(d+1<LongInt(1)?d+_n:d);
    return d;
}
