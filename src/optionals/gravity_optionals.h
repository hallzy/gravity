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

#ifndef GRAVITY_INCLUDE_DATE
#define GRAVITY_INCLUDE_DATE
#endif // GRAVITY_INCLUDE_DATE

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

#ifdef GRAVITY_INCLUDE_DATE
#define GRAVITY_DATE_REGISTER(_vm)          gravity_date_register(_vm)
#define GRAVITY_DATE_FREE()                 gravity_date_free()
#define GRAVITY_DATE_NAME()                 gravity_date_name()
#define GRAVITY_ISDATE_CLASS(_c)            gravity_isdate_class(_c)
#include "gravity_date.h"
#else
#define GRAVITY_DATE_REGISTER(_vm)
#define GRAVITY_DATE_FREE()
#define GRAVITY_DATE_NAME()                 NULL
#define GRAVITY_ISDATE_CLASS(_c)            false
#endif // GRAVITY_INCLUDE_DATE

#endif // __GRAVITY_OPTIONALS__
