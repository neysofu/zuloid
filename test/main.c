/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"

extern void test_fen(void);
extern void test_jsonrpc_exit(void);
extern void test_jsonrpc_init(void);
extern void test_jsonrpc_no_response_from_empty_request(void);
extern void test_jsonrpc_no_response_from_notification(void);
extern void test_jsonrpc_non_null_response(void);
extern void test_jsonrpc_set_then_get(void);
extern void test_jsonrpc_setup(void);
extern void test_jsonrpc_status(void);
extern void test_utils(void);

int
main(void)
{
    UNITY_BEGIN();
	RUN_TEST(test_fen);
	RUN_TEST(test_jsonrpc_exit);
	RUN_TEST(test_jsonrpc_init);
	RUN_TEST(test_jsonrpc_no_response_from_empty_request);
	RUN_TEST(test_jsonrpc_no_response_from_notification);
	RUN_TEST(test_jsonrpc_non_null_response);
	RUN_TEST(test_jsonrpc_set_then_get);
	//RUN_TEST(test_jsonrpc_setup);
	RUN_TEST(test_jsonrpc_status);
	RUN_TEST(test_utils);
    return UNITY_END();
}
