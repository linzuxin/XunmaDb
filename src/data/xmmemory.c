#include "xmmemory.h"

blockprofile* initDefBlockProfile(){
    blockprofile* profile = NULL;
    xmnew0(profile);
    if (profile){
        profile->defbyte[0] = 2;
        profile->defbyte[1] = 2;
        profile->defbyte[2] = 2;
        profile->defbyte[3] = 2;

        profile->sizebyte[0] = 2;
        profile->sizebyte[1] = 2;
        profile->sizebyte[2] = 2;
        profile->sizebyte[3] = 2;
    }
    return profile;
}
