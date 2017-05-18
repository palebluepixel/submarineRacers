#ifndef _MOD_HXX_
#define _MOD_HXX_

/* Modular indexing that allows for a negative input index */
inline int modularIndexOffset(int i, int off, int n) {
    int __index__ = i + off; 
    int __negflag__ = 0; 
    if(__index__ < 0){ 
        __index__ = -__index__; 
        __negflag__ = 1; 
    } 
    int __offset__ = __index__ % n; 
    return (__negflag__) ? n - __offset__ : __offset__;
}

#endif //!_MOD_HXX_