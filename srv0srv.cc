unsigned long    srv_zdr_dump_indextree;
unsigned long    srv_zdr_dump_fsp_hdr;
unsigned long    srv_zdr_dump_trxlock;
unsigned long    srv_zdr_dump_redo_hdr;
static   char  srv_zdr_dump_range_lsn_recsstr[FN_REFLEN];
char*    srv_zdr_dump_range_lsn_recs=srv_zdr_dump_range_lsn_recsstr;
static   char srv_zdr_dump_redo_blockstr[FN_REFLEN];
char*    srv_zdr_dump_redo_block=srv_zdr_dump_redo_blockstr;
static   char srv_zdr_dump_fseg_inodestr[FN_REFLEN];
char*    srv_zdr_dump_fseg_inode=srv_zdr_dump_fseg_inodestr;
static   char srv_zdr_dump_pagestr[FN_REFLEN];
char*   srv_zdr_dump_page = srv_zdr_dump_pagestr;

static   char srv_zdr_dump_rseg_nodestr[FN_REFLEN];
static   char  srv_zdr_dump_undolog_nodestr[FN_REFLEN];
my_bool  srv_zdr_dump_undo_architecture;
my_bool  srv_zdr_dump_doublewrite;
char*   srv_zdr_dump_rseg_node=srv_zdr_dump_rseg_nodestr;
char*   srv_zdr_dump_undolog_node=srv_zdr_dump_undolog_nodestr;