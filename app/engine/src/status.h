#pragma once

// Used as a status flag inside 'struct Engine' to keep track of internal
// state and to exit gracefully if needed.
enum Status {
	STATUS_WAIT,
	STATUS_SEARCH,
	STATUS_QUIT,
};
