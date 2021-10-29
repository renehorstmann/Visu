#include <pbase/rhc/alloc.h>
#include "visu/control/base.h"


void vu_control_kill(VuControl **self_ptr) {
    p_rhc_free(*self_ptr);
    *self_ptr = NULL;
}
