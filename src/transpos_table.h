#pragma once

struct TransposTable {

};

struct TransposTable *
transpos_table_new(struct TransposTable *transpos_table);

void
transpos_table_drop(struct TransposTable *transpos_table);
