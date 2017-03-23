/*
 * Copyright 2010-2016, Tarantool AUTHORS, please see AUTHORS file.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "trivia/config.h"
#include "trivia/util.h"
#include "say.h"
#include "errinj.h"

#define ERRINJ_MEMBER(n, t, s) { /* .name = */ #n, /* .type = */ t, /* .state = */ s },

struct errinj errinjs[errinj_enum_MAX] = {
	ERRINJ_LIST(ERRINJ_MEMBER)
};

struct errinj *
errinj_lookup(char *name)
{
	int i;
	for (i = 0 ; i < errinj_enum_MAX ; i++) {
		if (strcmp(errinjs[i].name, name) == 0)
			return &errinjs[i];
	}
	return NULL;
}

/**
 * Get state of the error injection handle by id.
 *
 * @param id error injection id.
 *
 * @return error injection handle state.
 */
bool
errinj_getb(int id)
{
	assert(id >= 0 && id < errinj_enum_MAX);
	assert(errinjs[id].type == ERRINJ_BOOL);
	return errinjs[id].state.bparam;
}

uint64_t
errinj_getu64(int id)
{
	assert(id >= 0 && id < errinj_enum_MAX);
	assert(errinjs[id].type == ERRINJ_U64);
	return errinjs[id].state.u64param;
}

/**
 * Set state of the error injection handle by id.
 *
 * @param id error injection id.
 * @param state error injection handle state.
 *
 */
void
errinj_setb(int id, bool state)
{
	assert(id >= 0 && id < errinj_enum_MAX);
	assert(errinjs[id].type == ERRINJ_BOOL);
	errinjs[id].state.bparam = state;
}

void
errinj_setu64(int id, uint64_t state)
{
	assert(id >= 0 && id < errinj_enum_MAX);
	assert(errinjs[id].type == ERRINJ_U64);
	errinjs[id].state.u64param = state;
}

/**
 * Set state of the error injection handle by name.
 *
 * @param name error injection name.
 * @param state error injection handle state.
 *
 * @return 0 on success, -1 if injection was not found.
 */
int
errinj_setb_byname(char *name, bool state)
{
	struct errinj *ei = errinj_lookup(name);
	if (ei == NULL)
		return -1;
	assert(ei->type == ERRINJ_BOOL);
	ei->state.bparam = state;
	return 0;
}

/**
 * Dump error injection states to the callback function.
 */
int errinj_foreach(errinj_cb cb, void *cb_ctx) {
	int i;
	for (i = 0 ; i < errinj_enum_MAX ; i++) {
		int res = cb(&errinjs[i], cb_ctx);
		if (res != 0)
			return res;
	}
	return 0;
}