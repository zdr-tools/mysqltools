void 
btr_cur_dump_index_prefetch(
   dict_index_t*    index,        /*!< in: index */
   FILE*            fp_btrcur
);
void
btr_cur_dump_index_record(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_page
);
void
btr_cur_dump_undo_architecture(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_dmp_undo_arch
);
void
btr_cur_dump_rseg_inode(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_dmp_rseg
);
void
btr_cur_dump_undolog_inode(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_dmp_undolog
);
void
log_dump_redo_loghdr(FILE*  fp_dmp_redo_hdr);
void
btr_cur_dump_redo_block(FILE* fp_dmp_redo_block,ulint lsn1,ulint lsn2);
void
btr_cur_dump_rangelsn_recs(FILE* fp_dmp_redo_block,ulint lsn1,ulint lsn2);
void
btr_cur_dump_doublewrite(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_dmp_doublewrite
);