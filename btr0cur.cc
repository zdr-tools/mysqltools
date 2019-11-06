/*****************************************************************//**
Dump file header*/
static void 
btr_cur_dump_file_header(FILE* fp_page,const page_t* page)
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
Dump page header*/
static void 
btr_cur_dump_page_header(FILE* fp_page,const page_t* page)
{
  const byte*    rec=page;
  fprintf(fp_page,"--------------------------------\n"
  "PAGE_HEADER :\n");
  fprintf(fp_page,"PAGE_N_DIR_SLOTS: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_HEAP_TOP: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_HEAP: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_FREE: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_GARBAGE: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_LAST_INSERT: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_DIRECTION: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_DIRECTION: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_N_RECS: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_MAX_TRX_ID: %02x %02x %02x %02x %02x %02x %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
  rec+=8;
  fprintf(fp_page,"PAGE_LEVEL: %02x %02x (%lu)\n",rec[0],rec[1],mach_read_from_2(rec));
  rec+=2;
  fprintf(fp_page,"PAGE_INDEX_ID: %02x %02x %02x %02x %02x %02x %02x %02x  (%lu)\n",rec[0],rec[1],rec[2],rec[3],rec[4],rec[5],rec[6],rec[7],mach_read_from_8(rec));
  rec+=8;
  fprintf(fp_page,"PAGE_BTR_SEG_LEAF: \n"
   "   SPACE_ID: %02x %02x %02x %02x \n"
   "   PAGE_NO:  %02x %02x %02x %02x (%lu)\n"
   "   OFFSET:   %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                           ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                           ,rec[8],rec[9],mach_read_from_2(rec+8));
  rec+=10;
  fprintf(fp_page,"PAGE_BTR_SEG_TOP: \n"
   "   SPACE_ID: %02x %02x %02x %02x \n"
   "   PAGE_NO:  %02x %02x %02x %02x (%lu)\n"
   "   OFFSET:   %02x %02x (%lu)\n",rec[0],rec[1],rec[2],rec[3]
                           ,rec[4],rec[5],rec[6],rec[7],mach_read_from_4(rec+4)
                           ,rec[8],rec[9],mach_read_from_2(rec+8));
}
/*****************************************************************//**
Dump index page slots*/
static void
btr_cur_dump_page_slots(
FILE* fp_page,
const page_t* page)
{
    ulint            n;
    ulint            i;
    page_dir_slot_t*    slot;

    n = page_dir_get_n_slots(page);

    fprintf(fp_page, "--------------------------------\n"
        "PAGE SLOTS\n"
        "Page address %p\n"
        "Directory stack top at offs: %lu; number of slots: %lu\n",
        page, (ulong) page_offset(page_dir_get_nth_slot(page, n - 1)),
        (ulong) n);
    for (i = 0; i < n; i++) {
        slot = page_dir_get_nth_slot(page, i);
            fprintf(fp_page,
                "Contents of slot: %lu: addr:%p n_owned: %lu,"
                " rec offs: %lu\n",
                (ulong) i,slot,
                (ulong) page_dir_slot_get_n_owned(slot),
                (ulong)
                page_offset(page_dir_slot_get_rec(slot)));
    }
    fprintf(fp_page, "Total of %lu records\n"
        "--------------------------------\n",
        (ulong) (PAGE_HEAP_NO_USER_LOW + page_get_n_recs(page)));

}
/*****************************************************************//**
Dump record extra_size*/
static void
btr_cur_dump_record_extra_size(
     FILE*               fp_page, /*!< in: file pointer*/
     const rec_t*        rec,    /*!< in: physical record */
     const dict_index_t* index  /*!< in: record descriptor */
)
{
    ulint    n=0;

    ut_ad(rec);
    ut_ad(index);
    if (dict_table_is_comp(index->table)) 
    {
        switch (UNIV_EXPECT(rec_get_status(rec),
                    REC_STATUS_ORDINARY)) 
        {
        case REC_STATUS_ORDINARY:
            n = dict_index_get_n_fields(index);
            break;
        case REC_STATUS_NODE_PTR:
            /* Node pointer records consist of the
            uniquely identifying fields of the record
            followed by a child page number field. */
            n = dict_index_get_n_unique_in_tree_nonleaf(index) + 1;
            break;
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* infimum or supremum record */
            n = 1;
            break;
        default:
            ut_error;
            return ;
        }
    } 
    else 
    {
        n = rec_get_n_fields_old(rec);
    }
    ulint    i    = 0;    
    if (dict_table_is_comp(index->table))
    {
        const byte*    nulls;
        const byte*    lens;
        const byte*    lens_end;
        dict_field_t*    field;
        ulint        null_mask;
        ulint        vfield_cnt=0;
        ulint        vfield_byte=0;
        ulint        null_fields=0;
        ulint        status = rec_get_status(rec);
        ulint        n_node_ptr_field = ULINT_UNDEFINED;

        switch (UNIV_EXPECT(status, REC_STATUS_ORDINARY)) {
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* the field is 8 bytes long */
          fprintf(fp_page,"rec_new_extra_5_bytes: %02x %02x %02x %02x %02x\n",rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
          return;
        case REC_STATUS_NODE_PTR:
            n_node_ptr_field
                = dict_index_get_n_unique_in_tree_nonleaf(
                    index);
            break;
        case REC_STATUS_ORDINARY:
            break;
        }

        nulls = rec - (REC_N_NEW_EXTRA_BYTES + 1);
        lens =lens_end= nulls - UT_BITS_IN_BYTES(index->n_nullable);
        null_mask = 1;

        /* read the lengths of fields 0..n */
        do 
        {
            ulint    len=0;
            if (UNIV_UNLIKELY(i == n_node_ptr_field)) {
                len = REC_NODE_PTR_SIZE;
                goto resolved;
            }

            field = dict_index_get_nth_field(index, i);
            if (!(dict_field_get_col(field)->prtype & DATA_NOT_NULL)) 
            {
                /* nullable field => read the null flag */

                if (UNIV_UNLIKELY(!(byte) null_mask)) 
                {
                    nulls--;
                    null_mask = 1;
                }

                if (*nulls & null_mask) 
                {
                    null_mask <<= 1;
                    /* No length is stored for NULL fields.
                    We do not advance offs, and we set
                    the length to zero and enable the
                    SQL NULL flag in offsets[]. */
                    len = 0;
                    null_fields++;
                    goto resolved;
                }
                null_mask <<= 1;
            }

            if (UNIV_UNLIKELY(!field->fixed_len)) 
            {
                const dict_col_t*    col
                    = dict_field_get_col(field);
                /* DATA_POINT should always be a fixed
                length column. */
                /* Variable-length field: read the length */
                len = *lens--;
                vfield_cnt++;
                vfield_byte++;
                /* If the maximum length of the field
                is up to 255 bytes, the actual length
                is always stored in one byte. If the
                maximum length is more than 255 bytes,
                the actual length is stored in one
                byte for 0..127.  The length will be
                encoded in two bytes when it is 128 or
                more, or when the field is stored
                externally. */
                if (DATA_BIG_COL(col)) 
                {
                    if (len & 0x80) 
                    {
                        lens--;
                        vfield_byte++;
                        goto resolved;
                    }
                }

            } 
            else 
            {
                len = field->fixed_len;
            }
resolved:
         len=0;
        } while (++i < n);
        fprintf(fp_page,"(%lu) variable-length fields  (%lu Bytes):\n",vfield_cnt,vfield_byte);
        uint i=0;
        lens++;
        for(i=0;lens<=lens_end;)
        {  
           i++;
           fprintf(fp_page," %02x",*lens++);
           if(!(i%16))
            fprintf(fp_page,"\n");
        }
        if(i!=0)
           fprintf(fp_page,"\n");
        fprintf(fp_page,"(%u) nullable fields and %lu fields is null. (%u Bytes): \n",(index->n_nullable),null_fields
                                                                                ,UT_BITS_IN_BYTES(index->n_nullable));
        for(i=0;lens<=nulls;lens++)
        {  
           i++;
           fprintf(fp_page," %u%u%u%u%u%u%u%u ",(1&(lens[0]>>7)),(1&(lens[0]>>6))
                                               ,(1&(lens[0]>>5)),(1&(lens[0]>>4))
                                               ,(1&(lens[0]>>3)),(1&(lens[0]>>2)),
                                               (1&(lens[0]>>1)),(1&lens[0])
                                );
           if(!(i%2))
            fprintf(fp_page,"\n");
        }
        if(i!=0)
        fprintf(fp_page,"\n");
        fprintf(fp_page,"rec_new_extra_5_bytes: %02x %02x %02x %02x %02x\n",rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
    }
    else
    {
        /* Old-style record: determine extra size and end offsets */
        if(page_rec_is_infimum(rec))
        {
           fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
           return;
        }
        if(page_rec_is_supremum(rec))
        {
          fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
          return;
        }
        if (rec_get_1byte_offs_flag(rec)) 
        {
            /* Determine offsets to fields */
            fprintf(fp_page,"data offsets(one-byte):\n");
            do {
            fprintf(fp_page," %02x",rec[-(REC_N_OLD_EXTRA_BYTES + n -i)]);
            i++;
            if(0==i%16)
              fprintf(fp_page,"\n");
            } while (i<n);
            if(i!=0)
              fprintf(fp_page,"\n");
        } 
        else 
        {
            /* Determine offsets to fields */
            fprintf(fp_page,"data offsets(two-bytes):\n");
            do {
                fprintf(fp_page," %02x %02x",rec[-(REC_N_OLD_EXTRA_BYTES + 2*(n -i))]
                                            ,rec[-(REC_N_OLD_EXTRA_BYTES + 2*(n -i)-1)]);
                i++;
                if(0==i%8)
                  fprintf(fp_page,"\n");
            } while (++i < n);
            if(i!=0)
              fprintf(fp_page,"\n");
        }
        fprintf(fp_page,"rec_old_extra_6_bytes: %02x %02x %02x %02x %02x %02x\n",rec[-6],rec[-5],rec[-4],rec[-3],rec[-2],rec[-1]);
    }
    return;
}
/*****************************************************************//**
Dump index one record*/
static void
btr_cur_dump_one_record(
        FILE*               fp_page, /*!< in: file pointer*/
        const rec_t*        rec,    /*!< in: physical record */
        const dict_index_t* index  /*!< in: record descriptor */
        ) 
{
    ulint    n;
    ut_ad(rec);
    ut_ad(index);
    if (dict_table_is_comp(index->table)) 
    {
        switch (UNIV_EXPECT(rec_get_status(rec),
                    REC_STATUS_ORDINARY)) 
        {
        case REC_STATUS_ORDINARY:
            n = dict_index_get_n_fields(index);
            break;
        case REC_STATUS_NODE_PTR:
            /* Node pointer records consist of the
            uniquely identifying fields of the record
            followed by a child page number field. */
            n = dict_index_get_n_unique_in_tree_nonleaf(index) + 1;
            break;
        case REC_STATUS_INFIMUM:
        case REC_STATUS_SUPREMUM:
            /* infimum or supremum record */
            n = 1;
            break;
        default:
            return ;
        }
    } 
    else 
    {
        n = rec_get_n_fields_old(rec);
    }
    /* The offsets header consists of the allocation size at
    offsets[0] and the REC_OFFS_HEADER_SIZE bytes. */
    ulint    i    = 0;    
    if (dict_table_is_comp(index->table))
    {
        const byte*    nulls;
        const byte*    lens;
        const byte*    rec_ptr=rec;
        dict_field_t*    field;
        ulint        null_mask;
        ulint        status = rec_get_status(rec);
        ulint        n_node_ptr_field = ULINT_UNDEFINED;

        switch (UNIV_EXPECT(status, REC_STATUS_ORDINARY)) {
        case REC_STATUS_INFIMUM:
            fprintf(fp_page," This record is infimum.\n");
            return;
        case REC_STATUS_SUPREMUM:
            /* the field is 8 bytes long */
            fprintf(fp_page," This record is supremum.\n");
            return;
        case REC_STATUS_NODE_PTR:
            n_node_ptr_field
                = dict_index_get_n_unique_in_tree_nonleaf(
                    index);
            break;
        case REC_STATUS_ORDINARY:
            break;
        }

        nulls = rec - (REC_N_NEW_EXTRA_BYTES + 1);
        lens = nulls - UT_BITS_IN_BYTES(index->n_nullable);
        null_mask = 1;

        /* read the lengths of fields 0..n */
        do {
            ulint    len=0;
            if (UNIV_UNLIKELY(i == n_node_ptr_field)) {
                len = REC_NODE_PTR_SIZE;
                goto resolved;
            }

            field = dict_index_get_nth_field(index, i);
            if (!(dict_field_get_col(field)->prtype & DATA_NOT_NULL)) 
            {
                /* nullable field => read the null flag */

                if (UNIV_UNLIKELY(!(byte) null_mask)) 
                {
                    nulls--;
                    null_mask = 1;
                }

                if (*nulls & null_mask) 
                {
                    null_mask <<= 1;
                    /* No length is stored for NULL fields.
                    We do not advance offs, and we set
                    the length to zero and enable the
                    SQL NULL flag in offsets[]. */
                    len = 0;
                    goto resolved;
                }
                null_mask <<= 1;
            }

            if (UNIV_UNLIKELY(!field->fixed_len)) 
            {
                const dict_col_t*    col
                    = dict_field_get_col(field);
                /* DATA_POINT should always be a fixed
                length column. */
                /* Variable-length field: read the length */
                len = *lens--;
                /* If the maximum length of the field
                is up to 255 bytes, the actual length
                is always stored in one byte. If the
                maximum length is more than 255 bytes,
                the actual length is stored in one
                byte for 0..127.  The length will be
                encoded in two bytes when it is 128 or
                more, or when the field is stored
                externally. */
                if (DATA_BIG_COL(col)) 
                {
                    if (len & 0x80) 
                    {
                        /* 1exxxxxxx xxxxxxxx */

                        len <<= 8;
                        len |= *lens--;

                        /* B-tree node pointers
                        must not contain externally
                        stored columns.  Thus
                        the "e" flag must be 0. */
                        ut_a(!(len & 0x4000));
                        len = len & 0x3fff;

                        goto resolved;
                    }
                }

            } 
            else 
            {
                len = field->fixed_len;
            }
resolved:
        if(UNIV_UNLIKELY(i == n_node_ptr_field)) 
        {
            fprintf(fp_page,"col %ld: nm: [child_page_no] len: %ld \n",i,len);
        }
        else
        {
            fprintf(fp_page,"col %ld: nm:[%s] len: %ld \n",i,(const char *)(field->name),len);
        }
        if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
           11==strlen((const char *)(field->name))
          )
        {
          roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
          ibool  is_insert;
          ulint  rseg_id;
          ulint  page_no;
          ulint  page_offset;
          trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
          fprintf(fp_page,"insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
        }
        for(ulint j=0;j<len;)
        {
          fprintf(fp_page," %02x",rec_ptr[j++]);
          if(!(j%25))
          fprintf(fp_page,"\n");
        }
        rec_ptr+=len;
        if(len)
        fprintf(fp_page,"\n");
      } while (++i < n);
    }
    else
    {
        /* Old-style record: determine extra size and end offsets */
        ulint  offs_prev=0;
        ulint  offs_new=0;
        ulint  len=0;
        const byte*    rec_ptr=rec;
        dict_field_t*    field;
        if(page_rec_is_infimum(rec))
        {
          fprintf(fp_page,"this record is infimum(Old-style).\n");
          return;
        }
        if(page_rec_is_supremum(rec))
        {
          fprintf(fp_page,"this record is supremum.\n");
          return;
        }
        if (rec_get_1byte_offs_flag(rec)) 
        {
            /* Determine offsets to fields */
            do {
                offs_new= rec_1_get_field_end_info(rec, i);
                if (offs_new & REC_1BYTE_SQL_NULL_MASK) 
                {
                    offs_new &= ~REC_1BYTE_SQL_NULL_MASK;
                }
                len=(offs_new>offs_prev)?(offs_new-offs_prev):0;
                offs_prev=offs_new;
                field=dict_index_get_nth_field(index, i);
                fprintf(fp_page,"col %ld: nm: [%s] len: %ld \n",i,(const char *)(field->name),len);
                if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
                   11==strlen((const char *)(field->name))
                  )
                {
                  roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
                  ibool  is_insert;
                  ulint  rseg_id;
                  ulint  page_no;
                  ulint  page_offset;
                  trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
                  fprintf(fp_page,"insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
                }
                for(ulint j=0;j<len;)
                {
                  fprintf(fp_page," %02x",rec_ptr[j++]);
                 if(!(j%25))
                  fprintf(fp_page,"\n");
                }
                rec_ptr+=len;
                if(len)
                fprintf(fp_page,"\n");
            } while (++i < n);
        } 
        else 
        {
            /* Determine offsets to fields */
            do 
            {
                offs_new = rec_2_get_field_end_info(rec, i);
                if (offs_new & REC_2BYTE_SQL_NULL_MASK) 
                {
                    offs_new &= ~REC_2BYTE_SQL_NULL_MASK;
                }
                if (offs_new & REC_2BYTE_EXTERN_MASK) 
                {
                    offs_new &= ~REC_2BYTE_EXTERN_MASK;
                }
                len=(offs_new>offs_prev)?(offs_new-offs_prev):0;
                offs_prev=offs_new;
                field=dict_index_get_nth_field(index, i);
                fprintf(fp_page,"col %ld: nm: [%s] len: %ld \n",i,(const char *)(field->name),len);
                if(0==memcmp("DB_ROLL_PTR",(const char *)(field->name),11) && 
                   11==strlen((const char *)(field->name))
                  )
                {
                  roll_ptr_t  roll_ptr=trx_read_roll_ptr(rec_ptr);
                  ibool  is_insert;
                  ulint  rseg_id;
                  ulint  page_no;
                  ulint  page_offset;
                  trx_undo_decode_roll_ptr(roll_ptr,&is_insert,&rseg_id,&page_no,&page_offset);
                  fprintf(fp_page,"insert:%lu space_id:%lu page_no:%lu offset:%lu \n",
                         is_insert,trx_undo_get_space_id(rseg_id),page_no,page_offset);
                }
                
                for(ulint j=0;j<len;)
                {
                  fprintf(fp_page," %02x",rec_ptr[j++]);
                 if(!(j%25))
                  fprintf(fp_page,"\n");
                }
                rec_ptr+=len;
                if(len)
                fprintf(fp_page,"\n");
            } while (++i < n);
        
        }
    }
    return;
}
/*****************************************************************//**
Dump index page record*/
void
btr_cur_dump_index_record(
  ulint         space_id,
  ulint         page_no,
  FILE*         fp_page
)
{
    page_cur_t      page_cursor;
   buf_block_t*    block=NULL;
   page_t*         page;
   mtr_t           mtr;
   rec_t*        node_ptr;
   uint           i=0,n_recs=0;

   page_id_t  page_id(space_id,page_no);
   bool            found;
   const page_size_t&    page_size
       = fil_space_get_page_size(page_id.space(), &found);
   if(false==found)
   {
      fprintf(fp_page,"space_id=%ld is not exist\n",space_id);
      return;
   }
   mtr_start(&mtr);
   block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                    BUF_GET, __FILE__, __LINE__, &mtr);
   if(NULL==block)
   {
     fprintf(fp_page,"space_id=%ld page_no=%ld is not exist\n",space_id,page_no);
     return;
   }
   page = buf_block_get_frame(block);
   if(FIL_PAGE_INDEX != fil_page_get_type(page) &&
      FIL_PAGE_RTREE != fil_page_get_type(page))
   {
      fprintf(fp_page,"space_id=%ld page_no=%ld File page type is not B-tree node \n",space_id,page_no);
      return;
   }
   index_id_t index_id=btr_page_get_index_id(page);
   dict_index_t* index=dict_index_get_if_in_cache(index_id);
   if(NULL==index)
   {
     fprintf(fp_page,"The index_id %ld not in cache",index_id);
     return;
   }
   page_cursor.index=index;
   page_cur_set_before_first(block, &page_cursor);
   ibool  is_comp=dict_table_is_comp(index->table);
   fprintf(fp_page,"-----begin dump page record\n");
    btr_cur_dump_file_header(fp_page,page);
    btr_cur_dump_page_header(fp_page,page+PAGE_HEADER);
    btr_cur_dump_page_slots(fp_page,page);
   /*for(; !page_cur_is_after_last(&page_cursor);page_cur_move_to_next(&page_cursor))*/
   n_recs=page_get_n_recs(page);
   node_ptr = page_cur_get_rec(&page_cursor);
   for(;i<=n_recs+1;node_ptr=page_rec_get_next(node_ptr))
   {
      fprintf(fp_page,"row#%d del:%ld n_owned:%ld heap_no:%ld current_offs:%ld next_record=%ld\n",
      i++,rec_get_deleted_flag(node_ptr,is_comp),is_comp? rec_get_n_owned_new(node_ptr):rec_get_n_owned_old(node_ptr),is_comp? rec_get_heap_no_new(node_ptr):rec_get_heap_no_old(node_ptr)
       ,page_offset(node_ptr),rec_get_next_offs(node_ptr,is_comp));
       btr_cur_dump_record_extra_size(fp_page,node_ptr,index);
       btr_cur_dump_one_record(fp_page,node_ptr,index);
    }
    
    node_ptr=page_header_get_ptr(page,PAGE_FREE);
    if(node_ptr)
    {
       fprintf(fp_page,"---------begin purged records dump\n");    
       i=0;
       while(node_ptr)
       {
           fprintf(fp_page,"row#%d del:%ld n_owned:%ld heap_no:%ld current_offs:%lu next_record:%ld\n",
           i++,rec_get_deleted_flag(node_ptr,is_comp),is_comp? rec_get_n_owned_new(node_ptr):rec_get_n_owned_old(node_ptr),is_comp? rec_get_heap_no_new(node_ptr):rec_get_heap_no_old(node_ptr)
            ,page_offset(node_ptr),rec_get_next_offs(node_ptr,is_comp));
            btr_cur_dump_record_extra_size(fp_page,node_ptr,index);
            btr_cur_dump_one_record(fp_page,node_ptr,index);
         node_ptr=page_rec_get_next(node_ptr);
       }
       fprintf(fp_page,"---------end purged records dump\n");
    }
    fprintf(fp_page,"-----end dump page record\n");
   mtr_commit(&mtr);
   return;
}
/*****************************************************************//**
Dump index page addr*/
void
btr_cur_dump_index_page(
   ulint         space_id,
   ulint         page_no,
   btr_cur_t*    cursor,
   page_size_t   page_size,
    uint           roothegiht,
  FILE*            fp_btrcur
)
{
    char  spacemsg[128]={0};
    buf_block_t*    block=NULL;
    page_t*         page;
    mtr_t           mtr;
    page_cur_t      page_cursor;
    uint level      =0;
    rec_t*        node_ptr;
    ulint        offsets_[REC_OFFS_NORMAL_SIZE];
    ulint*        offsets        = offsets_;
    mem_heap_t*    heap        = NULL;
    page_id_t  page_id(space_id,page_no);
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    page = buf_block_get_frame(block);
    level = btr_page_get_level(page, mtr);
    memset(spacemsg,' ',(roothegiht-level)*5);
    if(level>=1)
    {
       fprintf(fp_btrcur,"%sbranch:[%p][%ld]level:%u \n",spacemsg,page,page_get_page_no(page),level);
       page_cur_set_before_first(block, &page_cursor);
       page_cur_move_to_next(&page_cursor);
       for(; !page_cur_is_after_last(&page_cursor);page_cur_move_to_next(&page_cursor))
       {
          node_ptr = page_cur_get_rec(&page_cursor);
           rec_offs_init(offsets_);
          offsets = rec_get_offsets(node_ptr, cursor->index, offsets,
                      ULINT_UNDEFINED, &heap);
          ulint child_page_no=btr_node_ptr_get_child_page_no(node_ptr, offsets);
          btr_cur_dump_index_page(space_id,child_page_no,cursor,page_size,roothegiht,fp_btrcur);
        }
    }
    else
    {
      fprintf(fp_btrcur,"%sleaf:[%p][%ld]\n",spacemsg,page,page_get_page_no(page));
      mtr_commit(&mtr);
      return;   
    }
    if (heap) 
    {
      mem_heap_free(heap);
    }
    mtr_commit(&mtr);
    return;
}
/*****************************************************************//**
Fetch every node of an index*/
void 
btr_cur_dump_index_prefetch(
   dict_index_t*    index,        /*!< in: index */
   FILE*            fp_btrcur
)
{
    btr_cur_t      pbtrcur;
    mtr_t          mtr;
    ulint        root_height = 0; /* remove warning */
    
    page_id_t        page_id(dict_index_get_space(index),
                    dict_index_get_page(index));
    const page_size_t&    page_size = dict_table_page_size(index->table);
    buf_block_t*    block;        
    page_t*        page;
    mtr_start(&mtr);
    block = buf_page_get_gen(page_id, page_size, RW_NO_LATCH, NULL,
                     BUF_GET, __FILE__, __LINE__, &mtr);
    page = buf_block_get_frame(block);
    root_height = btr_page_get_level(page, mtr);
    mtr_commit(&mtr);
    pbtrcur.index=index;
    fprintf(fp_btrcur,"-----begin index tree dump\n");
    btr_cur_dump_index_page(dict_index_get_space(index),dict_index_get_page(index),&pbtrcur,page_size,root_height,fp_btrcur);
    fprintf(fp_btrcur,"-----end index tree dump\n");
}