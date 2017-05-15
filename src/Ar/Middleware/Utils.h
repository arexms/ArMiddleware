#pragma once

namespace Ar { namespace Middleware
{
    template < typename T >
    T* safeNew( T *&ptr )
    {
        ptr = new T;
        return ptr;
    }

	/*template < typename T, typename Arg1 >
    T* safeNew( T *&ptr, Arg1 &arg1 )
    {
        ptr = new T( arg1 );
        return ptr;
    }*/

    template < typename T, typename Arg1 >
    T* safeNewWith1Arg( T *&ptr, Arg1 &arg1 )
    {
        ptr = new T( arg1 );
        return ptr;
    }

    template < typename T, typename Arg1 >
    T* safeNewWith1Arg(Arg1 arg1)
    {
        T *ptr = new T(arg1);
        return ptr;
    }

    template < typename T, typename Arg1 >
    T* safeNew(Arg1 &arg1)
    {
        T *ptr = new T(arg1);
        return ptr;
    }

    template < typename T >
    T* safeNew()
    {
        return new T;
    }

    template < typename T >
    T* safeNewArray( T *&ptr, unsigned size )
    {
        ptr = new T[ size ];
        return ptr;
    }

    template < typename T >
    T* safeNewArray( unsigned size )
    {
        return new T[ size ];
    }

    template < typename T >
    T* safeDel( T *&ptr )
    {
        delete ptr;
        ptr = nullptr;
        return ptr;
    }

    template < typename T >
    T* safeDelArray( T *&ptr )
    {
        delete []ptr;
        ptr = nullptr;
        return ptr;
    }
} }
