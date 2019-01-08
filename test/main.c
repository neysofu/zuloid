/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "Unity/src/unity.h"

extern void test_string_is_whitespace_empty_string(void);
extern void test_string_is_whitespace_many_kinds_of_whitespace(void);
extern void test_string_is_whitespace_partially_whitespace(void);
extern void test_string_is_whitespace_final_non_whitespace_character(void);
extern void test_string_is_whitespace_number_sign(void);

extern void test_jsonrpc_error_response_to_unparsable_request(void);
extern void test_jsonrpc_error_response_to_invalid_request(void);
extern void test_jsonrpc_response_with_id_to_invalid_request_with_id(void);
extern void test_jsonrpc_no_response_to_invalid_request_with_no_id(void);
extern void test_jsonrpc_no_response_to_empty_request(void);

extern void test_method_init_metadata(void);
extern void test_method_init_reentrant(void);

int
main(void)
{
    UNITY_BEGIN();

	RUN_TEST(test_string_is_whitespace_empty_string);
	RUN_TEST(test_string_is_whitespace_many_kinds_of_whitespace);
	RUN_TEST(test_string_is_whitespace_partially_whitespace);
	RUN_TEST(test_string_is_whitespace_final_non_whitespace_character);
	RUN_TEST(test_string_is_whitespace_number_sign);

	RUN_TEST(test_jsonrpc_error_response_to_unparsable_request);
	RUN_TEST(test_jsonrpc_error_response_to_invalid_request);
	RUN_TEST(test_jsonrpc_response_with_id_to_invalid_request_with_id);
	RUN_TEST(test_jsonrpc_no_response_to_invalid_request_with_no_id);
	RUN_TEST(test_jsonrpc_no_response_to_empty_request);
	//RUN_TEST(test_request_without_id_shall_not_get_response);
	////RUN_TEST(test_fen);
	//RUN_TEST(test_jsonrpc_exit);
	//RUN_TEST(test_jsonrpc_init);
	//RUN_TEST(test_send_request_edge_cases);
	//RUN_TEST(test_jsonrpc_set_then_get);
	////RUN_TEST(test_jsonrpc_setup);
	//RUN_TEST(test_jsonrpc_status);
	//RUN_TEST(test_utils);
    return UNITY_END();
}
