/********************************************************************\
 * split-register-p.h -- private split register declarations        *
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

#ifndef SPLIT_REGISTER_P_H
#define SPLIT_REGISTER_P_H

#include "Group.h"
#include "gnc-ledger-display.h"
#include "splitreg.h"


#define SPLIT_TRANS_STR _("-- Split Transaction --")
#define STOCK_SPLIT_STR _("-- Stock Split --")


typedef struct _SRInfo SRInfo;
struct _SRInfo
{
  /* The blank split at the bottom of the register */
  GUID blank_split_guid;

  /* The currently open transaction, if any */
  GUID pending_trans_guid;

  /* A transaction used to remember where to put the cursor */
  Transaction *cursor_hint_trans;

  /* A split used to remember where to put the cursor */
  Split *cursor_hint_split;

  /* A split used to remember where to put the cursor */
  Split *cursor_hint_trans_split;

  /* Used to remember where to put the cursor */
  CursorClass cursor_hint_cursor_class;

  /* If the hints were set by the traverse callback */
  gboolean hint_set_by_traverse;

  /* If traverse is to the newly created split */
  gboolean traverse_to_new;

  /* A flag indicating if the last traversal was 'exact'.
   * See table-allgui.[ch] for details. */
  gboolean exact_traversal;

  /* Indicates that the current transaction is expanded
   * in ledger mode. Meaningless in other modes. */
  gboolean trans_expanded;

  /* set to TRUE after register is loaded */
  gboolean reg_loaded;

  /* flag indicating whether full refresh is ok */
  gboolean full_refresh;

  /* The default account where new splits are added */
  GUID default_account;

  /* The last date recorded in the blank split */
  time_t last_date_entered;

  /* true if the current blank split has been edited and commited */
  gboolean blank_split_edited;

  /* true if the demarcation between 'past' and 'future' transactions
   * should be visible */
  gboolean show_present_divider;

  /* true if we are loading the register for the first time */
  gboolean first_pass;

  /* true if the user has already confirmed changes of a reconciled
   * split */
  gboolean change_confirmed;

  /* User data for users of SplitRegisters */
  gpointer user_data;

  /* hook to get parent widget */
  SRGetParentCallback get_parent;
};


SRInfo * xaccSRGetInfo (SplitRegister *reg);

gncUIWidget xaccSRGetParent (SplitRegister *reg);

Split * sr_get_split (SplitRegister *reg, VirtualCellLocation vcell_loc);
Account * sr_get_default_account (SplitRegister *reg);

Transaction * xaccSRGetTrans (SplitRegister *reg,
                              VirtualCellLocation vcell_loc);

Split * xaccSRGetTransSplit (SplitRegister *reg,
                             VirtualCellLocation vcell_loc,
                             VirtualCellLocation *trans_split_loc);

Split * xaccSRGetCurrentTransSplit (SplitRegister *reg,
                                    VirtualCellLocation *vcell_loc);

gboolean xaccSRFindSplit (SplitRegister *reg,
                          Transaction *trans, Split *trans_split,
                          Split *split, CursorClass cursor_class,
                          VirtualCellLocation *vcell_loc);

void xaccSRShowTrans (SplitRegister *reg, VirtualCellLocation start_loc);

void xaccSRSetTransVisible (SplitRegister *reg,
                            VirtualCellLocation vcell_loc,
                            gboolean visible,
                            gboolean only_blank_split);

void sr_set_cell_fractions (SplitRegister *reg, Split *split);

CellBlock * sr_get_passive_cursor (SplitRegister *reg);
CellBlock * sr_get_active_cursor (SplitRegister *reg);

void sr_set_last_num (SplitRegister *reg, const char *num);

Account * gnc_split_register_get_account (SplitRegister *reg, int cell_type);

gboolean gnc_split_register_recn_cell_confirm (char old_flag, gpointer data);

void LedgerDestroy (SplitRegister *reg);

#endif
