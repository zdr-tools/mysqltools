/*****************************************************************//**
Dump file header*/
static void 
fsp_header_dump_file_header(FILE* fp_page,const page_t* page)
{
  const byte*    rec=page;
  fprintf(fp_page,"--------------------------------\n"
  "FILE_HEADER:\n");
  fprintf(fp_page,"FIL_PAGE_SPACE_OR_CHKSUM: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_OFFSET: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_PREV: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_NEXT: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
  rec+=4;
  fprintf(fp_page,"FIL_PAGE_LSN: %02x %02x %02x %02x %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]);
  rec+=8;
  fprintf(fp_page,"FIL_PAGE_TYPE: %02x %02x\n",rec[0],rec[1]);
  rec+=2;
  fprintf(fp_page,"FIL_PAGE_FILE_FLUSH_LSN: %02x %02x %02x %02x %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]);
  rec+=8;
  fprintf(fp_page,"FIL_PAGE_SPACE_ID: %02x %02x %02x %02x\n",rec[0],rec[1],rec[2],rec[3]);
}
/*****************************************************************//**
Dump fsp_hdr_body*/
static void
fsp_header_dump_fsp_hdr_body(
FILE* fp_page,
const page_t* page)
{
 const byte*    rec=page+FSP_HEADER_OFFSET;
 fprintf(fp_page, "--------------------------------\n"
     "FSP_HEADER:\n");
 fprintf(fp_page,"FSP_SPACE_ID: %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]);
 rec+=4;
 fprintf(fp_page,"FSP_NOT_USED: %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]);
 rec+=4;
 fprintf(fp_page,"TABLESPACE_SIZE: %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
 rec+=4;
 fprintf(fp_page,"FSP_FREE_LIMIT: %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
 rec+=4;
 fprintf(fp_page,"FSP_SPACE_FLAGS: %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]);
 rec+=4;
 fprintf(fp_page,"FSP_FRAG_N_USED(number of used pages in the FSP_FREE_FRAG list):\n"" %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]);
 rec+=4;
 fprintf(fp_page,"FSP_FREE(list of free extents):\n"
    "        FLST_LEN: %02x %02x %02x %02x \n"
    "        FLST_FIRST:\n"
    "           PAGE:%02x %02x %02x %02x (%lu)\n"
    "           OFFSETS:%02x %02x (%lu)\n"
    "        FLST_LAST:\n"
    "           PAGE:%02x %02x %02x %02x (%lu)\n"
    "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                                     ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                     ,rec[8],rec[9],mach_read_from_2(rec+8)
                                     ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                     ,rec[14],rec[15],mach_read_from_2(rec+14));
 rec+=16;
 fprintf(fp_page,"FSP_FREE_FRAG(list of partially free extents not belonging to any segment):\n"
     "        FLST_LEN: %02x %02x %02x %02x \n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                                     ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                     ,rec[8],rec[9],mach_read_from_2(rec+8)
                                     ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                     ,rec[14],rec[15],mach_read_from_2(rec+14));

 rec+=16;
 fprintf(fp_page,"FSP_FULL_FRAG(list of full extents not belonging to any segment):\n"
     "        FLST_LEN: %02x %02x %02x %02x \n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                                     ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                     ,rec[8],rec[9],mach_read_from_2(rec+8)
                                     ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                     ,rec[14],rec[15],mach_read_from_2(rec+14));

  rec+=16;
  fprintf(fp_page,"FSP_SEG_ID(which give the first unused segment id): \n"
  " %02x %02x %02x %02x %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]
      ,rec[4],rec[5],rec[6],rec[7]);
  rec+=8;
  fprintf(fp_page,"FSP_SEG_INODES_FULL(list of pages containing segment headers, where all the segment inode slots are reserved):\n"
      "        FLST_LEN: %02x %02x %02x %02x \n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                                     ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                     ,rec[8],rec[9],mach_read_from_2(rec+8)
                                     ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                     ,rec[14],rec[15],mach_read_from_2(rec+14));

   rec+=16;
   fprintf(fp_page,"FSP_SEG_INODES_FREE(list of pages containing segment headers, where not all the segment header slots are reserved):\n"
       "        FLST_LEN: %02x %02x %02x %02x \n"
       "        FLST_FIRST:\n"
       "           PAGE:%02x %02x %02x %02x (%lu)\n"
       "           OFFSETS:%02x %02x (%lu)\n"
       "        FLST_LAST:\n"
       "           PAGE:%02x %02x %02x %02x (%lu)\n"
       "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                                     ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                     ,rec[8],rec[9],mach_read_from_2(rec+8)
                                     ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                     ,rec[14],rec[15],mach_read_from_2(rec+14));
}
/*****************************************************************//**
Dump fsp_xdesc seg*/
static void
fsp_header_dump_fsp_xdesc(
FILE* fp_page,
const page_t* page)
{
    const byte*    rec=page;
    fprintf(fp_page, "--------------------------------\n"
        "File extent descriptor data structure\n"
        "offset %lu:\n",page_offset(rec));
    fprintf(fp_page,"XDES_ID(The identifier of the segment to which this extent belongs):\n"
     " %02x %02x %02x %02x %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]);
     rec+=8;
     fprintf(fp_page,"XDES_FLST_NODE(The list node data structure for the descriptors):\n"
      "FLST_PREV:\n"
      "    PAGE_NO:%02x %02x %02x %02x \n"
      "    OFFSETS:%02x %02x (%lu)\n"
      "FLST_NEXT:\n"
      "    PAGE_NO:%02x %02x %02x %02x \n"
      "    OFFSETS:%02x %02x (%lu)\n"
      ,rec[0],rec[1],rec[2],rec[3]
      ,rec[4],rec[5],mach_read_from_2(rec+4)
      ,rec[6],rec[7],rec[8],rec[9]
      ,rec[10],rec[11],mach_read_from_2(rec+10));
      rec+=12;
      fprintf(fp_page,"XDES_STATE(contains state information of the extent)\n"
       "(XDES_FREE-1 XDES_FREE_FRAG-2 XDES_FULL_FRAG-3 XDES_FSEG-4)\n"
       ": %02x %02x %02x %02x \n",rec[0],rec[1],rec[2],rec[3]);
       rec+=4;
       fprintf(fp_page,"XDES_BITS:(XDES_FREE_BIT:0 XDES_CLEAN_BIT:1)\n");
       uint bit0=0,bit1=0,bit2=0,bit3=0,bit4=0,bit5=0,bit6=0,bit7=0;
       for(uint i=0;i<UT_BITS_IN_BYTES(FSP_EXTENT_SIZE * XDES_BITS_PER_PAGE);rec++)
       {
         bit0=(1&rec[0]);
         bit1=(1&(rec[0]>>1));
         bit2=(1&(rec[0]>>2));
         bit3=(1&(rec[0]>>3));
         bit4=(1&(rec[0]>>4));
         bit5=(1&(rec[0]>>5));
         bit6=(1&(rec[0]>>6));
         bit7=(1&(rec[0]>>7));
         fprintf(fp_page," %u%u %u%u %u%u %u%u",bit0,bit1,bit2,bit3,bit4,bit5,bit6,bit7);
         i++;
         if(0==i%4 )
          fprintf(fp_page,"\n");
       }
       fprintf(fp_page,"\n");
}
/*****************************************************************//**
Dump fseg flst node*/
static 
void
fsp_header_dump_fseg_flst_node(
FILE*   fp_fseg_page,
const page_t* page)
{
    const byte*    rec=page+FSEG_PAGE_DATA;
    fprintf(fp_fseg_page, "--------------------------------\n"
        "FSEG LIST NODE:\n"
        "     PREV NODE \n"
        "       PGAE_NO: %02x %02x %02x %02x (%lu) \n"
        "       OFFSETS: %02x %02x (%lu) \n"
        "     NEXT NODE \n"
        "       PGAE_NO: %02x %02x %02x %02x (%lu)\n"
        "       OFFSETS: %02x %02x (%lu) \n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                            ,rec[4],rec[5],mach_read_from_2(rec+4)
                                            ,rec[6],rec[7],rec[8],rec[9],mach_read_from_4(rec+6)
                                            ,rec[10],rec[11],mach_read_from_2(rec+10));
}
/*****************************************************************//**
Dump one fseg node*/
static 
void
fsp_header_dump_one_fseg_node(
FILE*   fp_fseg_page,
uint    i,
ulint   offsets,
const byte* rec)
{
    //const byte*    rec=page+FSEG_ARR_OFFSET+i*FSEG_INODE_SIZE;
    fprintf(fp_fseg_page, "--------------------------------\n"
        "FSEG NODE %u      OFFSETS: %lu \n",i,offsets);
    fprintf(fp_fseg_page,
    "FSEG_ID:(8 bytes of segment id: if this is 0,it means that the header is unused)\n"
    " %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7]
                ,mach_read_from_8(rec));
    rec+=8;
    fprintf(fp_fseg_page,
    "FSEG_NOT_FULL_N_USED:(number of used segment pages in the FSEG_NOT_FULL list)\n"
    " %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                ,mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_fseg_page,
    "FSEG_FREE:(list of free extents of this segment)\n"
     "        FLST_LEN: %02x %02x %02x %02x (%lu)\n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                         ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                         ,rec[8],rec[9],mach_read_from_2(rec+8)
                                         ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                         ,rec[14],rec[15],mach_read_from_2(rec+14));
    rec+=16;
    fprintf(fp_fseg_page,
    "FSEG_NOT_FULL:(list of partially free extents)\n"
     "        FLST_LEN: %02x %02x %02x %02x (%lu)\n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                         ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                         ,rec[8],rec[9],mach_read_from_2(rec+8)
                                         ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                         ,rec[14],rec[15],mach_read_from_2(rec+14));
    rec+=16;
    fprintf(fp_fseg_page,
    "FSEG_FULL:(list of full extents)\n"
     "        FLST_LEN: %02x %02x %02x %02x (%lu)\n"
     "        FLST_FIRST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n"
     "        FLST_LAST:\n"
     "           PAGE:%02x %02x %02x %02x (%lu)\n"
     "           OFFSETS:%02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec)
                                         ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                                         ,rec[8],rec[9],mach_read_from_2(rec+8)
                                         ,rec[10],rec[11],rec[12],rec[13],mach_read_from_4(rec+10)
                                         ,rec[14],rec[15],mach_read_from_2(rec+14));
    rec+=16;
    fprintf(fp_fseg_page,
    "FSEG_MAGIC_N:(magic number used in debugging)\n"
    " %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],mach_read_from_4(rec));
    rec+=4;
    fprintf(fp_fseg_page,
    "FSEG_FRAG_ARR:(array of individual pages belonging to this segment in fsp fragment extent lists)\n"
    "  FSEG_FRAG_ARR_N_SLOTS:%lu FSEG_FRAG_SLOT_SIZE: 4 Bytes \n",FSEG_FRAG_ARR_N_SLOTS);
    for(uint i=0;i<FSEG_FRAG_ARR_N_SLOTS; rec+=4)
    {
       fprintf(fp_fseg_page, 
        " %02x %02x %02x %02x ",rec[0],rec[1],rec[2],rec[3]);
        i++;
       if(0==i%4)
       fprintf(fp_fseg_page,"\n");
    }
    fprintf(fp_fseg_page,"\n");
    return;
}
/*****************************************************************//**
Dump file space header*/
void
fsp_header_dump_fsp_hdr
(
ulint   space_id,
ulint   nxdespage,/*in: if is 0,the first xdes page*/
ulint   limit,
FILE*   fp_fsp_hdr
)
{
    ulint           page_no=nxdespage*(UNIV_PAGE_SIZE);
    buf_block_t*    block=NULL;
    page_t*         page;
    mtr_t           mtr;
    
    page_id_t  page_id(space_id,page_no);
    bool            found;
    const page_size_t&    page_size
        = fil_space_get_page_size(page_id.space(), &found);
    if(false==found)
    {
        fprintf(fp_fsp_hdr,"space_id=%ld is not exist\n",space_id);
        return;
    }
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
                     
    if(NULL==block)
    {
      fprintf(fp_fsp_hdr,"space_id=%ld is not exist\n",space_id);
      return;
    }
    page = buf_block_get_frame(block);
    if(FIL_PAGE_TYPE_FSP_HDR != fil_page_get_type(page) &&
       FIL_PAGE_TYPE_XDES != fil_page_get_type(page))
    {
       fprintf(fp_fsp_hdr,"File page type is not B-tree node \n");
       return;
    }
    fsp_header_dump_file_header(fp_fsp_hdr,page);
    fsp_header_dump_fsp_hdr_body(fp_fsp_hdr,page);
    if(nxdespage==0)
    {/*xdes page0*/
       limit  = mach_read_from_4(page+FSP_HEADER_OFFSET+ FSP_FREE_LIMIT);
    }
    for(uint i=0;
       i<(limit>UNIV_PAGE_SIZE ? UNIV_PAGE_SIZE:limit);
        i+=FSP_EXTENT_SIZE
        )
    {
      fsp_header_dump_fsp_xdesc(fp_fsp_hdr,page+XDES_ARR_OFFSET+(i/FSP_EXTENT_SIZE)*XDES_SIZE);
    }
    if(limit>UNIV_PAGE_SIZE)
    {
      limit-=UNIV_PAGE_SIZE;
      fsp_header_dump_fsp_hdr(space_id,
       nxdespage+1,
       limit,
       fp_fsp_hdr);
    }
    return;
}

/*****************************************************************//**
Dump file segement innode*/
void 
fsp_header_dump_fseg_page
(ulint   space_id,
 ulint   page_no,
 FILE*   fp_fseg_page)
{
    buf_block_t*    block=NULL;
    page_t*         page;
    mtr_t           mtr;
    
    page_id_t  page_id(space_id,page_no);
    bool            found;
    const page_size_t&    page_size
        = fil_space_get_page_size(page_id.space(), &found);
    if(false==found)
    {
        fprintf(fp_fseg_page,"space_id=%ld is not exist\n",space_id);
        return;
    }
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    if(NULL==block)
    {
      fprintf(fp_fseg_page,"space_id=%ld page_no=%lu is not exist\n",space_id,page_no);
      return;
    }
    page = buf_block_get_frame(block);
    if(FIL_PAGE_INODE != fil_page_get_type(page))
    {
       fprintf(fp_fseg_page,"File page type is not file segment node \n");
       return;
    }
    fsp_header_dump_file_header(fp_fseg_page,page);
    fsp_header_dump_fseg_flst_node(fp_fseg_page,page);
    for (ulint i = 0; i < FSP_SEG_INODES_PER_PAGE(page_size); i++) 
    {   
        fseg_inode_t*    inode=page + FSEG_ARR_OFFSET + FSEG_INODE_SIZE * i;
        if (mach_read_from_8(inode+ FSEG_ID)) 
        {
            /* This is used */
            fsp_header_dump_one_fseg_node(fp_fseg_page,i,FSEG_ARR_OFFSET + FSEG_INODE_SIZE * i,inode);
        }
    }
    return;
}