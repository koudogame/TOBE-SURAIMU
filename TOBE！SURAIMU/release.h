#pragma once

#define SAFE_RELEASE( x )       { if(x) {x->Release(); x = NULL;}}
#define SAFE_DESTROY( x )       { if(x) {x->destroy(); x = NULL;}}

#define SAFE_DELETE( x )        { if(x) {delete x; x = NULL;}}
#define SAFE_DELETE_ARRAY( x )  { if(x) {delete[] x; x = NULL;}}


template <typename T>
inline void safe_delete( T*& p )
{
    typedef char def_check[ sizeof( T ) ? 1 : -1 ];

    delete p;
    p = nullptr;
}


template <typename T>
inline void safe_delete_array( T*& p_array )
{
    typedef char def_check[ sizeof( T ) ? 1 : -1 ];


    delete[] p_array;
    p_array = nullptr;
}