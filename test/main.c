#include <stdlib.h>
#include <check.h>
#include <string.h>
#include <sysexits.h>
#include "uci.h"

START_TEST
(uci_cmd_new_can_malloc) {
	ck_assert(uci_cmd_new(NULL));
} END_TEST

START_TEST
(uci_cmd_drop_frees) {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	uci_cmd_drop(cmd);
	ck_assert(!cmd);
} END_TEST

START_TEST
(uci_cmd_empty) {
	char *str = "\n";
	struct UciCmd *cmd = str_to_uci_cmd(str, strlen(str), NULL);
	ck_assert_uint_eq(cmd->argc, 0);
} END_TEST

START_TEST
(uci_cmd_arg_len_is_correct) {
	char *str = "Wabba lubba dub dub";
	struct UciCmd *cmd = str_to_uci_cmd(str, strlen(str), NULL);
	ck_assert_int_eq(uci_cmd_arg_len(cmd, 0), 4);
	ck_assert_int_eq(uci_cmd_arg_len(cmd, 1), 4);
	ck_assert_int_eq(uci_cmd_arg_len(cmd, 2), 3);
	ck_assert_int_eq(uci_cmd_arg_len(cmd, 3), 3);
	ck_assert_int_eq(uci_cmd_arg_len(cmd, 1337), 0);
} END_TEST

Suite *
uci_cmd_suite(void) {
	Suite *s = suite_create("UCI");
	TCase *tc_core = tcase_create("Core");
	tcase_add_test(tc_core, uci_cmd_new_can_malloc);
	tcase_add_test(tc_core, uci_cmd_drop_frees);
	suite_add_tcase(s, tc_core);
	return s;
}

int
main(void) {
	size_t num_failed;
	Suite *s = uci_cmd_suite();
	SRunner *sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed == 0) ? EX_OK : EX_SOFTWARE;
}
