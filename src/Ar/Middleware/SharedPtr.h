#ifndef Ar_Middleware_SharedPtr_h__
#define Ar_Middleware_SharedPtr_h__

#include <Ar/Middleware/Utils.h>
#include <memory>

namespace Ar { namespace Middleware
{
    enum PtrType
    {
        PtrType_NO_ARRAY = 0,
        PtrType_ARRAY
    };

    template < typename T, PtrType = PtrType_NO_ARRAY >
    struct Deallocator { void operator()( T *&ptr ) { safeDel( ptr ); } };

    template < typename T >
    struct Deallocator< T, PtrType_ARRAY > { void operator()( T *&ptr ) { safeDelArray( ptr ); } };

    template < typename T, PtrType type = PtrType_NO_ARRAY >
    class SharedPtr
    {
    public:
        inline explicit SharedPtr( T *ptr = 0 );

        inline void operator=(T *ptr);

        inline T* get() const;

        inline T* operator->();

        inline void reset( T *ptr );

        inline bool isEmpty() const;

    protected:
        std::shared_ptr< T > _ptr;
    };

    // IMPLEMENTATION
    template < typename T, PtrType type >
    inline SharedPtr< T, type >::SharedPtr( T *ptr )
        : _ptr( ptr, Deallocator< T, type >() )
    {}

    template < typename T, PtrType type >
    inline void SharedPtr< T, type >::operator=(T *ptr)
    {
        _ptr = std::shared_ptr<T>(ptr);
    }

    template < typename T, PtrType type >
    inline T* SharedPtr< T, type >::get() const
    {
        return _ptr.get();
    }

    template < typename T, PtrType type >
    inline T* SharedPtr< T, type >::operator->()
    {
        return _ptr.get();
    }

    template < typename T, PtrType type >
    inline void SharedPtr< T, type >::reset( T *ptr )
    {
        _ptr.reset( ptr, Deallocator< T, type >() );
    }

    template < typename T, PtrType type >
    inline bool SharedPtr< T, type >::isEmpty() const
    {
        return _ptr.get()==nullptr;
    }
} }

#endif // Ar_Middleware_SharedPtr_h__
