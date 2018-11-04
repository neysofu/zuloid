/**
 * @file ttable_node.h
 * @brief Search nodes.
 */

#pragma once

#include "chess/board.h"
#include "globals.h"
#include <stdint.h>
#include <stdlib.h>

struct CandidateLine
{
#if SWITCH_TRAIN
	uint32_t frequency;
	uint32_t uncertainty;
#else
	uint8_t frequency;
#endif
	// This can potentially be a move camouflaged as pointer.
	struct TTableNode *ttnode;
};

/**
 * A transposition. Should fit into as little cache lines as possible.
 *
 * Note: transposition tables don't need to store the chess position at all!
 * This is because the search tree is completely blind, and only supports
 * indirect querying and hash collisions are dealts with children nodes.
 *
 * Nodes should be searcheable for when a new node comes up with the same
 * position and, before inserting the new node, should be able to find out if it
 * exists just yet. What to do at that point?
 * - Check a hash table? It's great, no cache misses, but doesn't scale.
 *
 * There are two different kind of nodes:
 * - Inner nodes, which must provide the board tensors.
 * - Outer nodes.
 *
 * The only search happens at outer nodes, so they're the ones that need board
 * tensors. It's gonna be very resource intensive if every single move is given
 * a board tensor... but let's not forget that I do ~220 moves in one. So, every
 * node exploration triggers a
 */
struct TTableNode
{
	struct BoardTensor *tensor; // Needed only if a node is nonterminal.
	/**
	 * @brief Counts the number of incoming connections to this node. A counter
	 * is necessary for deleting nodes.
	 * not be enough in some cases.
	 */
	/**
	 * It is *technically* possible to have a transposition with more than 256
	 * collisions, but it won't ever happen in normal circumstances; you would
	 * need a crazy position like R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1
	 * and Cthulhu only knows what search parameters. Just to make sure, a
	 * safety pig has been provided below for protecting the engine from any
	 * bug.
	 *                          _
	 *  _._ _..._ .-',     _.._(`))
	 * '-. `     '  /-._.-'    ',/
	 *    )         \            '.
	 *   / _    _    |             \
	 *  |  a    a    /              |
	 *  \   .-.                     ;
	 *   '-('' ).-'       ,'       ;
	 *      '-;           |      .'
	 *         \           \    /
	 *         | 7  .__  _.-\   \
	 *         | |  |  ``/  /`  /
	 *        /,_|  |   /,_/   /
	 *           /,_/      '`-'
	 */
	uint8_t reference_count;
	uint8_t children_count;
	uint16_t frequency_unit;
	struct CandidateLine lines[NUM_AVG_CANDIDATE_MOVES];
};

void
ttable_node_backpropagate(struct TTableNode *parent,
                          struct TTableNode *child,
                          bool *lock);

/**
 * @brief Choose one of @p ttnode's child nodes according to their frequencies.
 * If the chosen node is not instantiated, instantiate it with minimal
 * available information.
 */
struct CandidateLine *
ttable_node_candidate_line(struct TTableNode *ttnode, uint32_t seed);
