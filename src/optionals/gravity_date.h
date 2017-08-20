//
//  gravity_date.h
//  gravity
//
//  Created by Marco Bambini on 19/08/2017.
//  Copyright © 2017 CreoLabs. All rights reserved.
//

#ifndef __GRAVITY_DATE__
#define __GRAVITY_DATE__

#include "gravity_value.h"

void gravity_date_register (gravity_vm *vm);
void gravity_date_free (void);
bool gravity_isdate_class (gravity_class_t *c);
const char *gravity_date_name (void);

#endif
