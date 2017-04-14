/* wmem_interval_array.h
 * Sorted arrays keyed by intervals - Public defines
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __WMEM_INTERVAL_ARRAY_H__
#define __WMEM_INTERVAL_ARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "wmem_core.h"

#include <glib.h>
#include <epan/range.h>

/**
 * A sorted array keyed by intervals
 * You keep inserting items, then sort the array.
 * sorting also combines items that compare equal into one and adjusts
 * the interval accordingly. find uses binary search to find the item
 *
 * This is particularly useful, if you got many similar items,
 * e.g. ObjectMapping subindices in the XDD.
 *
 * Intervall Trees wouldn't work, because they didn't allow expanding
 * existing intervals. Using an array instead of a tree, additionally offers
 * a possible performance advantage, but it's not that critical here,
 * as finding should only happen in the async frames
 *
 * Much room for optimization in the creation process of the array,
 * but it doesn't matter much, as they aren't created frequently.
 * Finding speed is what matters
 *
 */

typedef struct wmem_iarray wmem_iarray_t;

/**
 * \param allocator wmem pool to use
 * \param elem_size size of elements to add into the iarray
 * \param cmp establishes whether two adjacent elements are equal and thus
 *            shall be combined at sort-time
 *
 * \returns a new interval array or NULL on failure
 *
 * Creates a new interval array.
 * Elements must have range_admin_t as their first element
 * \NOTE The cmp parameter can be used to free resources. When combining,
 * it's always the second argument that's getting removed.
 */

WS_DLL_PUBLIC wmem_iarray_t *
wmem_iarray_new(wmem_allocator_t *allocator, const guint elem_size, GEqualFunc cmp)
G_GNUC_MALLOC;


/** Returns true if the iarr is empty. */

WS_DLL_PUBLIC gboolean
wmem_iarray_is_empty(wmem_iarray_t *iarr);

/** Returns true if the iarr is sorted. */

WS_DLL_PUBLIC gboolean
wmem_iarray_is_sorted(wmem_iarray_t *iarr);


/** Inserts an element */

WS_DLL_PUBLIC void
wmem_iarray_insert(wmem_iarray_t *iarr, guint32 where, range_admin_t *data);

/** Makes array suitable for searching */

WS_DLL_PUBLIC void
wmem_iarray_sort(wmem_iarray_t *iarr);

/*
 * Finds an element in the interval array. Returns NULL if it doesn't exist
 * Calling this is unspecified if the array wasn't sorted before
 */

WS_DLL_PUBLIC range_admin_t *
wmem_iarray_find(wmem_iarray_t *arr, guint32 value);


/** Print ranges within the iarr */

WS_DLL_PUBLIC void
wmem_print_iarray(wmem_iarray_t *iarr);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WMEM_INTERVAL_ARRAY_H__ */

