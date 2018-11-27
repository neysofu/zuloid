/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file implements the external APIs to the engine over stdin. */

#include "cJSON/cJSON.h"
#include "engine.h"

#define LITERAL_AUTHORS ("author(s)")
#define LITERAL_CODE ("code")
#define LITERAL_DATA ("data")
#define LITERAL_ERROR ("error")
#define LITERAL_GAME ("game")
#define LITERAL_ID ("id")
#define LITERAL_JSONRPC ("jsonrpc")
#define LITERAL_KEY ("key")
#define LITERAL_LICENSE ("license")
#define LITERAL_MESSAGE ("message")
#define LITERAL_META ("meta")
#define LITERAL_METHOD ("method")
#define LITERAL_NAME ("name")
#define LITERAL_NOTATION ("notation")
#define LITERAL_PARAMS ("params")
#define LITERAL_PATH ("path")
#define LITERAL_RELEASE_DATE ("release-date")
#define LITERAL_RESULT ("result")
#define LITERAL_TREE ("tree")
#define LITERAL_TWO_POINT_OH ("2.0")
#define LITERAL_URL ("url")
#define LITERAL_VALUE ("value")
#define LITERAL_VERSION ("version")

#define JSONRPC_CHESS_ERROR_CODE (150)
#define JSONRPC_CHESS_ERROR_MSG ("Invalid notation.")
#define JSONRPC_INVALID_METHOD_CODE (-32601)
#define JSONRPC_INVALID_METHOD_MSG ("Method not found.")
#define JSONRPC_INVALID_PARAMS_CODE (-32602)
#define JSONRPC_INVALID_PARAMS_MSG ("Invalid parameters.")
#define JSONRPC_INVALID_REQUEST_CODE (-32600)
#define JSONRPC_INVALID_REQUEST_MSG ("Invalid request.")
#define JSONRPC_MODE_ERROR_CODE (100)
#define JSONRPC_MODE_ERROR_MSG ("This method is not available during search.")
#define JSONRPC_PARSE_ERROR_CODE (-32700)
#define JSONRPC_PARSE_ERROR_MSG ("Parse error.")
#define JSONRPC_OOM_CODE (-32042)
#define JSONRPC_OOM_MSG ("Not enough available memory to complete your request.")

struct cJSON *
engine_rpc_load(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_perft(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_get(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_exit(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_status(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_save(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_search(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_set(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_setup(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_train(struct Engine *engine, const struct cJSON *params);
struct cJSON *
engine_rpc_uci(struct Engine *engine, const struct cJSON *params);

const char *
engine_rpc(struct Engine *engine, const char *cmd);

const struct cJSON *
engine_rpc_cjson(struct Engine *engine, const struct cJSON *request);
