/*****************************************************************//**
Dump file space header*/
void
fsp_header_dump_fsp_hdr
(
ulint   space_id,
ulint   nxdespage,
ulint   limit,
FILE*   fp_fsp_hdr
);
/*****************************************************************//**
Dump file segement innode*/
void
fsp_header_dump_fseg_page
(
ulint   space_id,
ulint   page_no,
FILE*   fp_fseg_page
);