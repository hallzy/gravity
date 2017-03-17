//
//  gravity_vm.h
//  gravity
//
//  Created by Marco Bambini on 11/11/14.
//  Copyright (c) 2014 CreoLabs. All rights reserved.
//

#ifndef __GRAVITY_VM__
#define __GRAVITY_VM__

#include "gravity_delegate.h"
#include "gravity_value.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*vm_filter_cb) (gravity_object_t *obj);
typedef void (*vm_transfer_cb) (gravity_vm *vm, gravity_object_t *obj);
typedef void (*vm_cleanup_cb) (gravity_vm *vm);

// Opaque VM struct
struct gravity_vm {
	gravity_hash_t		*context;							// context hash table
	gravity_delegate_t	*delegate;							// registered runtime delegate
	gravity_fiber_t		*fiber;								// current fiber
	void				*data;								// custom data optionally set by the user
	uint32_t			pc;									// program counter
	double				time;								// useful timer for the main function
	bool				aborted;							// set when VM has generated a runtime error
	
	// anonymous names
	uint32_t			nanon;								// counter for anonymous classes (used in object_bind)
	char				temp[64];							// temprary buffer used for anonymous names generator
	
	// callbacks
	vm_transfer_cb		transfer;							// function called each time a gravity_object_t is allocated
	vm_cleanup_cb		cleanup;							// function called when VM must be cleaned-up
	vm_filter_cb		filter;								// function called to filter objects in the cleanup process
	
	// garbage collector
	bool				gcenabled;							// flag to enable/disable garbage collector
	gravity_int_t		memallocated;						// total number of allocated memory
	gravity_object_t	*gchead;							// head of garbage collected objects
	gravity_int_t		gcminthreshold;						// minimum GC threshold size to avoid spending too much time in GC
	gravity_int_t		gcthreshold;						// memory required to trigger a GC
	gravity_float_t		gcratio;							// ratio used in automatic recomputation of the new gcthreshold value
	gravity_int_t		gccount;							// number of objects into GC
	gravity_object_r	graylist;							// array of collected objects while GC is in process (gray list)
	gravity_object_r	gcsave;								// array of temp objects that need to be saved from GC

	// date properties
	bool date_human_readable; // if true, months and days get text values. ex Saturday, January
	
	// internal stats fields
	#if GRAVITY_VM_STATS
	uint32_t			nfrealloc;							// to check how many frames reallocation occurred
	uint32_t			nsrealloc;							// to check how many stack reallocation occurred
	uint32_t			nstat[GRAVITY_LATEST_OPCODE];		// internal used to collect opcode usage stats
	double				tstat[GRAVITY_LATEST_OPCODE];		// internal used to collect microbenchmarks
	nanotime_t			t;									// internal timer
	#endif
};


gravity_vm			*gravity_vm_new (gravity_delegate_t *delegate);
gravity_vm			*gravity_vm_newmini (void);
void				gravity_vm_set_callbacks (gravity_vm *vm, vm_transfer_cb vm_transfer, vm_cleanup_cb vm_cleanup);
void				gravity_vm_free (gravity_vm *vm);
void				gravity_vm_reset (gravity_vm *vm);
bool				gravity_vm_runclosure (gravity_vm *vm, gravity_closure_t *closure, gravity_value_t selfvalue, gravity_value_t params[], uint16_t nparams);
bool				gravity_vm_runmain (gravity_vm *vm, gravity_closure_t *closure);
void				gravity_vm_loadclosure (gravity_vm *vm, gravity_closure_t *closure);
void				gravity_vm_setvalue (gravity_vm *vm, const char *key, gravity_value_t value);
gravity_value_t		gravity_vm_lookup (gravity_vm *vm, gravity_value_t key);
gravity_value_t		gravity_vm_getvalue (gravity_vm *vm, const char *key, uint32_t keylen);
double				gravity_vm_time (gravity_vm *vm);
gravity_value_t		gravity_vm_result (gravity_vm *vm);
gravity_delegate_t	*gravity_vm_delegate (gravity_vm *vm);
gravity_fiber_t		*gravity_vm_fiber (gravity_vm *vm);
void				gravity_vm_setfiber(gravity_vm* vm, gravity_fiber_t *fiber);
void				gravity_vm_seterror (gravity_vm *vm, const char *format, ...);
void				gravity_vm_seterror_string (gravity_vm* vm, const char *s);
bool				gravity_vm_ismini (gravity_vm *vm);
gravity_value_t		gravity_vm_keyindex (gravity_vm *vm, uint32_t index);
bool				gravity_vm_isaborted (gravity_vm *vm);
void				gravity_vm_setaborted (gravity_vm *vm);

void				gravity_gray_value (gravity_vm* vm, gravity_value_t v);
void				gravity_gray_object (gravity_vm* vm, gravity_object_t *obj);
void				gravity_gc_start (gravity_vm* vm);
void				gravity_gc_setenabled (gravity_vm* vm, bool enabled);
void				gravity_gc_push (gravity_vm *vm, gravity_object_t *obj);
void				gravity_gc_pop (gravity_vm *vm);

void				gravity_vm_transfer (gravity_vm* vm, gravity_object_t *obj);
void				gravity_vm_cleanup (gravity_vm* vm);
void				gravity_vm_filter (gravity_vm* vm, vm_filter_cb cleanup_filter);

gravity_closure_t	*gravity_vm_loadfile (gravity_vm *vm, const char *path);
gravity_closure_t	*gravity_vm_loadbuffer (gravity_vm *vm, const char *buffer, size_t len);
void				gravity_vm_initmodule (gravity_vm *vm, gravity_function_t *f);

gravity_closure_t	*gravity_vm_fastlookup (gravity_vm *vm, gravity_class_t *c, int index);
void				gravity_vm_setslot (gravity_vm *vm, gravity_value_t value, uint32_t index);
gravity_value_t		gravity_vm_getslot (gravity_vm *vm, uint32_t index);
void				gravity_vm_setdata (gravity_vm *vm, void *data);
void				*gravity_vm_getdata (gravity_vm *vm);
void				gravity_vm_memupdate (gravity_vm *vm, gravity_int_t value);

gravity_value_t		gravity_vm_get (gravity_vm *vm, const char *key);
bool				gravity_vm_set (gravity_vm *vm, const char *key, gravity_value_t value);
char				*gravity_vm_anonymous (gravity_vm *vm);

gravity_value_t		gravity_date_get (gravity_vm *vm, const char *key);
bool				gravity_date_set (gravity_vm *vm, const char *key, gravity_value_t value);

#ifdef __cplusplus
}
#endif

#endif

