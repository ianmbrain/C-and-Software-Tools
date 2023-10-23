// Contains Main()
// You can define the comparison functions in your top-level parks component and make them static

#include <catalog.h>

static int idComp( void const *va, void const *vb )
{
    Park const *p1 = va;
    Park const *p2 = vb;

    if ( p1->id < p2->id ) {
        return -1;
    }
    else if ( p1->id < p2->id ) {
        return 1;
    }
    else {
        return 0;
    }   
}

static int idComp( void const *va, void const *vb )
{
    Park const *p1 = va;
    Park const *p2 = vb;

    int str_cmp = strcmp( p1->name, p2->name );
    if ( str_cmp == 0 ) {
        return idComp( va, vb );
    }
    else {
        return str_cmp;
    }
}
