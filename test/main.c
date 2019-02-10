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

extern void test_method_get_settings(void);
extern void test_method_init_metadata(void);
extern void test_method_init_reentrancy(void);
extern void test_method_setup_basic_fen(void);

extern void test_fen_init(void);
extern void test_fen(void);

extern void test_file_to_char(void);
extern void test_char_to_file(void);

extern void test_color_other(void);

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

	RUN_TEST(test_method_get_settings);
	RUN_TEST(test_method_init_metadata);
	RUN_TEST(test_method_init_reentrancy);
	RUN_TEST(test_method_setup_basic_fen);

	RUN_TEST(test_fen_init);
	RUN_TEST(test_fen);

	RUN_TEST(test_file_to_char);
	RUN_TEST(test_char_to_file);

	RUN_TEST(test_color_other);

    return UNITY_END();
}
