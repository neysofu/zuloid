#include <cstdlib>
#include <cstring>
#include "catch.hpp"
extern "C" {
#include <sysexits.h>
#include "uci.h"
}

TEST_CASE("'uci_cmd_new' allocates memory when given a NULL pointer.", "[UciCmd]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	REQUIRE(cmd);
	uci_cmd_drop(cmd);
}

TEST_CASE("'uci_cmd_new' is idempotent.", "[UciCmd]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	REQUIRE(cmd == uci_cmd_new(cmd));
	uci_cmd_drop(cmd);
}

TEST_CASE("A newly created 'UciCmd' is an empty command.", "[UciCmd]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	REQUIRE(cmd->argc == 0);
	uci_cmd_drop(cmd);
}

TEST_CASE("'str_to_uci_cmd' can parse empty commands.", "[UciCmd]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	char str_1[] = "";
	char str_2[] = " ";
	char str_3[] = " \t  \n";
	char str_4[] = "\n";
	REQUIRE(str_to_uci_cmd(str_1, strlen(str_1), cmd)->argc == 0);
	REQUIRE(str_to_uci_cmd(str_2, strlen(str_2), cmd)->argc == 0);
	REQUIRE(str_to_uci_cmd(str_3, strlen(str_3), cmd)->argc == 0);
	REQUIRE(str_to_uci_cmd(str_4, strlen(str_4), cmd)->argc == 0);
	uci_cmd_drop(cmd);
}

TEST_CASE("'str_to_uci_cmd' gives the right number of tokens in command strings.", "[UciCmd]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	char str_1[] = "Spam\n";
	char str_2[] = "Wabba      lubba\t\tdub dub \t";
	char str_4[] = "1337";
	char str_5[] = "My hands\vare   typing words :)\n";
	REQUIRE(str_to_uci_cmd(str_1, strlen(str_1), cmd)->argc == 1);
	REQUIRE(str_to_uci_cmd(str_2, strlen(str_2), cmd)->argc == 4);
	REQUIRE(str_to_uci_cmd(str_4, strlen(str_4), cmd)->argc == 1);
	REQUIRE(str_to_uci_cmd(str_5, strlen(str_5), cmd)->argc == 6);
	uci_cmd_drop(cmd);
}

TEST_CASE("The length of command arguments is properly set.", "[UciCmd]") {
	char str[] = "\t Wubba lubba   dub    dub, I am in great pain \tplease help me.\n";
	struct UciCmd *cmd = str_to_uci_cmd(str, strlen(str), NULL);
	REQUIRE(uci_cmd_arg_length(cmd, 0) == 5);
	REQUIRE(uci_cmd_arg_length(cmd, 1) == 5);
	REQUIRE(uci_cmd_arg_length(cmd, 2) == 3);
	REQUIRE(uci_cmd_arg_length(cmd, 3) == 4);
	REQUIRE(uci_cmd_arg_length(cmd, 4) == 1);
	REQUIRE(uci_cmd_arg_length(cmd, 5) == 2);
	REQUIRE(uci_cmd_arg_length(cmd, 6) == 2);
	REQUIRE(uci_cmd_arg_length(cmd, 7) == 5);
	REQUIRE(uci_cmd_arg_length(cmd, 8) == 4);
	REQUIRE(uci_cmd_arg_length(cmd, 9) == 6);
	REQUIRE(uci_cmd_arg_length(cmd, 10) == 4);
	REQUIRE(uci_cmd_arg_length(cmd, 11) == 3);
	uci_cmd_drop(cmd);
}

TEST_CASE("'uci_cmd_iter_new' allocates memory when given a NULL pointer.", "[UciCmd][UciCmdIter]") {
	struct UciCmd *cmd = uci_cmd_new(NULL);
	struct UciCmdIter *iter = uci_cmd_iter_new(NULL, cmd, 0);
	REQUIRE(iter);
	uci_cmd_iter_drop(iter);
	uci_cmd_drop(cmd);
}

TEST_CASE("'uci_cmd_iter_next' stops when the command is over.", "[UciCmd][UciCmdIter]") {
	char str[] = "Spam foo   bar\n";
	struct UciCmd *cmd = str_to_uci_cmd(str, strlen(str), NULL);
	struct UciCmdIter *iter = uci_cmd_iter_new(NULL, cmd, 0);
	REQUIRE(uci_cmd_iter_next(iter));
	REQUIRE(uci_cmd_iter_next(iter));
	REQUIRE(uci_cmd_iter_next(iter));
	REQUIRE(!uci_cmd_iter_next(iter));
	uci_cmd_drop(iter->cmd);
	uci_cmd_iter_drop(iter);
}

TEST_CASE("'uci_cmd_iter_next' produces strings that have the same length as 'uci_cmd_arg_length'", "[UciCmd][UciCmdIter]") {
	char str[] = "Spam foo   bar ahahah \n";
	struct UciCmd *cmd = str_to_uci_cmd(str, strlen(str), NULL);
	struct UciCmdIter *iter = uci_cmd_iter_new(NULL, cmd, 0);
	REQUIRE(strlen(uci_cmd_iter_next(iter)) == uci_cmd_arg_length(cmd, 0));
	REQUIRE(strlen(uci_cmd_iter_next(iter)) == uci_cmd_arg_length(cmd, 1));
	REQUIRE(strlen(uci_cmd_iter_next(iter)) == uci_cmd_arg_length(cmd, 2));
	REQUIRE(strlen(uci_cmd_iter_next(iter)) == uci_cmd_arg_length(cmd, 3));
	uci_cmd_drop(iter->cmd);
	uci_cmd_iter_drop(iter);
}
