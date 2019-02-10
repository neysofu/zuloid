/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef Z64C_H
#define Z64C_H

/* A fully self-contained Z64C instance. */
struct Engine;

/* Creates a self-contained engine instance with default settings.
 *
 * Result
 *  NULL on failure; a valid pointer on success. */
struct Engine *
engine_new(void);

/* Gracefully kills `engine`. No shutdown JSON-RPC Request is needed. */
void
engine_delete(struct Engine *engine);

/* Runs a "Remote Procedure Call" (RPC) and returns the engine's response.
 * All communication must be in valid JSON-RPC 2.0.
 *
 * Result
 *   A JSON-RPC response string without line breaks. */
char *
engine_call(struct Engine *engine, const char *request);

/*
 * */
const int *
engine_exit_status(const struct Engine *engine);

#endif
