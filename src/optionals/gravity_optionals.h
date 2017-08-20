//
//  gravity_optionals.h
//  gravity
//
//  Created by Marco Bambini on 14/08/2017.
//  Copyright © 2017 CreoLabs. All rights reserved.
//

#ifndef __GRAVITY_OPTIONALS__
#define __GRAVITY_OPTIONALS__

#ifndef GRAVITY_INCLUDE_MATH
#define GRAVITY_INCLUDE_MATH
#endif // GRAVITY_INCLUDE_MATH

#ifdef GRAVITY_INCLUDE_MATH
#define GRAVITY_MATH_REGISTER(_vm)          gravity_math_register(_vm)
#define GRAVITY_MATH_FREE()                 gravity_math_free()
#define GRAVITY_MATH_NAME()                 gravity_math_name()
#define GRAVITY_ISMATH_CLASS(_c)            gravity_ismath_class(_c)
#include "gravity_math.h"
#else
#define GRAVITY_MATH_REGISTER(_vm)
#define GRAVITY_MATH_FREE()
#define GRAVITY_MATH_NAME()                 NULL
#define GRAVITY_ISMATH_CLASS(_c)            false
#endif // GRAVITY_INCLUDE_MATH

#endif // __GRAVITY_OPTIONALS__
