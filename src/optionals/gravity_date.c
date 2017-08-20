//
//  gravity_date.c
//  gravity
//
//  Created by Marco Bambini on 19/08/2017.
//  Copyright © 2017 CreoLabs. All rights reserved.
//

#include <math.h>
#include <time.h>
#include <inttypes.h>
#include "gravity_vm.h"
#include "gravity_date.h"
#include "gravity_core.h"
#include "gravity_hash.h"
#include "gravity_macros.h"
#include "gravity_vmmacros.h"

#define DATE_CLASS_NAME             "Date"

gravity_class_t                     *gravity_class_date = NULL;
static uint32_t                     refcount = 0;

static const char *day_names[7]           = {"Sunday", "Monday", "Tuesday",
                                             "Wednesday", "Thursday", "Friday",
                                             "Saturday"};
static const char *abbrev_day_names[7]    = {"Sun", "Mon", "Tues", "Wed",
                                             "Thurs", "Fri", "Sat"};
static const char *month_names[12]        = {"January", "February", "March",
                                             "April", "May", "June", "July",
                                             "August", "September", "October"
                                             "November", "December"};
static const char *abbrev_month_names[12] = {"Jan", "Feb", "Mar", "Apr", "May",
                                             "June", "July", "Aug", "Sept",
                                             "Oct", "Nov", "Dec"};
// MARK: - Implementation -

static bool date_now (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    int t = (int)time(NULL);
    RETURN_VALUE(VALUE_FROM_INT(t), rindex);
}

static bool date_second (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_sec;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_sec;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_minute (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_min;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_min;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_hour (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_hour;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_hour;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_12hour (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_hour;
        if (tm > 12) {
            tm -= 12;
        }
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_hour;
        if (tm > 12) {
            tm -= 12;
        }
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_month_day (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_mday;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_mday;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_month (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1 because tm_mon is a base 0 number
        int tm = localtime(&t)->tm_mon + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_mon + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_month_name (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1 because tm_mon is a base 0 number
        int tm = localtime(&t)->tm_mon;
        uint32_t len = strlen(month_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, month_names[tm], len), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_mon;
        uint32_t len = strlen(month_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, month_names[tm], len), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_month_name_abbreviated (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1 because tm_mon is a base 0 number
        int tm = localtime(&t)->tm_mon;
        uint32_t len = strlen(abbrev_month_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, abbrev_month_names[tm], len), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_mon;
        uint32_t len = strlen(abbrev_month_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, abbrev_month_names[tm], len), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_year (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1900 because tm_year is the number of years since 1900
        int tm = localtime(&t)->tm_year + 1900;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_year + 1900;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_week_day (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1 because tm_wday is a base 0 number
        int tm = localtime(&t)->tm_wday + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_wday + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_week_day_name (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_wday;
        uint32_t len = strlen(day_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, day_names[tm], len), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_wday;
        uint32_t len = strlen(day_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, day_names[tm], len), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_week_day_name_abbreviated (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        int tm = localtime(&t)->tm_wday;
        uint32_t len = strlen(abbrev_day_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, abbrev_day_names[tm], len), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_wday;
        uint32_t len = strlen(abbrev_day_names[tm]);
        RETURN_VALUE(VALUE_FROM_STRING(vm, abbrev_day_names[tm], len), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

static bool date_year_day (gravity_vm *vm, gravity_value_t *args, uint16_t nargs, uint32_t rindex) {
    #pragma unused(args,nargs)
    gravity_value_t value = GET_VALUE(1);
    if (nargs == 1) {
        time_t t = time(NULL);
        // Plus 1 because tm_yday is a base 0 number
        int tm = localtime(&t)->tm_yday + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    if (VALUE_ISA_INT(value)) {
        int tm = localtime(&value.n)->tm_yday + 1;
        RETURN_VALUE(VALUE_FROM_INT(tm), rindex);
    }
    // should be NaN
    RETURN_VALUE(VALUE_FROM_UNDEFINED, rindex);
}

// MARK: - Internals -

static void create_optional_class (void) {
    gravity_class_date = gravity_class_new_pair(NULL, DATE_CLASS_NAME, NULL, 0, 0);
    gravity_class_t *meta = gravity_class_get_meta(gravity_class_date);
    gravity_class_bind(meta, "now", NEW_CLOSURE_VALUE(date_now));
    gravity_class_bind(meta, "seconds", NEW_CLOSURE_VALUE(date_second));
    gravity_class_bind(meta, "minutes", NEW_CLOSURE_VALUE(date_minute));
    gravity_class_bind(meta, "hours", NEW_CLOSURE_VALUE(date_hour));
    gravity_class_bind(meta, "hours_12", NEW_CLOSURE_VALUE(date_12hour));
    gravity_class_bind(meta, "day", NEW_CLOSURE_VALUE(date_month_day));
    gravity_class_bind(meta, "month", NEW_CLOSURE_VALUE(date_month));
    gravity_class_bind(meta, "month_name_short", NEW_CLOSURE_VALUE(date_month_name_abbreviated));
    gravity_class_bind(meta, "month_name", NEW_CLOSURE_VALUE(date_month_name));
    gravity_class_bind(meta, "year", NEW_CLOSURE_VALUE(date_year));
    gravity_class_bind(meta, "dow", NEW_CLOSURE_VALUE(date_week_day));
    gravity_class_bind(meta, "dow_name_short", NEW_CLOSURE_VALUE(date_week_day_name_abbreviated));
    gravity_class_bind(meta, "dow_name", NEW_CLOSURE_VALUE(date_week_day_name));
    gravity_class_bind(meta, "doy", NEW_CLOSURE_VALUE(date_year_day));

    SETMETA_INITED(gravity_class_date);
}

// MARK: - Commons -

bool gravity_isdate_class (gravity_class_t *c) {
    return (c == gravity_class_date);
}

const char *gravity_date_name (void) {
    return DATE_CLASS_NAME;
}

void gravity_date_register (gravity_vm *vm) {
    if (!gravity_class_date) create_optional_class();
    ++refcount;

    if (!vm || gravity_vm_ismini(vm)) return;
    gravity_vm_setvalue(vm, DATE_CLASS_NAME, VALUE_FROM_OBJECT(gravity_class_date));
}

void gravity_date_free (void) {
    if (!gravity_class_date) return;

    --refcount;
    if (refcount) return;

    mem_check(false);
    gravity_class_t *meta = gravity_class_get_meta(gravity_class_date);
    gravity_class_free_core(NULL, meta);
    gravity_class_free_core(NULL, gravity_class_date);
    mem_check(true);

    gravity_class_date = NULL;
}
