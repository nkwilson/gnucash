/********************************************************************\
 * qofquery.h -- API for finding objects that can be queried        *
 *                                                                  *
 * This program is free software; you can redistribute it and/or    *
 * modify it under the terms of the GNU General Public License as   *
 * published by the Free Software Foundation; either version 2 of   *
 * the License, or (at your option) any later version.              *
 *                                                                  *
 * This program is distributed in the hope that it will be useful,  *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the    *
 * GNU General Public License for more details.                     *
 *                                                                  *
 * You should have received a copy of the GNU General Public License*
 * along with this program; if not, contact:                        *
 *                                                                  *
 * Free Software Foundation           Voice:  +1-617-542-5942       *
 * 59 Temple Place - Suite 330        Fax:    +1-617-542-2652       *
 * Boston, MA  02111-1307,  USA       gnu@gnu.org                   *
 *                                                                  *
\********************************************************************/

/** @file qofquery.h
    @breif API for finding objects that can be queried
    @author Copyright (C) 2002 Derek Atkins <warlord@MIT.EDU>
*/


#ifndef QOF_QUERYNEW_H
#define QOF_QUERYNEW_H

#include "GNCId.h"
#include "qofquerycore.h"
#include "qofbook.h"

/** A Query */
typedef struct _QofQuery QofQuery;

/** Query Term Operators, for combining Query Terms */
typedef enum {
  QOF_QUERY_AND=1,
  QOF_QUERY_OR,
  QOF_QUERY_NAND,
  QOF_QUERY_NOR,
  QOF_QUERY_XOR
} QofQueryOp;

/* First/only term is same as 'and' */
#define QOF_QUERY_FIRST_TERM QOF_QUERY_AND

/** Default sort object type */
#define QUERY_DEFAULT_SORT	"GnucashQueryDefaultSortObject"

/** "Known" Object Parameters -- all objects must support these */
#define QOF_QUERY_PARAM_BOOK	"book"
#define QOF_QUERY_PARAM_GUID	"guid"
#define QOF_QUERY_PARAM_ACTIVE	"active" /* it's ok if an object does
					  * not support this */

/** Basic API Functions */

GSList * qof_query_build_param_list (char const *param, ...);

/** Create a new query.  A Query MUST be set with a 'search-for' type.
 *  you can create and set this value in one step or two */
QofQuery * qof_query_create (void);
QofQuery * qof_query_create_for (GNCIdTypeConst obj_type);
void qof_query_destroy (QofQuery *q);

/** Set the object type to be searched for */
void qof_query_search_for (QofQuery *query, GNCIdTypeConst obj_type);

/** Set the book to be searched (you can search multiple books) */
void qof_query_set_book (QofQuery *q, QofBook *book);


/** This is the general function that adds a new Query Term to a query.
 * It will find the 'obj_type' object of the search item and compare
 * the 'param_list' parameter to the predicate data via the comparitor.
 *
 * The param_list is a recursive list of parameters.  For example, you
 * can say 'split->memo' by creating a list of one element, "SPLIT_MEMO".
 * You can say 'split->account->name' by creating a list of two elements,
 * "SPLIT_ACCOUNT" and "ACCOUNT_NAME".  The list becomes the property of
 * the Query.
 *
 * For example:
 *
 * acct_name_pred_data = make_string_pred_data(QOF_STRING_MATCH_CASEINSENSITIVE,
 *					       account_name);
 * param_list = make_list (SPLIT_ACCOUNT, ACCOUNT_NAME, NULL);
 * qof_query_add_term (query, param_list, QOF_COMPARE_EQUAL,
 *		    acct_name_pred_data, QOF_QUERY_AND);
 */

void qof_query_add_term (QofQuery *query, GSList *param_list,
		      QofQueryPredData *pred_data, QofQueryOp op);

void qof_query_add_guid_match (QofQuery *q, GSList *param_list,
			   const GUID *guid, QofQueryOp op);
void qof_query_add_guid_list_match (QofQuery *q, GSList *param_list,
			       GList *guid_list, QofGuidMatch options,
			       QofQueryOp op);

void qof_query_add_boolean_match (QofQuery *q, GSList *param_list, gboolean value,
			      QofQueryOp op);

/** Run the query: */
GList * qof_query_run (QofQuery *query);

/** Return the results of the last query, without re-running */
GList * qof_query_last_run (QofQuery *query);

void qof_query_clear (QofQuery *query);
void qof_query_purge_terms (QofQuery *q, GSList *param_list);
int qof_query_has_terms (QofQuery *q);
int qof_query_num_terms (QofQuery *q);

gboolean qof_query_has_term_type (QofQuery *q, GSList *term_param);

QofQuery * qof_query_copy (QofQuery *q);
QofQuery * qof_query_invert(QofQuery *q);

/** Merges two queries together.  Both queries must be compatible
 * search-types.  If both queries are set, they must search for the
 * same object type.  If only one is set, the resulting query will
 * search for the set type.  If neither query has the search-type set,
 * the result will be unset as well.
 */
QofQuery * qof_query_merge(QofQuery *q1, QofQuery *q2, QofQueryOp op);

/** Like qof_query_merge, but this will merge q2 into q1.  q2 remains
 * unchanged.
 */
void qof_query_merge_in_place(QofQuery *q1, QofQuery *q2, QofQueryOp op);

/** The lists become the property of the Query and will be freed
 * by the query when it is destroyed.
 */
void qof_query_set_sort_order (QofQuery *q,
			   GSList *primary_sort_params,
			   GSList *secondary_sort_params,
			   GSList *tertiary_sort_params);

void qof_query_set_sort_options (QofQuery *q, gint prim_op, gint sec_op,
			     gint tert_op);

void qof_query_set_sort_increasing (QofQuery *q, gboolean prim_inc,
				gboolean sec_inc, gboolean tert_inc);


void qof_query_set_max_results (QofQuery *q, int n);

/** Compare two queries for equality. This is a simplistic
 * implementation -- logical equivalences between different
 * and/or trees are ignored. */
gboolean qof_query_equal (QofQuery *q1, QofQuery *q2);

/* Print the Query in human-readable format.
 * Useful for debugging and development.
 */
void qof_query_print (QofQuery *query);

/* Return the type of data we're querying for */
GNCIdType qof_query_get_search_for (QofQuery *q);

/* Return the list of books we're using */
GList * qof_query_get_books (QofQuery *q);

#endif /* QOF_QUERYNEW_H */